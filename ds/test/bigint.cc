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
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ds/bigint.h"

#include <stdexcept>

#define check_bigint(V1, V2, Expected)          \
  do {                                          \
    const psy::ds::BigInt b1(V1), b2(V2);       \
    const psy::ds::BigInt b3 = b1 + b2;         \
    const auto expect = Expected;               \
    EXPECT_EQ(b3.ToString(), expect);           \
  } while(0);


TEST(bigint, bad_init) {
  EXPECT_THROW({const psy::ds::BigInt bi("12a");}, std::domain_error);
  EXPECT_THROW({const psy::ds::BigInt bi("a12");}, std::domain_error);
  EXPECT_THROW({const psy::ds::BigInt bi("1a2");}, std::domain_error);
}

TEST(bigint, init) {
  const psy::ds::BigInt bi("123");
  const auto actual = bi.GetDigits();
  const auto expect = std::deque<std::uint8_t>{1, 2, 3};
  EXPECT_THAT(actual, ::testing::ContainerEq(expect));
}

TEST(bigint, add) {
  check_bigint("123", "321", "444");
  check_bigint("1111111111111111111111111111111111111111111111111",
               "1111111111111111111111111111111111111111111111111",
               "2222222222222222222222222222222222222222222222222");
}

TEST(bigint, carry) {
  check_bigint("190", "10", "200");

  check_bigint("1599", "1", "1600");

  check_bigint("9", "1", "10");
  check_bigint("99", "1", "100");
  check_bigint("999", "1", "1000");
  check_bigint("9999", "1", "10000");

  check_bigint("9999999999999999999999999999999999999999999999999999",
               "1",
               "10000000000000000000000000000000000000000000000000000");
}
