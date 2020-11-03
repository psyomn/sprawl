#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <stdexcept>
#include <thread>
#include <vector>

#include <cstddef>
#include <cstdint>

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h> // getopt

void Usage(void);
void ClientLoop(void);

const std::uint16_t kDefaultPort = 8000;

class FileTTL {
public:
  explicit FileTTL() :
    files_(), port_(kDefaultPort), address_({0}),
    server_fd_(0), udp_backlog_(5) {
    InitSocket();
  }

  ~FileTTL() {
    DeinitSocket();
  }

  void Consumer() {
    using namespace std::chrono_literals;
    namespace sc = std::chrono;

    while (true) {
      std::this_thread::sleep_for(1s);
      std::lock_guard<std::mutex> guard(mutex_);

      const auto seconds =
        sc::duration_cast<sc::seconds>(sc::system_clock::now().time_since_epoch());
      const auto ss = std::uint64_t(seconds.count());

      // NOTE: this will choke on a lot of insertions. If this was not
      // a toy, a mechanism like timing wheels could be added
      // here. But this is a toy.
      auto files_iterator = files_.begin();
      while (files_iterator != files_.end()) {
        std::cout << files_iterator->first << " ";
        if (ss >= files_iterator->first) {
          for (auto const& filename : files_iterator->second)  {
            std::cout << "removing " << files_iterator->first << "... ";
            if (remove(filename.c_str()) != 0)
              perror("remove file");
            else
              std::cout << "[done]" << std::endl;
          }
            files_iterator = files_.erase(files_iterator);
        } else {
          ++files_iterator;
        }
        std::cout << "." << std::endl;
      }
    }
  }

  void Run() {
    std::thread consumer(&FileTTL::Consumer, this);

    while (true) {
      constexpr size_t message_size = 1024;
      std::uint8_t buffer[message_size] = {0};

      // TODO: replace with recv, and better flags
      ssize_t recv_size = read(server_fd_, &buffer, message_size);

      if (recv_size < 0)
        std::cerr << "error reading from socket" << std::endl;

      // debug
      // for (size_t i = 0; i < message_size; ++i)
      //   std::cout << int(buffer[i]) << " ";
      // std::cout << std::endl;

      std::size_t cursor = 0;

      std::size_t path_size = 0;
      path_size |= std::uint64_t(buffer[cursor++]);
      path_size |= std::uint64_t(buffer[cursor++]) << 8;
      path_size |= std::uint64_t(buffer[cursor++]) << 16;
      path_size |= std::uint64_t(buffer[cursor++]) << 24;

      std::string path;
      for (size_t i = 0; i < path_size; ++i)
        path += static_cast<char>(buffer[cursor++]);

      std::uint64_t timestamp = 0;
      timestamp |= std::uint64_t(buffer[cursor++]);
      timestamp |= std::uint64_t(buffer[cursor++]) << 8;
      timestamp |= std::uint64_t(buffer[cursor++]) << 16;
      timestamp |= std::uint64_t(buffer[cursor++]) << 24;
      timestamp |= std::uint64_t(buffer[cursor++]) << 32;
      timestamp |= std::uint64_t(buffer[cursor++]) << 40;
      timestamp |= std::uint64_t(buffer[cursor++]) << 48;
      timestamp |= std::uint64_t(buffer[cursor++]) << 56;

      InsertPair(timestamp, std::move(path));
    }

    consumer.join();
  }

  inline std::uint16_t GetPort() const { return port_; }

  void InsertPair(std::uint64_t timestamp, std::string&& path) {
    std::lock_guard<std::mutex> guard(mutex_);
    std::cout << "insert" << std::endl;

    auto found = files_.find(timestamp);
    if (found == files_.end()) {
      std::vector<std::string> vec = {std::move(path)};
      files_.insert({timestamp, std::move(vec)});
    } else {
      found->second.push_back(std::move(path));
    }
  }

private:
  // maps get sorted by std::less so we can get older timestamps this
  // way. Ideally this should be changed to something like a priority
  // queue.
  std::map<std::uint64_t, std::vector<std::string>> files_;
  std::uint16_t port_;
  struct sockaddr_in address_;
  int server_fd_;
  int udp_backlog_;
  std::mutex mutex_;

