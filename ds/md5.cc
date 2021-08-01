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
#include "md5.h"

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <sstream>

namespace psy::ds::MD5 {
  typedef std::uint32_t u32;

  constexpr size_t kSineSize = 64;
  const u32 kSine[kSineSize] = { /* T[N] in rfc1321 */
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
  };

  const u32 kA0 = 0x67452301;
  const u32 kB0 = 0xefcdab89;
  const u32 kC0 = 0x98badcfe;
  const u32 kD0 = 0x10325476;

  constexpr u32 LeftRotate(u32 value, u32 shift_size) {
    return (value << shift_size) | (value >> (32 - shift_size));
  }

  constexpr u32 kS11 = 7;
  constexpr u32 kS12 = 12;
  constexpr u32 kS13 = 17;
  constexpr u32 kS14 = 22;
  constexpr u32 kS21 = 5;
  constexpr u32 kS22 = 9;
  constexpr u32 kS23 = 14;
  constexpr u32 kS24 = 20;
  constexpr u32 kS31 = 4;
  constexpr u32 kS32 = 11;
  constexpr u32 kS33 = 16;
  constexpr u32 kS34 = 23;
  constexpr u32 kS41 = 6;
  constexpr u32 kS42 = 10;
  constexpr u32 kS43 = 15;
  constexpr u32 kS44 = 21;

  constexpr u32 F(u32 x, u32 y, u32 z) { return (x & y) | ((~x) & z); }
  constexpr u32 G(u32 x, u32 y, u32 z) { return (x & z) | (y & (~z)); }
  constexpr u32 H(u32 x, u32 y, u32 z) { return x ^ y ^ z; }
  constexpr u32 I(u32 x, u32 y, u32 z) { return y ^ (x | (~z)); }

  constexpr void FF(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac) {
    a += F(b, c, d) + x + ac;
    a = LeftRotate(a, s);
    a += b;
  }

  constexpr void GG(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac) {
    a += G(b, c, d) + x + ac;
    a = LeftRotate(a, s);
    a += b;
  }

  constexpr void HH(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac) {
    a += H(b, c, d) + x + ac;
    a = LeftRotate(a, s);
    a += b;
  }

  constexpr void II(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac) {
    a += I(b, c, d) + x + ac;
    a = LeftRotate(a, s);
    a += b;
  }

  std::array<u32, 4> Digest(const std::vector<std::uint8_t>& message) {
    const auto padded_message = PadMessage(message); /* M[...] in rfc */
    const size_t padded_size = padded_message.size();

    u32 A0 = kA0, B0 = kB0, C0 = kC0, D0 = kD0;

    u32 a = 0, b = 0, c = 0, d = 0;
    std::array<uint32_t, 16> x = {0};

    for (u32 i = 0; i < padded_size; i += 64) {
      a = A0;
      b = B0;
      c = C0;
      d = D0;

      ExtractChunk(padded_message, i, i+64, x);

      // as seen in the RFC
      FF(a, b, c, d, x[ 0], kS11, 0xd76aa478);
      FF(d, a, b, c, x[ 1], kS12, 0xe8c7b756);
      FF(c, d, a, b, x[ 2], kS13, 0x242070db);
      FF(b, c, d, a, x[ 3], kS14, 0xc1bdceee);
      FF(a, b, c, d, x[ 4], kS11, 0xf57c0faf);
      FF(d, a, b, c, x[ 5], kS12, 0x4787c62a);
      FF(c, d, a, b, x[ 6], kS13, 0xa8304613);
      FF(b, c, d, a, x[ 7], kS14, 0xfd469501);
      FF(a, b, c, d, x[ 8], kS11, 0x698098d8);
      FF(d, a, b, c, x[ 9], kS12, 0x8b44f7af);
      FF(c, d, a, b, x[10], kS13, 0xffff5bb1);
      FF(b, c, d, a, x[11], kS14, 0x895cd7be);
      FF(a, b, c, d, x[12], kS11, 0x6b901122);
      FF(d, a, b, c, x[13], kS12, 0xfd987193);
      FF(c, d, a, b, x[14], kS13, 0xa679438e);
      FF(b, c, d, a, x[15], kS14, 0x49b40821);

      GG(a, b, c, d, x[ 1], kS21, 0xf61e2562);
      GG(d, a, b, c, x[ 6], kS22, 0xc040b340);
      GG(c, d, a, b, x[11], kS23, 0x265e5a51);
      GG(b, c, d, a, x[ 0], kS24, 0xe9b6c7aa);
      GG(a, b, c, d, x[ 5], kS21, 0xd62f105d);
      GG(d, a, b, c, x[10], kS22,  0x2441453);
      GG(c, d, a, b, x[15], kS23, 0xd8a1e681);
      GG(b, c, d, a, x[ 4], kS24, 0xe7d3fbc8);
      GG(a, b, c, d, x[ 9], kS21, 0x21e1cde6);
      GG(d, a, b, c, x[14], kS22, 0xc33707d6);
      GG(c, d, a, b, x[ 3], kS23, 0xf4d50d87);
      GG(b, c, d, a, x[ 8], kS24, 0x455a14ed);
      GG(a, b, c, d, x[13], kS21, 0xa9e3e905);
      GG(d, a, b, c, x[ 2], kS22, 0xfcefa3f8);
      GG(c, d, a, b, x[ 7], kS23, 0x676f02d9);
      GG(b, c, d, a, x[12], kS24, 0x8d2a4c8a);

      HH(a, b, c, d, x[ 5], kS31, 0xfffa3942);
      HH(d, a, b, c, x[ 8], kS32, 0x8771f681);
      HH(c, d, a, b, x[11], kS33, 0x6d9d6122);
      HH(b, c, d, a, x[14], kS34, 0xfde5380c);
      HH(a, b, c, d, x[ 1], kS31, 0xa4beea44);
      HH(d, a, b, c, x[ 4], kS32, 0x4bdecfa9);
      HH(c, d, a, b, x[ 7], kS33, 0xf6bb4b60);
      HH(b, c, d, a, x[10], kS34, 0xbebfbc70);
      HH(a, b, c, d, x[13], kS31, 0x289b7ec6);
      HH(d, a, b, c, x[ 0], kS32, 0xeaa127fa);
      HH(c, d, a, b, x[ 3], kS33, 0xd4ef3085);
      HH(b, c, d, a, x[ 6], kS34,  0x4881d05);
      HH(a, b, c, d, x[ 9], kS31, 0xd9d4d039);
      HH(d, a, b, c, x[12], kS32, 0xe6db99e5);
      HH(c, d, a, b, x[15], kS33, 0x1fa27cf8);
      HH(b, c, d, a, x[ 2], kS34, 0xc4ac5665);

      II(a, b, c, d, x[ 0], kS41, 0xf4292244);
      II(d, a, b, c, x[ 7], kS42, 0x432aff97);
      II(c, d, a, b, x[14], kS43, 0xab9423a7);
      II(b, c, d, a, x[ 5], kS44, 0xfc93a039);
      II(a, b, c, d, x[12], kS41, 0x655b59c3);
      II(d, a, b, c, x[ 3], kS42, 0x8f0ccc92);
      II(c, d, a, b, x[10], kS43, 0xffeff47d);
      II(b, c, d, a, x[ 1], kS44, 0x85845dd1);
      II(a, b, c, d, x[ 8], kS41, 0x6fa87e4f);
      II(d, a, b, c, x[15], kS42, 0xfe2ce6e0);
      II(c, d, a, b, x[ 6], kS43, 0xa3014314);
      II(b, c, d, a, x[13], kS44, 0x4e0811a1);
      II(a, b, c, d, x[ 4], kS41, 0xf7537e82);
      II(d, a, b, c, x[11], kS42, 0xbd3af235);
      II(c, d, a, b, x[ 2], kS43, 0x2ad7d2bb);
      II(b, c, d, a, x[ 9], kS44, 0xeb86d391);

      A0 += a;
      B0 += b;
      C0 += c;
      D0 += d;
    }

    return std::array<u32, 4>{A0, B0, C0, D0};
  }

