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
#include <cstdint>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ds/md5.h"

using namespace psy::ds;

TEST(md5, basic_pad) {
  const std::vector<uint8_t> original = {0, 1, 2, 3};
  const auto padded_message = MD5::PadMessage(original);
  EXPECT_EQ(padded_message.size(), 64);

  EXPECT_EQ(original[0], padded_message[0]);
  EXPECT_EQ(original[3], padded_message[3]);

  EXPECT_EQ(padded_message[4], 0b10000000);
  EXPECT_EQ(padded_message[5], 0);

  std::uint64_t msg_size = 0;
  auto it = padded_message.cend() - 8;

  for (size_t i = 0; i < 8; ++i) msg_size |= *it++;

  EXPECT_EQ(msg_size, 4 * 8);
}

TEST(md5, basic_string_message_pad) {
  const std::vector<uint8_t> original = {
    'h', 'e', 'l', 'l', 'o', ' ',
    't', 'h', 'e', 'r', 'e',
  };
  const auto padded_message = MD5::PadMessage(original);
  EXPECT_EQ(padded_message.size(), 64);

  EXPECT_EQ(original[0], padded_message[0]);
  EXPECT_EQ(original[3], padded_message[3]);

  EXPECT_EQ(padded_message[11], 0b10000000);
  EXPECT_EQ(padded_message[12], 0);

  std::uint64_t msg_size = 0;
  auto it = padded_message.cend() - 8;

  for (size_t i = 0; i < 8; ++i) msg_size |= *it++;

  EXPECT_EQ(msg_size, 11 * 8);
}

TEST(md5, pad_n_plus_1) {
  std::vector<uint8_t> original;

  for (size_t i = 0; i < 65; ++i) original.push_back(0xa);
  const auto padded_message = MD5::PadMessage(original);

  EXPECT_EQ(padded_message.size(), 128);
  EXPECT_EQ(padded_message[65], 0b10000000);
}

TEST(md5, empty_message) {
  const std::vector<std::uint8_t> message = {};
  const auto raw_digest = MD5::Digest(message);
  const std::string digest = MD5::DigestToString(raw_digest);
  EXPECT_EQ(digest, std::string("d41d8cd98f00b204e9800998ecf8427e"));
}

TEST(md5, extract_chunk) {
  const std::vector<std::uint8_t> data = {
    0, 1, 2, 3,
    1, 2, 3, 4,
    2, 2, 2, 2,
    3, 3, 3, 3,
    4, 4, 4, 4,
    5, 5, 5, 5,
    6, 6, 6, 6,
    7, 7, 7, 7,
    8, 8, 8, 8,
    9, 9, 9, 9,
    10, 10, 10, 10,
    11, 11, 11, 11,
    12, 12, 12, 12,
    13, 13, 13, 13,
    14, 14, 14, 14,
    15, 15, 15, 15,
  };

  std::array<std::uint32_t, 16> chunks = {0};
  MD5::ExtractChunk(data, 0, 64, chunks);

  EXPECT_EQ(chunks[0], 0x03020100);
}

#define check_digest(inputstr, expectedstr)                             \
  do {                                                                  \
    const std::string input = inputstr;                                 \
    std::vector<std::uint8_t> message(input.cbegin(), input.cend());    \
    const std::string actual = MD5::DigestToString(MD5::Digest(message)); \
    EXPECT_EQ(actual, expectedstr);                                     \
  } while (false)

TEST(md5, rfc_test_cases) {
  check_digest("", "d41d8cd98f00b204e9800998ecf8427e");
  check_digest("a", "0cc175b9c0f1b6a831c399e269772661");
  check_digest("abc", "900150983cd24fb0d6963f7d28e17f72");
  check_digest("message digest", "f96b697d7cb7938d525a2f31aaf161d0");
  check_digest("abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b");
  check_digest("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
               "d174ab98d277d9f5a5611c2c9f419d9f");
  check_digest("12345678901234567890123456789012345678901234567890123456789012345678901234567890",
               "57edf4a22be3c955ac49da2e2107b67a");
}


TEST(md5, test_other_input) {
  check_digest("hello there how are you doing",
               "66ecf4396778b35ee7eac3eaccfd214c");

  check_digest("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
               "43387810db1f81c6ccc747be89af4b72");
}

TEST(md5, check_larger_input) {
  // basically: $ yes | head -1000 > b
  std::vector<std::uint8_t> message;
  message.resize(2000);
  for (size_t i = 0; i < 2000; i+=2) { message[i] = 'y'; message[i+1] = 0xa; }

  const auto raw_digest = MD5::Digest(message);
  const std::string digest = MD5::DigestToString(raw_digest);
  EXPECT_EQ(digest, std::string("52cedf1b90dbddf860fcdaabed76aee4"));
}

TEST(md5, check_even_larger_input) {
  // basically: $ yes | head -20000 > b
  std::vector<std::uint8_t> message;
  message.resize(40000);
  for (size_t i = 0; i < 40000; i+=2) { message[i] = 'y'; message[i+1] = 0xa; }

  const auto raw_digest = MD5::Digest(message);
  const std::string digest = MD5::DigestToString(raw_digest);
  EXPECT_EQ(digest, std::string("c694eebe75d844b1aa8055479d859963"));
}
