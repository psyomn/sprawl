/**
 * algorithm has a bunch of useful utilities, and I'd like to have a
 * vague knowing of them. Therefore I decided to use everything at
 * least once. I'll try with different containers as well.
 */
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <algorithm>
#include <string>
#include <vector>

static const std::vector<std::string> kStrVec = {
  "one", "two", "three", "four", "potato"
};

TEST(algorithm, vector_all_of) {
  {
    const bool result = std::all_of(kStrVec.cbegin(), kStrVec.cend(),
                                    [](const std::string& str) { return str.size() > 2; });
    EXPECT_TRUE(result);
  }
}

TEST(algorithm, vector_find_if) {
  {
    std::vector<std::string>::const_iterator it =
      std::find_if(kStrVec.cbegin(), kStrVec.cend(),
                   [](const std::string& str) -> bool {
                     return str == "potato";
                   });

    EXPECT_TRUE(it != kStrVec.cend());
  }

  {
    std::vector<std::string>::const_iterator it =
      std::find_if(kStrVec.cbegin(), kStrVec.cend(),
                   [](const std::string& str) -> bool {
                     return str == "notato";
                   });

    EXPECT_TRUE(it == kStrVec.cend());
  }
}
