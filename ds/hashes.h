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
#ifndef SPRAWL_DS_HASHES_H_
#define SPRAWL_DS_HASHES_H_

#include <cstdint>
#include <vector>

namespace psy::ds {
  // I got djb2, sdbm, lose-lose from the below link:
  //   http://www.cse.yorku.ca/~oz/hash.html
  // nb: I'm not sure if these are OK for 64 bit but they should.
  // note: might be worth inline. but might also not.
  std::uint64_t djb2(const std::vector<std::byte>& bytes) noexcept;
  std::uint64_t sdbm(const std::vector<std::byte>& bytes) noexcept;
  std::uint64_t loselose(const std::vector<std::byte>& bytes) noexcept;
  std::uint16_t fletcher16(const std::vector<std::byte>& bytes) noexcept;
  std::uint32_t fletcher32(const std::vector<std::byte>& bytes) noexcept;
  std::uint64_t fletcher64(const std::vector<std::byte>& bytes) noexcept;
}

#endif /* SPRAWL_DS_HASHES_H_ */
