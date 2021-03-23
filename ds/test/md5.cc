#include <cstdint>

#include <iostream> // todo remove me

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ds/md5.h"

TEST(md5, basic_pad) {
  using md5 = psy::ds::MD5;
  const std::vector<uint8_t> original = {0, 1, 2, 3};
  const auto padded_message = md5::PadMessage(original);
  EXPECT_EQ(padded_message.size(), 64);

  EXPECT_EQ(original[0], padded_message[0]);
  EXPECT_EQ(original[3], padded_message[3]);

  EXPECT_EQ(padded_message[4], 0b10000000);
  EXPECT_EQ(padded_message[5], 0);

  std::uint64_t msg_size = 0;
  auto it = padded_message.cend() - 8;

  for (size_t i = 0; i < 8; ++i) msg_size |= *it++;
  EXPECT_EQ(msg_size, 4);
}

TEST(md5, pad_n_plus_1) {
  using md5 = psy::ds::MD5;
  std::vector<uint8_t> original;

  for (size_t i = 0; i < 65; ++i) original.push_back(0xa);
  const auto padded_message = md5::PadMessage(original);

  EXPECT_EQ(padded_message.size(), 128);
  EXPECT_EQ(padded_message[65], 0b10000000);
}

TEST(md5, simple_message) {
  using md5 = psy::ds::MD5;
  const std::vector<std::uint8_t> message = { 'h', 'e', 'l', 'l', 'o' };
  const auto ret = md5::Digest(message);

  std::cout << std::hex
            << ret[0] << " "
            << ret[1] << " "
            << ret[2] << " "
            << ret[3] << " " << std::endl;
}
