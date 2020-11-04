#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ds/hashes.h"

#include <vector>

std::vector<std::byte> sample_bytes_1(void);
std::vector<std::byte> sample_bytes_2(void);

std::vector<std::byte> sample_bytes_1(void) {
  return std::vector<std::byte>{
    std::byte(0x01),
    std::byte(0x02),
    std::byte(0x03),
    std::byte(0x04),
    std::byte(0x05),
  };
}

std::vector<std::byte> sample_bytes_2(void) {
  return std::vector<std::byte>{
    std::byte(0x01),
    std::byte(0x10),
    std::byte(0xaa),
    std::byte(0xbb),
    std::byte(0xcc),
    std::byte(0xff),
  };
}

TEST(hashes, djb2) {
  const auto bytes_1 = sample_bytes_1();
  const auto result_1 = psy::ds::djb2(bytes_1);
  EXPECT_NE(result_1, 0);
  EXPECT_EQ(result_1, 210588810932);

  const auto bytes_2 = sample_bytes_2();
  const auto result_2 = psy::ds::djb2(bytes_2);
  EXPECT_NE(result_2, result_1);
  EXPECT_EQ(result_2, 6949453571238);
}

TEST(hashes, sdbm) {
  const auto bytes_1 = sample_bytes_1();
  const auto result_1 = psy::ds::sdbm(bytes_1);
  const auto bytes_2 = sample_bytes_2();
  const auto result_2 = psy::ds::sdbm(bytes_2);
  EXPECT_NE(result_1, 0);
  EXPECT_EQ(result_1, 71598627968528387U);
  EXPECT_EQ(result_2, 12367020746941822611U);
  EXPECT_NE(result_1, result_2);
}

TEST(hashes, loselose) {
  const auto bytes_1 = sample_bytes_1();
  const auto result_1 = psy::ds::loselose(bytes_1);
  EXPECT_NE(result_1, 0);
  EXPECT_EQ(result_1, 15);
}
