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

#include "net/udp.h"

// declarations

struct session {
  std::string host;
  std::string port;
  std::uint16_t port_i;
  std::string message;
  bool wait_for_reply;
  int sock_fd;
  psy::net::UDPClient client_;

  // TODO fix ports
  session() : host("127.0.0.1"),
              port("9995"),
              port_i(9995),
              message(""),
              wait_for_reply(false),
              client_({host, port_i})

              {}

  ~session() {}
};

void send_message_to(const struct session& sess);
void receive_and_print(const struct session& sess);
void print_usage(const char * const name);

// implementation

void send_message_to(const struct session& sess) {
  std::cout << "sending message[" << sess.message << "] to "
            << sess.host << ":"
            << sess.port << std::endl;

  std::uint8_t buffer[psy::net::kMaxUDPSize] = {0};

  (void) memcpy(buffer, sess.message.c_str(), sizeof(buffer));

  sess.client_.Send(buffer);

  // TODO: fix these when I figure out the error handling in the UDP
  // api
  // if (ret == -1)
  //   std::cerr << "problem sending message: " << ret << std::endl;
  // else
  //   std::cout << "sent number of bytes: " << ret << std::endl;
}

void receive_and_print(const struct session& sess) {
  std::cout << "waiting for a reply... " << std::endl;

  auto bytes = sess.client_.Receive();
  std::cout << "received " << bytes.size() << "bytes" << std::endl;

  const std::string ret_string(bytes.begin(), bytes.end());
  std::cout << ret_string << std::endl;
}

void print_usage(const char * const name) {
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

  send_message_to(sess);
  if (sess.wait_for_reply) receive_and_print(sess);

  return EXIT_SUCCESS;
}
