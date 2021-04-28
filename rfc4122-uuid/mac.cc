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
#include "mac.h"

namespace psy::uuid {
  std::uint64_t MacStringToInt(const std::string& mac_string)  {
    const std::string delim = ":";
    std::uint64_t ret = 0;

    auto start = 0U;
    auto end =  mac_string.find(delim);
    while (end != std::string::npos) {
      const std::string octet = mac_string.substr(start, end - start);
      ret = (ret << 8) | std::stoul(octet, 0, 16);

      start = end + delim.length();
      end = mac_string.find(delim, start);
    }

    if (start < mac_string.size() - 1) {
      const std::string octet = mac_string.substr(start, mac_string.size() - 1);
      ret = (ret << 8) | std::stoul(octet, 0, 16);
    }

    return ret;
  }
}