  std::string DigestToString(const std::array<u32, 4> digest) {
    char output[32 + 1] = {0};

    std::sprintf(output,
                 "%02x%02x%02x%02x"
                 "%02x%02x%02x%02x"
                 "%02x%02x%02x%02x"
                 "%02x%02x%02x%02x",

                 ((digest[0] & 0x000000ff))       ,
                 ((digest[0] & 0x0000ff00) >> 8)  ,
                 ((digest[0] & 0x00ff0000) >> 16) ,
                 ((digest[0] & 0xff000000) >> 24) ,

                 ((digest[1] & 0x000000ff))       ,
                 ((digest[1] & 0x0000ff00) >> 8)  ,
                 ((digest[1] & 0x00ff0000) >> 16) ,
                 ((digest[1] & 0xff000000) >> 24) ,

                 ((digest[2] & 0x000000ff))       ,
                 ((digest[2] & 0x0000ff00) >> 8)  ,
                 ((digest[2] & 0x00ff0000) >> 16) ,
                 ((digest[2] & 0xff000000) >> 24) ,

                 ((digest[3] & 0x000000ff))       ,
                 ((digest[3] & 0x0000ff00) >> 8)  ,
                 ((digest[3] & 0x00ff0000) >> 16) ,
                 ((digest[3] & 0xff000000) >> 24));

    return std::string(output);
  }

  std::vector<std::uint8_t> PadMessage(const std::vector<std::uint8_t>& message) {
    const size_t msg_size = message.size();
    const size_t num_blocks = ((msg_size + 8) >> 6) + 1;
    const size_t total_len = (num_blocks << 6) - msg_size;
    const size_t surplus_bytes = total_len - 8 - 1;

    std::vector<std::uint8_t> padded(message);
    padded.push_back(0x80);

    for (size_t i = 0; i < surplus_bytes; ++i) padded.push_back(0);

    const std::uint64_t msg_size_64 = std::uint64_t(msg_size) * 8;
    const u32 hi = (0xffffffff00000000 & msg_size_64) >> 32;
    const u32 lo = (0x00000000ffffffff & msg_size_64);

    padded.push_back((0xff & lo));
    padded.push_back((0xff00 & lo) >> 8);
    padded.push_back((0xff0000 & lo) >> 16);
    padded.push_back((0xff000000 & lo) >> 24);

    padded.push_back(0xff & (hi));
    padded.push_back(0xff & (hi >> 8));
    padded.push_back(0xff & (hi >> 16));
    padded.push_back(0xff & (hi >> 24));

    return padded;
  }

  void ExtractChunk(const std::vector<std::uint8_t>& message,
                    const size_t from,
                    const size_t to,
                    std::array<std::uint32_t, 16>& chunk) {
    // TODO: reuse common endian encoder here
    size_t count = 0;
    for (size_t i = from; i < to; i += 4, count++) {
      chunk[count] =
        message[i+0]
        | std::uint32_t(message[i+1]) << 8
        | std::uint32_t(message[i+2]) << 16
        | std::uint32_t(message[i+3]) << 24;
    }
  }
}
