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
#include "codec.h"
namespace psy::common::Codec {
  Result EncodeU64LE(const std::uint64_t x, std::vector<std::uint8_t>& vec) {
    vec.push_back((x & 0xff));
    vec.push_back((x & 0xff00) >> 8);
    vec.push_back((x & 0xff0000) >> 16);
    vec.push_back((x & 0xff000000) >> 24);

    vec.push_back((x & 0xff00000000) >> 32);
    vec.push_back((x & 0xff0000000000) >> 40);
    vec.push_back((x & 0xff000000000000) >> 48);
    vec.push_back((x & 0xff00000000000000) >> 56);
    return Result::Ok;
  }

  Result DecodeU64LE(std::uint64_t& x, const std::uint8_t* cursor) {
    x =
      std::uint64_t(*cursor)             |
      std::uint64_t(*(cursor + 1)) << 8  |
      std::uint64_t(*(cursor + 2)) << 16 |
      std::uint64_t(*(cursor + 3)) << 24 |
      std::uint64_t(*(cursor + 4)) << 32 |
      std::uint64_t(*(cursor + 5)) << 40 |
      std::uint64_t(*(cursor + 6)) << 48 |
      std::uint64_t(*(cursor + 7)) << 56 ;
    return Result::Ok;
  }
}