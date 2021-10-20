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
#ifndef SPRAWL_DS_MD5_H_
#define SPRAWL_DS_MD5_H_

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace psy::ds::MD5 {
  std::array<std::uint32_t, 4> Digest(const std::vector<std::uint8_t>& message);
  std::string DigestToString(const std::array<std::uint32_t, 4> digest);
  std::vector<std::uint8_t> PadMessage(const std::vector<std::uint8_t>& message);
  void ExtractChunk(const std::vector<std::uint8_t>& v,
                    size_t from, size_t to, std::array<std::uint32_t, 16>& chunk);
}

#endif
