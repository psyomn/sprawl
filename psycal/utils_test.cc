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

#include "psycal/utils.h"

#define mkts(TIMESTAMP) psy::psycal::Utils::ProcessTimestamp(TIMESTAMP)

TEST(psycal, utils_process_verbose_timestamp) {
  EXPECT_TRUE(mkts("2021-1-1T11h15").has_value());
  EXPECT_TRUE(mkts("2022-1-1T11h15").has_value());
  EXPECT_TRUE(mkts("2022-1-1T14h15").has_value());
  EXPECT_TRUE(mkts("2022-1-1T12h15").has_value());
  EXPECT_TRUE(mkts("2022-1-1T12h59").has_value());

  EXPECT_FALSE(mkts("nope").has_value());
  EXPECT_FALSE(mkts("2022-50-50T11h20").has_value());
}

TEST(psycal, utils_process_short_timestamp) {
  std::optional<std::tm>
    a = mkts("11h15"),
    b = mkts("9h15"),
    c = mkts("21h15"),
    d = mkts("23h33"),
    e = mkts("3h33"),
    f = mkts("4h30");

  struct tc {
    std::optional<std::tm> tm;
    int expected_hour;
    int expected_min;
  };

  const std::vector<tc> tcs = {
    {a, 11, 15},
    {b, 9, 15},
    {c, 21, 15},
    {d, 23, 33},
    {e, 3, 33},
    {f, 4, 30},
  };

  for (const auto& el : tcs) ASSERT_TRUE(el.tm.has_value());

  const time_t nowish = time(nullptr);
  std::tm* now = localtime(&nowish);

  for (const auto& el : tcs) {
    EXPECT_EQ(el.tm.value().tm_year, now->tm_year);
    EXPECT_EQ(el.tm.value().tm_mon, now->tm_mon);
    EXPECT_EQ(el.tm.value().tm_mday, now->tm_mday);

    EXPECT_EQ(el.tm.value().tm_hour, el.expected_hour);
    EXPECT_EQ(el.tm.value().tm_min, el.expected_min);
  }
}
