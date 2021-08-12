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
#include "dump.h"

#include <cstdint>
#include <ctime>

namespace psy::psycal {
  std::vector<Event> ReadEventsFrom(std::istream& is) {
    std::vector<Event> ret;

    while (!is.eof() && is.peek() != EOF) {
      std::uint64_t timestamp_u64 = 0;
      is.read(reinterpret_cast<char*>(&timestamp_u64), sizeof(timestamp_u64));

      size_t message_size = 0;
      is.read(reinterpret_cast<char*>(&message_size), sizeof(message_size));

      char *buffer = new char[message_size];
      is.read(reinterpret_cast<char*>(buffer),
              static_cast<std::streamsize>(message_size));

      const time_t timestamp = time_t(timestamp_u64);
      std::tm result = {0};
      (void) localtime_r(&timestamp, &result);

      ret.push_back(
          Event(std::move(result), std::move(std::string(buffer)))
      );
      delete[] buffer;
    }

    return ret;
  }
}
