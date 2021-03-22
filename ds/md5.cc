#include "md5.h"

namespace psy::ds {
  // constants taken from https://en.wikipedia.org/wiki/MD5

  constexpr size_t kShiftSize = 64;
  const std::uint8_t kShift[kShiftSize] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21,
  };

  const std::uint32_t kA0 = 0x67452301;
  const std::uint32_t kB0 = 0xefcdab89;
  const std::uint32_t kC0 = 0x98badcfe;
  const std::uint32_t kD0 = 0x10325476;

  std::array<std::uint32_t, 4> MD5::Digest(const std::vector<std::uint8_t>& message) {
    auto padded_message = PadMessage(message);
    const size_t padded_size = padded_message.size();

    std::uint32_t A0 = kA0, B0 = kB0, C0 = kC0, D0 = kD0;
    std::uint64_t F = 0, G = 0;

    for (size_t i = 0; i < padded_size; i += 64) {
      std::uint32_t a = A0, b = B0, c = C0, d = D0;

      for (size_t j = 0; j < kShiftSize; ++j) {
        if (j >= 48 && j <= 63) {
          // F := C xor (B or (not D))
          // g := (7×i) mod 16
        } else if (j >= 32 && j <= 47) {
          // F := B xor C xor D
          // g := (3×i + 5) mod 16
        } else if (j >= 16 && j <= 31) {
          // F := (D and B) or ((not D) and C)
          // g := (5×i + 1) mod 16
        } else if (j >= 0  && j <= 15) {
          // F := (B and C) or ((not B) and D)
          // g := i
        }
      }

      A0 += a;
      B0 += b;
      C0 += c;
      D0 += d;
    }

    std::array<std::uint32_t, 4> ret = {0, 0, 0, 0};
    return ret;
  }

  std::vector<std::uint8_t> MD5::PadMessage(const std::vector<std::uint8_t>& message) {
    // 64 byte chunks
    const size_t msg_size = message.size();
    const size_t remainder = msg_size % 64;
    const size_t surplus_bytes = 64 - remainder - 1 - 8;

    std::vector<std::uint8_t> padded(message);
    padded.push_back(0x80);

    for (size_t i = 0; i < surplus_bytes; ++i)
      padded.push_back(0);

    const std::uint64_t msg_size_64 = std::uint64_t(msg_size);

    padded.push_back(0xff & msg_size_64);
    padded.push_back(0xff & (msg_size_64 >> 8));
    padded.push_back(0xff & (msg_size_64 >> 16));
    padded.push_back(0xff & (msg_size_64 >> 24));

    padded.push_back(0xff & (msg_size_64 >> 32));
    padded.push_back(0xff & (msg_size_64 >> 40));
    padded.push_back(0xff & (msg_size_64 >> 48));
    padded.push_back(0xff & (msg_size_64 >> 56));

    return padded;
  }
}
