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

#include "colshape.h"

using namespace psy::gamejam;

/*
** basic testing of collisions on simple rectangless through intersection.
**
** you will see me doing the pattern of r1.CollidesWith(r2), and
** r2.CollidesWith(r1).  This is mostly a sanity check, and in development you
** should not be doing this comparison.
*/

TEST(gamejam, colshape_rect_outside) {
  {
    const auto r1 = Colshape(0, 0, 10, 10);
    const auto r2 = Colshape(-10, -10, 5, 5);
    EXPECT_FALSE(r1.CollidesWith(r2));
    EXPECT_FALSE(r2.CollidesWith(r1));
  }

  {
    const auto r1 = Colshape(0, 0, 10, 10);
    const auto r2 = Colshape(1000, 1000, 5000, 5000);
    EXPECT_FALSE(r1.CollidesWith(r2));
    EXPECT_FALSE(r2.CollidesWith(r1));
  }
}

TEST(gamejam, colshape_rect_inside) {
  const auto r1 = Colshape(0, 0, 100, 100);
  const auto r2 = Colshape(10, 10, 90, 90);

  EXPECT_TRUE(r1.CollidesWith(r1));
  EXPECT_TRUE(r2.CollidesWith(r2));
}


TEST(gamejam, colshape_just_touching_left) {
  const auto r1 = Colshape(10, 0, 10, 10);

  /* check that a growing rectangle from the left still collides */
  for (std::int64_t i = 0; i < 10; i++) {
    const auto r2 = Colshape(9 + i, 0, 1, 1);
    EXPECT_TRUE(r1.CollidesWith(r2) && r2.CollidesWith(r1))
      << r1 << " collides with " << r2 << "?"
      << std::endl
      << "r1.col(r2): " << r1.CollidesWith(r2)
      << std::endl
      << "r2.col(r1): " << r2.CollidesWith(r1);
  }
}

TEST(gamejam, colshae_just_touching_up) {
  const auto r1 = Colshape(0, 0, 10, 10);

  EXPECT_FALSE(r1.CollidesWith(Colshape(0, -2, 1, 1)));

  for (std::int64_t i = 0; i < 10; i++) {
    const auto r2 = Colshape(0, -1 + i, 1, 1);
    EXPECT_TRUE(r1.CollidesWith(r2));
    EXPECT_TRUE(r2.CollidesWith(r1));
  }
}


TEST(gamejam, colshae_just_touching_right) {
  const auto r1 = Colshape(0, 0, 10, 10);

  EXPECT_FALSE(r1.CollidesWith(Colshape(11, 0, 10, 10)));

  for (int64_t i = 0; i < 10; ++i) {
    const auto r2 = Colshape(10 - i, 0, 10, 10);
    EXPECT_TRUE(r1.CollidesWith(r2));
    EXPECT_TRUE(r2.CollidesWith(r1));
  }
}

TEST(gamejam, colshape_just_touching_down) {
  const auto r1 = Colshape(0, 0, 10, 10);

  EXPECT_FALSE(r1.CollidesWith(Colshape(0, 11, 10, 10)));

  for (int64_t i = 0; i < 10; ++i) {
    const auto r2 = Colshape(0, 0, 10, 10 - i);
    EXPECT_TRUE(r1.CollidesWith(r2));
    EXPECT_TRUE(r2.CollidesWith(r1));
  }
}

TEST(gamejam, colshape_just_touching_corners) {
  const int64_t
    max_left = 0,
    max_width = 10,
    max_up = 0,
    max_height = 10;

  const Colshape
    tl = Colshape(max_left  - 1, max_up     - 1, 2, 2),
    /* check to see that a pixel will collide with the top right corner */
    tr = Colshape(max_width    , max_up     - 1, 1, 1),
    /* check to see pixel collides with bottom left corner */
    bl = Colshape(max_left  - 1, max_height, 1, 1),
    br = Colshape(max_width    , max_height, 1, 1);

  const auto
    r1 = Colshape(max_left, max_up, max_width, max_height);

  EXPECT_TRUE(r1.CollidesWith(tl));
  EXPECT_TRUE(r1.CollidesWith(tr));
  EXPECT_TRUE(r1.CollidesWith(bl));
  EXPECT_TRUE(r1.CollidesWith(br));
}

