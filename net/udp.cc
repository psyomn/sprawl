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
#include "udp.h"

namespace psy::net {
  std::string ErrorToString(int err) {
    return std::string(strerror(err));
  }

  void UDPClient::Send(const std::uint8_t message[kMaxUDPSize]) {
    const ssize_t ret =
      sendto(sock_fd_,
             reinterpret_cast<const void*>(message),
             kMaxUDPSize,
             0,
             reinterpret_cast<const struct sockaddr*>(&destination_addr_),
             sizeof(destination_addr_));

    if (ret == -1) { /* TODO add error handling here */ }
    else { /* return number of bytes here */ }
  }

  std::vector<std::uint8_t> UDPClient::Receive() {
    char buffer[psy::net::kMaxUDPSize] = {0};
    const ssize_t ret = recv(sock_fd_,
                             reinterpret_cast<void*>(buffer),
                             sizeof(buffer),
                             0);

    if (ret < 0) {
      last_error_ = errno;
      return {};
    }

    last_error_ = (std::nullopt);
    return std::vector<std::uint8_t>(buffer, buffer + sizeof(buffer));
  }

  bool UDPClient::Errored() const {
    return last_error_.has_value();
  }

  void UDPListener::ListenWith(std::function<void(std::uint8_t[kMaxUDPSize])> listenfunc) {
    while (true) {
      std::uint8_t buffer[kMaxUDPSize] = {0};
      ssize_t recv_size = read(server_fd_, &buffer, kMaxUDPSize);

      if (recv_size < 0) {
        last_error_ = errno;
        continue;
      }

      last_error_ = std::nullopt;
      listenfunc(buffer);
    }
  }
}
