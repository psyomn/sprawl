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
#ifndef CACOPHAGY_COMMON_CODEC_H_
#define CACOPHAGY_COMMON_CODEC_H_
#include <cstdint>
#include <vector>
namespace psy::common::Codec {
  enum class Result { Ok, Error };
  Result EncodeU64LE(std::uint64_t x, std::vector<std::uint8_t>& vec);
}
#endif
