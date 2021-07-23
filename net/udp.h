/*
   common code that I'm a little tired of writing again all the
   time. This is not optimized, and is for specific personal use. Keep
   that in mind if you choose to use this.

   Copyright 2021 Simon (psyomn) Symeonidis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef CACOPHAGY_PSYCAL_H_
#define CACOPHAGY_PSYCAL_H_

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <functional>
#include <optional>
#include <string>

#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace psy::net {
  constexpr size_t kMaxUDPSize = 508;

  static std::string ErrorToString(int err);

  /**
   * This is mostly a naive implementation to get most of my use
   * cases satisfied, and I suggest that you don't use this
   */
  class UDPClient {
  public:
    UDPClient(std::string host, std::uint16_t port) :
      buffer_({0}), sock_fd_(socket(AF_INET, SOCK_DGRAM, 0)),
      host_(host), port_(port), destination_addr_({0}),
      last_error_(std::nullopt) {
      destination_addr_.sin_family = AF_INET;
      destination_addr_.sin_addr.s_addr = inet_addr(host_.c_str());
      destination_addr_.sin_port = htons(port_);
    }

    ~UDPClient() {
      if (sock_fd_ != 0) close(sock_fd_);
    }

    UDPClient(const UDPClient& other) = delete;
    UDPClient& operator=(const UDPClient& other) = delete;
    UDPClient(UDPClient&& other) = delete;
    UDPClient& operator=(UDPClient&& other) = delete;

    void Send(const std::uint8_t message[kMaxUDPSize]);
    std::vector<std::uint8_t> Receive();
    bool Errored() const;
    std::string ErrorString() const;

    inline const std::string& GetHost() const { return host_; }
    inline std::uint16_t GetPort() const { return port_; }

  private:
    std::uint8_t buffer_[kMaxUDPSize];
    int sock_fd_;
    std::string host_;
    std::uint16_t port_;
    struct sockaddr_in destination_addr_;
    std::optional<int> last_error_;
  };

  class UDPListener {
  public:
    explicit UDPListener(std::uint16_t port) :
      port_(port), address_({0}), server_fd_(0),
      last_error_(std::nullopt) {

      if ((server_fd_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        throw std::runtime_error("could not create socket");

      address_.sin_family = AF_INET;
      address_.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
      address_.sin_port = htons(port_);

      if ((bind(server_fd_, reinterpret_cast<struct sockaddr *>(&address_), sizeof(address_))) == -1)
        throw std::runtime_error("could not bind address");
    }

    ~UDPListener() {
      if (server_fd_ > 0) close(server_fd_);
    }

    UDPListener(const UDPListener& other) = delete;
    UDPListener& operator=(const UDPListener& other) = delete;
    UDPListener(UDPListener&& other) = delete;
    UDPListener& operator=(UDPListener&& other) = delete;

    void ListenWith(std::function<void(std::uint8_t[kMaxUDPSize])>);
  private:
    std::uint16_t port_;
    struct sockaddr_in address_;
    int server_fd_;
    std::optional<int> last_error_;
  };
}

#endif
