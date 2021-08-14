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
#include "utils.h"

#include <cstdint>
#include <ctime>
#include <functional>

namespace psy::psycal /* ::Dump // TODO */ {
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

  void SaveOld(const std::vector<Event>& olds) {
    std::ofstream of(
        Utils::HistoryFilePath(),
        std::ofstream::binary | std::ofstream::app | std::ofstream::out);
    of.imbue(std::locale::classic());

    for (const auto& el : olds) {
      const std::uint64_t timestamp = el.GetUnixTimestamp();
      of.write(reinterpret_cast<const char*>(&timestamp), sizeof(timestamp));

      const std::uint64_t message_size = el.GetMessage().size();
      of.write(reinterpret_cast<const char*>(&message_size), sizeof(message_size));

      of.write(reinterpret_cast<const char*>(el.GetMessage().data()),
               static_cast<std::streamsize>(el.GetMessage().size()));
    }
    of.close();
  }

  void SaveCurrentEvents(const std::priority_queue<Event,
                         std::vector<Event>,
                         Event::Comparator>& events) {
    std::ofstream of(
        Utils::EventsFilePath(),
        std::ios::binary|std::ios::out);
    of.imbue(std::locale::classic());

    /* priority queue doesn't allow for iteration, so I can't quite
       dump this directly onto disk. */
    std::priority_queue<
      Event,
      std::vector<Event>,
      Event::Comparator> copy = events;

    while (copy.size() > 0) {
      auto element = copy.top();
      const std::uint64_t timestamp = element.GetUnixTimestamp();
      of.write(reinterpret_cast<const char*>(&timestamp), sizeof(timestamp));

      const size_t message_size = element.GetMessage().size();
      of.write(reinterpret_cast<const char*>(&message_size), sizeof(message_size));
      of.write(element.GetMessage().c_str(),
               static_cast<std::streamsize>(element.GetMessage().size()));

      copy.pop();
    }
  }
}