  void InitSocket() {
    if ((server_fd_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
      throw std::runtime_error("could not create socket");

    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = htonl(INADDR_ANY);
    address_.sin_port = htons(port_);

    if ((bind(server_fd_, reinterpret_cast<struct sockaddr *>(&address_), sizeof(address_))) == -1)
      throw std::runtime_error("could not bind address");
  }

  void DeinitSocket() {
    if (server_fd_ == 0) close(server_fd_);
  }
};

void Usage(void) {
  std::cout << "usage: " << std::endl
            << "  fttl [-s] [-c <message>]" << std::endl;
}

void ClientLoop(void) {
  auto help_fn = [](void) -> void {
    std::cout << "usage: " << std::endl
              << "  help - print this" << std::endl
              << "  send - send a message in the format of 'path:unixtimestamp'" << std::endl;
  };

  auto send_message_fn = [](const std::string& path, const std::string& timestamp) -> void {
    int sock_fd = 0;
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
      std::cerr << "could not open socket for client" << std::endl;
      return;
    }

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_port = htons(kDefaultPort);
    address.sin_addr.s_addr = INADDR_ANY;

    constexpr size_t message_len = 1024;
    char buffer[message_len] = {0};
    std::size_t cursor = 0;

    // == Encode path
    // linux max path is ~4096, so using four bytes to encode this
    // should be more than enough
    const std::size_t path_size = path.size();
    buffer[cursor++] = 0xff & path_size;
    buffer[cursor++] = 0xff & (path_size >> 8);
    buffer[cursor++] = 0xff & (path_size >> 16);
    buffer[cursor++] = 0xff & (path_size >> 24);

    for (size_t i = 0; i < path.size(); ++i)
      buffer[cursor++] = path[i];

    // == Encode timestamp
    const std::uint64_t timestamp_i = std::stoull(timestamp);

    // TODO: a better mechanism here for sending buffers is probably
    //   wanted but I'm writing a lot of this as a quick one off (eg:
    //   a very long path could deplete space for timestamp)
    buffer[cursor++] = 0xff & timestamp_i;
    buffer[cursor++] = 0xff & (timestamp_i >> 8);
    buffer[cursor++] = 0xff & (timestamp_i >> 16);
    buffer[cursor++] = 0xff & (timestamp_i >> 24);
    buffer[cursor++] = 0xff & (timestamp_i >> 32);
    buffer[cursor++] = 0xff & (timestamp_i >> 40);
    buffer[cursor++] = 0xff & (timestamp_i >> 48);
    buffer[cursor++] = 0xff & (timestamp_i >> 56);

    for (size_t i = 0; i < message_len; ++i)
      std::cout << int(buffer[i]) << " ";

    ssize_t send_bytes =
      sendto(sock_fd, reinterpret_cast<const void*>(buffer),
             cursor, 0, reinterpret_cast<struct sockaddr*>(&address),
             sizeof(address));

    std::cout << "sent " << send_bytes << " bytes" << std::endl;
  };

  std::string input;
  std::cout << "[fttl] ";
  while (std::getline(std::cin, input)) {
    if (input == "help") help_fn();
    if (input.find("send") != std::string::npos) {
      const std::string delimiter = " ";

      const size_t offset = input.find(delimiter);
      if (offset == std::string::npos) {
        help_fn();
        continue;
      }

      const std::string token =
        input.substr(offset + delimiter.size(), input.size() - offset - 1);

      const std::string timestamp_delimiter = ":";
      const size_t timestamp_offset = token.find(timestamp_delimiter);
      std::cout << "token: " << token << std::endl;
      if (timestamp_offset == std::string::npos) {
        std::cout << "bad format for path:timestamp. example: /tmp/blah/bleh:123123123" << std::endl;
        help_fn();
        continue;
      }

      const auto path = token.substr(0, timestamp_offset);
      const auto unix_timestamp = token.substr(timestamp_offset + 1, token.size() - 1);
      send_message_fn(path, unix_timestamp);
    }

    std::cout << "[fttl] ";
  }
}

int main(int argc, char* argv[]) {
  struct session {
    bool server_mode_flag;
    bool client_mode_flag;
  } session = { false, false };

  int opt = 0;
  while ((opt = getopt(argc, argv, "sc")) != -1) {
    switch (opt) {
    case 's':
      session.server_mode_flag = true;
      break;
    case 'c':
      session.client_mode_flag = true;
      break;
    case '?':
      Usage();
      break;
    default:
      Usage();
      break;
    }
  }

  if (session.server_mode_flag) {
    std::cout << "server running..." << std::endl;
    FileTTL().Run();
    return 0;
  }

  if (session.client_mode_flag) {
    ClientLoop();
    return 0;
  }

  Usage();
  return -1;
}
