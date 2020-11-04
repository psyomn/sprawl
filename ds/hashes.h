#ifndef CACOPHAGY_DS_HASHES_H_
#define CACOPHAGY_DS_HASHES_H_

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
}

#endif /* CACOPHAGY_DS_HASHES_H_ */
