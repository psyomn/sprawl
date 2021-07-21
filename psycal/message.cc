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
#include "message.h"

#include "common/codec.h"
namespace psy::psycal::Message {
  std::vector<std::uint8_t> IntoBuffer(const std::vector<Event>& events) {
    std::vector<std::uint8_t> ret;

    for (const auto& event : events) {
      psy::common::Codec::EncodeU64LE(event.GetUnixTimestamp(), ret);
      const auto message = event.GetEncodedMessage();
      ret.insert(std::end(ret), std::begin(message), std::end(message));
    }

    return ret;
  }

  std::vector<Event> FromBuffer(std::uint16_t buffer[], size_t size) {
    std::vector<Event> events;
    return events;
  }
}
