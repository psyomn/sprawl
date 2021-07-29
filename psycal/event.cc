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
#include <iterator>
#include <sstream>

#include <algorithm>

#include "event.h"

#include "common/codec.h"

namespace psy::psycal {
  Event::Event(std::tm&& tm, std::string&& message) :
    tm_(std::move(tm)),
    message_(std::move(message)) {}

  std::uint64_t Event::GetUnixTimestamp() const {
    namespace sc = std::chrono;
    std::tm tm_2 = tm_;
    const time_t event_time = mktime(&tm_2);
    return static_cast<std::uint64_t>(event_time);
  }

  std::vector<std::uint8_t> Event::GetEncodedMessage() const {
    std::vector<std::uint8_t> ret;

    const std::size_t size = message_.size();
    psy::common::Codec::EncodeU64LE(std::uint64_t(size), ret);

    std::copy(std::cbegin(message_),
              std::cend(message_),
              std::back_inserter(ret));

    return ret;
  }
}
