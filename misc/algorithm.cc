/**
 * algorithm has a bunch of useful utilities, and I'd like to have a
 * vague knowing of them. Therefore I decided to use everything at
 * least once. I'll try with different containers as well.
 */
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <algorithm>
#include <cstdint>
#include <set>
#include <string>
#include <map>
#include <random>
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

TEST(algorithm, set_find_if) {
  std::set<std::uint8_t> s;
  s.insert(1);
  s.insert(2);
  s.insert(7);
  s.insert(9);

  // nb: use the member functions of set, unless you need something
  // like this.
  const auto it =
    std::find_if(s.cbegin(), s.cend(),
                 [](const std::uint8_t& v) { return v == 7; });

  EXPECT_TRUE(it != s.cend());
  EXPECT_EQ(*it, *s.find(7));
}

TEST(algorithm, map_find_if) {
  std::map<std::string, int> m = {
    {"bob", 1},
    {"jane", 20},
    {"chris", 4},
    {"mark", 1000},
  };

  // nb: mostly for fun and profit, probably don't use find_if
  const auto it =
    std::find_if(m.cbegin(), m.cend(),
                 [](const std::pair<std::string, int>& v){
                   return v.first == "mark";
                 });

  EXPECT_EQ(it->first, "mark");
  EXPECT_EQ(it->second, 1000);
}

TEST(algorithm, any_of_vector) {
  {
    const bool check_one =
      std::any_of(kStrVec.cbegin(), kStrVec.cend(),
                  [](const std::string& s) { return s == "one"; });
    EXPECT_TRUE(check_one);
  }

  {
    const bool check_none =
      std::any_of(kStrVec.cbegin(), kStrVec.cend(),
                  [](const std::string& s) { return s == "none"; });
    EXPECT_FALSE(check_none);
  }
}

TEST(algorithm, none_of_vector) {
  const bool check =
    std::none_of(kStrVec.cbegin(), kStrVec.cend(),
                 [](const decltype(kStrVec[0])& s) { return s == "none"; });
  EXPECT_TRUE(check);
}

TEST(algorithm, foreach) {
  { // lambdas
    std::vector<std::uint32_t> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::for_each(v.begin(), v.end(), [](std::uint32_t& val) { val++; });
    EXPECT_EQ(v, std::vector<std::uint32_t>({2, 3, 4, 5, 6, 7, 8, 9, 10}));
  }

  { // with functor object
    std::vector<std::uint32_t> v = { 1, 2, 3, 4 };
    class functor {
    public:
      void operator() (decltype(v[0])& val) { ++val; }
    } my_functor;

    std::for_each(v.begin(), v.end(), my_functor);
  }
}

TEST(algorithm, unique) {
  std::vector<std::uint32_t> v = {
    1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4,
  };

  std::vector<std::uint32_t>::iterator
    it = std::unique(v.begin(), v.end(),
                     [](decltype(v[0])& i,decltype(v[0])& j)
                     { return i == j; });

  auto new_size = decltype(v)::size_type(std::distance(v.begin(), it));
  if (new_size > 0) v.resize(new_size);

  EXPECT_EQ(v.size(), 4);
}

TEST(algorithm, replace_if) {
  std::vector<std::uint32_t> v = { 1, 1, 1, 1, 8, 8, 8, 1, 1 };

  std::replace_if(v.begin(), v.end(),
                  [](const std::uint32_t& val) { return val == 1; }, 0);

  EXPECT_EQ(v, std::vector<std::uint32_t>({0, 0, 0, 0, 8, 8, 8, 0, 0}));
}

TEST(algorithm, shuffle) {
  std::vector<std::uint32_t> v(100);
  std::generate_n(v.begin(), v.size(),
                  [](){
                    // nb: not mt safe, so don't do this at home kids!
                    static std::uint32_t count = 0;
                    return ++count;
                  });

  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[99], 100);
  EXPECT_EQ(v[49], 50);

  const auto copy_v = std::vector<std::uint32_t>(v);

  std::shuffle(v.begin(), v.end(),
               std::default_random_engine(1337));

  EXPECT_EQ(v[0], 28);
  EXPECT_EQ(v[99], 12);
}

TEST(algorithm, partition) {
  std::vector<std::string> og = {
    "banana", "anana", "pineapple",
    "laughlin", "derp souls", "hello",
    "tanzania", "yamaha", "patagonia",
    "something", "somethingelse", "definitelynot",
    "panama", "maintainable", "kampala",
    "blargh", "blorgh", "fizzbuzz",
    "disadvantage", "barbara", "arkansas",
  };

  auto match_fn = [](const std::string& s) {
    return std::count(s.cbegin(), s.cend(), 'a') >= 3;
  };

  auto num_matching = size_t(std::count_if(og.begin(), og.end(), match_fn));

  std::vector<std::string> pass;
  std::vector<std::string> fail;
  pass.resize(num_matching);
  fail.resize(og.size() - num_matching);

  std::partition_copy(og.begin(), og.end(),
                      pass.begin(),
                      fail.begin(),
                      match_fn);

  EXPECT_EQ(pass.size(), 11);
  EXPECT_EQ(fail.size(), 10);

  const std::vector<std::string> expected_pass = {
    "banana", "anana", "tanzania", "yamaha", "patagonia", "panama",
    "maintainable", "kampala", "disadvantage", "barbara", "arkansas",
  };

  const std::vector<std::string> expected_fail = {
    "pineapple", "laughlin", "derp souls", "hello", "something",
    "somethingelse", "definitelynot", "blargh", "blorgh", "fizzbuzz",
  };

  EXPECT_EQ(pass, expected_pass);
  EXPECT_EQ(fail, expected_fail);
}
