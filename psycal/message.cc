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
#include <stdexcept>

#include "message.h"

#include "common/codec.h"

namespace psy::psycal::Message {
  // for now we're dealing with one event per udp packet. ability to
  // pack more in the future but we don't really care about that for
  // now)
  std::vector<std::uint8_t> IntoBuffer(const std::vector<Event>& events) {
    std::vector<std::uint8_t> ret;

    for (const auto& event : events) {
      psy::common::Codec::EncodeU64LE(event.GetUnixTimestamp(), ret);
      const auto message = event.GetEncodedMessage();
      ret.insert(std::end(ret), std::begin(message), std::end(message));
    }

    return ret;
  }

  std::vector<Event> FromBuffer(const std::uint8_t buffer[], const size_t size) {
    namespace pcc = psy::common::Codec;

    std::uint64_t timestamp_u64 = 0;
    if (pcc::DecodeU64LE(timestamp_u64, &buffer[0]) != pcc::Result::Ok)
      throw std::domain_error("error decoding timestamp");

    std::uint64_t message_size_u64 = 0;
    if (pcc::DecodeU64LE(message_size_u64, &buffer[0] + 8) != pcc::Result::Ok)
      throw std::domain_error("error decoding message size");

    std::string message;
    std::copy(&buffer[0] + 16, &buffer[0] + 16 + message_size_u64,
              std::back_inserter(message));

    const time_t timestamp = time_t(timestamp_u64);
    std::tm result = {0};
    (void) localtime_r(&timestamp, &result);

    return std::vector<Event>{Event(std::move(result), std::move(message))};
  }
}
