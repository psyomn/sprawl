/*
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

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netdb.h>

// declarations

struct session {
  std::string host;
  std::string port;
  std::uint16_t port_i;
  std::string message;
  bool wait_for_reply;
  int sock_fd;

  session() : host("127.0.0.1"),
              port("9090"),
              port_i(9090),
              message(""),
              wait_for_reply(false),
              sock_fd(0) {}

  ~session() { close(sock_fd); }

  void make() {
    port_i = 0;
    std::stringstream ss;
    ss << port;
    ss >> port_i;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  }
};

void send_message_to(const struct session& sess) noexcept;
void receive_and_print(const struct session& sess) noexcept;
void print_usage(const char * const name) noexcept;

// implementation

void send_message_to(const struct session& sess) noexcept {
  std::cout << "sending message[" << sess.message << "] to "
            << sess.host << ":"
            << sess.port << std::endl;

  // from what I understand, it seems that getaddrinfo is the more
  // "modern" way of doing things now. I can't be arsed though. the
  // documentation is horrendous, and I just really want to get
  // something very simple down.

  struct sockaddr_in destination_addr = {0};
  destination_addr.sin_family = AF_INET;
  destination_addr.sin_addr.s_addr = inet_addr(sess.host.c_str());
  destination_addr.sin_port = htons(sess.port_i);

  const ssize_t ret = sendto(
    sess.sock_fd,
    reinterpret_cast<const void*>(sess.message.c_str()),
    sess.message.size(),
    0,
    reinterpret_cast<struct sockaddr*>(&destination_addr),
    sizeof(destination_addr));

  if (ret == -1)
    std::cerr << "problem sending message: " << ret << std::endl;
  else
    std::cout << "sent number of bytes: " << ret << std::endl;
}

void receive_and_print(const struct session& sess) noexcept {
  std::cout << "waiting for a reply... " << std::endl;
  char buffer[508] = {0}; // max unfragmented

  const ssize_t ret = recv(
    sess.sock_fd,
    reinterpret_cast<void*>(buffer),
    sizeof(buffer),
    0
  );

  if (ret < 0) {
    std::cerr << "problem receiving message: " << ret << std::endl;
    return;
  }

  const size_t safe_ret = static_cast<size_t>(ret);
  std::cout << "received " << ret << "bytes" << std::endl;
  const std::string ret_string(buffer, safe_ret);
  std::cout << ret_string << std::endl;
}

void print_usage(const char * const name) noexcept {
  fprintf(stderr, "Usage: %s <-s message> [-r receive] [-h host default=127.0.0.1] [-p port default=9090]\n", name);
}

int main(int argc, char* argv[]) {
  struct session sess;
  int opt = 0;

  while ((opt = getopt(argc, argv, "h:p:s:r")) != -1) {
    switch (opt) {
    case 'h':
      sess.host = optarg;
      break;
    case 'p':
      sess.port = optarg;
      break;
    case 's':
      sess.message = optarg;
      break;
    case 'r':
      sess.wait_for_reply = true;
      break;
    default: /* '?' */
      print_usage(argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (argc <= 1) {
    print_usage(argv[0]);
    return EXIT_SUCCESS;
  }

  sess.make();

  send_message_to(sess);
  if (sess.wait_for_reply) receive_and_print(sess);

  return EXIT_SUCCESS;
}
