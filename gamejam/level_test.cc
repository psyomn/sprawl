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

#include "level.h"

#include <iostream>
#include <sstream>

TEST(gamejam, bad_metadata_format) {
  /* the preamble is wrong, so it should return nothing when trying to build */
  std::stringstream ss[] = {
    std::stringstream{"x:"},
    std::stringstream{"y:"},

    std::stringstream{"x:12\ny:\n"},
    std::stringstream{"y:12\nx:\n"},

    std::stringstream{"t:\nx:12"},
    std::stringstream{"t:\ny:12"},
  };

  namespace pg = psy::gamejam;
  pg::LevelBuilder builder;

  for (auto& el : ss) {
    auto result = builder.FromText(el);
    EXPECT_EQ(result, std::nullopt);
  }
}

TEST(gamejam, bad_leveldata_format) {
  /* TODO */
}

TEST(gamejam, parse_metadata) {
  std::stringstream ss(
    "x:11\n"
    "y:22\n"
    "t:/path/to/tilemap.png\n"
    "\n"
  );

  namespace pg = psy::gamejam;

  auto builder = pg::LevelBuilder();
  auto result = builder.ParseMetadata(ss);

  ASSERT_EQ(pg::LevelBuilder::ParseStatus::kSuccess, result);

  EXPECT_EQ(builder.width_, 11);
  EXPECT_EQ(builder.height_, 22);
  EXPECT_EQ(builder.path_to_tileset_, "/path/to/tilemap.png");
}

TEST(gamejam, level_from_text) {
  std::stringstream ss(
    "x:26\n"
    "y:7\n"
    "t:/path/to/tilemap.png\n"
    "\n"
    "                          \n"
    "                          \n"
    "               ###        \n"
    "                          \n"
    "   ####      #######      \n"
    "                          \n"
    "##########################\n"
  );

  namespace pg = psy::gamejam;
  auto builder = pg::LevelBuilder();

  auto result = builder.FromText(ss);

  ASSERT_TRUE(result.has_value());
  auto val = result.value();

  EXPECT_EQ(val.Width(), 26);
  EXPECT_EQ(val.Height(), 7);
  EXPECT_EQ(val.TilesetPath(), "/path/to/tilemap.png");
}
