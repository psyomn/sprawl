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
#include "hashes.h"
#include <iostream>

namespace psy::ds {
  std::uint64_t djb2(const std::vector<std::byte>& bytes) noexcept {
    std::uint64_t hash = 5381;
    for (const auto& byte : bytes)
      hash = ((hash << 5) + hash) + std::uint64_t(byte);
    return hash;
  }

  std::uint64_t sdbm(const std::vector<std::byte>& bytes) noexcept {
    std::uint64_t hash = 0;
    for (const auto& byte : bytes)
      hash = std::uint64_t(byte) + (hash << 6) + (hash << 16) - hash;
    return hash;
  }

  std::uint64_t loselose(const std::vector<std::byte>& bytes) noexcept {
    std::uint64_t hash = 0;
    for (const auto& byte : bytes) hash += std::uint64_t(byte);
    return hash;
  }

  std::uint16_t fletcher16(const std::vector<std::byte>& bytes) noexcept {
    auto c0 = std::uint8_t(0);
    auto c1 = std::uint8_t(0);

    for (const auto& byte : bytes) {
      c0 = (c0 + std::uint8_t(byte));
      c1 = (c1 + c0);
    }

    return c0 | (c1 << 8);
  }

  std::uint32_t fletcher32(const std::vector<std::byte>& bytes) noexcept {
    auto c0 = std::uint16_t(0);
    auto c1 = std::uint16_t(0);

    for (const auto& byte : bytes) {
      c0 = (c0 + std::uint16_t(byte));
      c1 = (c1 + c0);
    }

    return c0 | std::uint32_t(c1 << 16);
  }
}
