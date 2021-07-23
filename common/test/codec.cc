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

#include "common/codec.h"

TEST(psycommon, codecode) {
  const std::uint64_t value = 0xaabbccddeeff0011;
  std::vector<std::uint8_t> actual;

  EXPECT_EQ(psy::common::Codec::EncodeU64LE(value, actual),
            psy::common::Codec::Result::Ok);

  const auto expected = std::vector<std::uint8_t>{
    0x11, 0x00, 0xff, 0xee,
    0xdd, 0xcc, 0xbb, 0xaa,
  };

  EXPECT_THAT(expected, ::testing::ContainerEq(actual));

  std::uint64_t decoded = 0;
  EXPECT_EQ(psy::common::Codec::DecodeU64LE(decoded, actual.data()),
            psy::common::Codec::Result::Ok);
  EXPECT_EQ(value, decoded);
}
