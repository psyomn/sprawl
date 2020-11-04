#include "hashes.h"

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
}
