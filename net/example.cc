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
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

#include "udp.h"

int main(void) {
  constexpr std::uint16_t kServerPort = 9999;

  std::thread server([]() noexcept {
    auto echo_fn = [](std::uint8_t buffer[psy::net::kMaxUDPSize]) noexcept -> void {
      for (size_t i = 0; i < 10; ++i) std::cout << buffer[i] << ",";
      std::cout << std::endl;
    };

    psy::net::UDPListener(kServerPort).ListenWith(echo_fn);
  });

  std::uint8_t send_buffer[psy::net::kMaxUDPSize] = {0};
  send_buffer[0] = 'a';
  send_buffer[1] = 'b';
  send_buffer[2] = 'c';

  psy::net::UDPClient client("127.0.0.1", kServerPort);
  while (true) {
    client.Send(send_buffer);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  server.join();

  return 0;
}
