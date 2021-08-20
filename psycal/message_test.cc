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

#include "event.h"
#include "message.h"

TEST(psycal, message_individual_encode) {
  std::tm tm = {0};
  ASSERT_EQ(strptime("2021-01-01T12h30", "%Y-%m-%dT%H-%m", &tm), nullptr);

  psy::psycal::Event event(std::move(tm), std::move("hi you"));

  const std::vector<std::uint8_t> expected = {
    6, 0, 0, 0, 0, 0, 0, 0,
    'h', 'i', ' ', 'y', 'o', 'u',
  };

  EXPECT_EQ(event.GetUnixTimestamp(), 1609502400);
  EXPECT_THAT(event.GetEncodedMessage(), ::testing::ContainerEq(expected));
}

TEST(psycal, message_into_and_from_buffer) {
  std::tm tm = {0};
  ASSERT_EQ(strptime("2021-01-01T12h30", "%Y-%m-%dT%H-%m", &tm), nullptr);

  psy::psycal::Event event(std::move(tm),
                           std::move(std::string("hi you")));

  const std::vector<std::uint8_t> expected = {
    0xc0, 0x0e, 0xef, 0x5f,  0, 0, 0, 0,
    6, 0, 0, 0,  0, 0, 0, 0,
    'h', 'i', ' ', 'y', 'o', 'u',
  };

  auto actual = psy::psycal::Message::IntoBuffer({event});
  EXPECT_THAT(expected, ::testing::ContainerEq(actual));

  const auto decoded_events = psy::psycal::Message::FromBuffer(actual.data(), actual.size());
  ASSERT_EQ(decoded_events.size(), 1);

  EXPECT_THAT(decoded_events[0].GetUnixTimestamp(), 1609502400);
  EXPECT_EQ(decoded_events[0].GetMessage(), std::string("hi you"));
}
