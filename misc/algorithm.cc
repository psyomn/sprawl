/**
 * algorithm has a bunch of useful utilities, and I'd like to have a
 * vague knowing of them. Therefore I decided to use everything at
 * least once. I'll try with different containers as well.
 */
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

/** test objects */

class Person {
public:
  Person(std::string name, std::uint8_t age) :
    name_(name), age_(age) {}
  const std::string& GetName() const noexcept { return name_; }
  std::uint8_t GetAge() const noexcept { return age_; }
private:
  std::string name_;
  std::uint8_t age_;
};

static const std::vector<std::string> kStrVec = {
  "one", "two", "three", "four", "potato"
};

static const std::vector<Person> kPeople = {
  Person("bob", 12), Person("jane", 13), Person("chris", 14),
  Person("barbara", 12), Person("nope", 12),
};

/** tests */

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

TEST(algorithm, vector_find_if_object) {
  {
    std::vector<Person>::const_iterator it =
      std::find_if(kPeople.cbegin(), kPeople.cend(),
                   [](const Person& p) { return p.GetName() == "jane"; });
    EXPECT_TRUE(it != kPeople.cend());
  }

  {
    const ssize_t count =
      std::count_if(kPeople.cbegin(), kPeople.cend(),
                   [](const Person& p) { return p.GetAge() % 2 == 0; });
    EXPECT_EQ(count, 4);
  }
}
