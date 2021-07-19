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
#include <memory>
#include <sstream>
#include <string>

#include <getopt.h>
#include <stdio.h>

#include "net/udp.h"

// declarations

struct Session {
  psy::net::UDPClient client_;
  std::string message_;
  bool wait_for_reply_;
  Session(std::string host, std::uint16_t port,
          std::string message, bool wait) :
    client_({host, port}), message_(message), wait_for_reply_(wait) {}
};

struct SessionBuilder {
  std::string host_;
  std::string port_;
  std::uint16_t port_i_;
  std::string message_;
  bool wait_for_reply_;

  SessionBuilder() :
    host_("127.0.0.1"),
    port_("9995"),
    port_i_(9995),
    message_(""),
    wait_for_reply_(false) {}

  SessionBuilder* Host(std::string host) { host_ = host; return this; }
  SessionBuilder* Message(std::string message) { message_ = message; return this; }
  SessionBuilder* WaitForReply(bool wait) { wait_for_reply_ = wait; return this; }

  SessionBuilder* Port(std::string port) {
    std::stringstream ss;
    ss << port_;
    ss >> port_i_;
    return this;
  }

  std::unique_ptr<Session> finalize() {
    return std::unique_ptr<Session>
      (new Session(host_, port_i_, message_, wait_for_reply_));
  }

  ~SessionBuilder() {}
};

void SendMessageTo(std::unique_ptr<Session>& sess);
void ReceiveAndPrint(std::unique_ptr<Session>& sess);
void PrintUsage(const char * const name);

// implementation

void SendMessageTo(std::unique_ptr<Session>& sess) {
  std::cout << "sending message [" << sess->message_ << "] to "
            << sess->client_.GetHost() << ":"
            << sess->client_.GetPort() << std::endl;

  std::uint8_t buffer[psy::net::kMaxUDPSize] = {0};

  (void) memcpy(buffer, sess->message_.c_str(), sizeof(buffer));

  sess->client_.Send(buffer);
  if (sess->client_.Errored())
    std::cerr << "error sending message: " << sess->client_.ErrorString() << std::endl;
}

void ReceiveAndPrint(std::unique_ptr<Session>& sess) {
  std::cout << "waiting for a reply... " << std::endl;

  auto bytes = sess->client_.Receive();
  std::cout << "received " << bytes.size() << "bytes" << std::endl;

  const std::string ret_string(bytes.begin(), bytes.end());
  std::cout << ret_string << std::endl;
}

void PrintUsage(const char * const name) {
  fprintf(stderr, "Usage: %s <-s message> [-r receive] [-h host default=127.0.0.1] [-p port default=9090]\n", name);
}

int main(int argc, char* argv[]) {
  struct SessionBuilder builder;
  int opt = 0;

  while ((opt = getopt(argc, argv, "h:p:s:r")) != -1) {
    switch (opt) {
    case 'h':
      builder.Host(optarg);
      break;
    case 'p':
      builder.Port(optarg);
      break;
    case 's':
      builder.Message(optarg);
      break;
    case 'r':
      builder.WaitForReply(true);
      break;
    default:
      PrintUsage(argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (argc <= 1) {
    PrintUsage(argv[0]);
    return EXIT_SUCCESS;
  }

  auto sess = builder.finalize();
  SendMessageTo(sess);
  if (sess->wait_for_reply_) ReceiveAndPrint(sess);

  return EXIT_SUCCESS;
}
