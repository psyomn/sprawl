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
#include "hexdump.h"

#include <iostream>
#include <iomanip>

namespace psy::common::Hexdump {
  void VecU8(const std::vector<std::uint8_t>& vec) {
    size_t count_row = 0;
    size_t count_col = 0;

    for (auto it = vec.cbegin(); it != vec.end(); ++it) {
      std::cout << std::hex
                << std::setw(2)
                << std::setfill('0')
                << int(*it) << ' ';

      ++count_row;
      ++count_col;
      if (count_col >= 8) { count_col = 0; std::cout << ' '; }
      if (count_row >= 16) { count_row = 0; std::cout << std::endl; }
    }
  }
}
