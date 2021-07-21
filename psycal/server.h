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
#ifndef CACOPHAGY_PSYCAL_SERVER_H_
#define CACOPHAGY_PSYCAL_SERVER_H_

#include <string>
#include <cstdint>

#include "net/udp.h"

namespace psy::psycal {
  class Server {
  public:
    explicit Server(std::uint16_t port) : server_(port) {}
    void Start() {
      auto constexpr max_sz = psy::net::kMaxUDPSize;
      auto listen_fn = [=](std::uint8_t bf[max_sz]) noexcept -> void {
      };

      server_.ListenWith(listen_fn);
    }
  private:
    psy::net::UDPListener server_;
  };
}

#endif /* CACOPHAGY_PSYCAL_SERVER_H_ */
