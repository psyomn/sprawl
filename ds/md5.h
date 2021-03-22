#ifndef CACOPHAGY_DS_MD5_H_
#define CACOPHAGY_DS_MD5_H_

#include <array>
#include <cstdint>
#include <vector>

namespace psy::ds {
  class MD5 {
  public:
    static std::array<std::uint32_t, 4> Digest(const std::vector<std::uint8_t>& message);
    static std::vector<std::uint8_t> PadMessage(const std::vector<std::uint8_t>& message);
  };
}

#endif
