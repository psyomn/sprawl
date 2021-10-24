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

#include "utils.h"

#include <iostream>
#include <sstream>

namespace pg = psy::gamejam;

TEST(gamejam, parse_int) {
  struct tc {
    std::stringstream ss;
    size_t expected;
  } tcs[] = {
    { std::stringstream(":123\n"), 123 },
    { std::stringstream(":12345910"), 12345910 },
  };


  for (auto& el : tcs) {
    auto maybe = pg::ParseInt(el.ss);
    ASSERT_TRUE(maybe.has_value());
    EXPECT_EQ(el.expected, maybe.value());
  }
}

TEST(gamejam, parse_str) {
  struct tc {
    std::stringstream ss;
    std::string expected;
  } tcs[] = {
    { std::stringstream(":a\n"), "a" },
    { std::stringstream(":a\n\n/b\n"), "a" },
  };

  for (auto& el : tcs) {
    auto maybe = pg::ParseStr(el.ss);
    ASSERT_TRUE(maybe.has_value());
    EXPECT_EQ(el.expected, maybe.value());
  }
}
