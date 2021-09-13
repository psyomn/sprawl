#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "uuid/uuid.h"

#include <memory>

TEST(uuid, from_string) {
  using namespace psy;
  {
    std::unique_ptr<uuid::UUID> uuid = psy::uuid::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    auto expected = std::array<std::uint64_t, 2> { 0xffffffffffffffff, 0xffffffffffffffff };
    EXPECT_THAT(uuid->Raw(), ::testing::ContainerEq(expected));
  }
  {
    std::unique_ptr<uuid::UUID> uuid = psy::uuid::FromString("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee");
    auto expected = std::array<std::uint64_t, 2> { 0xaaaaaaaabbbbcccc, 0xddddeeeeeeeeeeee };
    EXPECT_THAT(uuid->Raw(), ::testing::ContainerEq(expected));
  }
}

TEST(uuid, nil_uuid_to_string) {
  using namespace psy;
  const std::string expected = "00000000-0000-0000-0000-000000000000";
  const auto uuid1 = psy::uuid::FromString(expected);
  const std::string str = uuid1->String();
  ASSERT_EQ(expected, str);
}

TEST(uuid, from_and_to_string) {
  using namespace psy;
  {
    const std::string expected = "f81d4fae-7dec-11d0-a765-00a0c91e6bf6";
    const auto uuid1 = psy::uuid::FromString(expected);
    const std::string str = uuid1->String();
    EXPECT_EQ(expected, str);
  }
  {
    const std::string expected = "ffffffff-ffff-ffff-ffff-ffffffffffff";
    const auto uuid1 = psy::uuid::FromString(expected);
    const std::string str = uuid1->String();
    EXPECT_EQ(expected, str);
  }
  {
    const std::string expected = "aabbccdd-eeee-ffff-0000-111112222233";
    const auto uuid1 = psy::uuid::FromString(expected);
    const std::string str = uuid1->String();
    EXPECT_EQ(expected, str);
  }
}

TEST(uuid, cmp_eq) {
  using namespace psy;
  {
    const auto uuid1 = psy::uuid::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    const auto uuid2 = psy::uuid::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    const auto r1 = uuid1->Raw();
    const auto r2 = uuid2->Raw();
    EXPECT_TRUE(r1[0] == r2[0]);
    EXPECT_TRUE(r1[1] == r2[1]);
    EXPECT_TRUE(*uuid1 == *uuid2);
  }
  {
    const auto uuid1 = psy::uuid::FromString("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    const auto uuid2 = psy::uuid::FromString("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    EXPECT_TRUE(*uuid1 == *uuid2);
  }
  {
    const auto uuid1 = psy::uuid::FromString("cccccccc-cccc-cccc-cccc-cccccccccccc");
    const auto uuid2 = psy::uuid::FromString("cccccccc-cccc-cccc-cccc-cccccccccccc");
    EXPECT_TRUE(*uuid1 == *uuid2);
  }
}

TEST(uuid, cmp_neq) {
  using namespace psy;
  {
    auto uuid1 = psy::uuid::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    auto uuid2 = psy::uuid::FromString("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    EXPECT_TRUE(uuid1 != uuid2);
  }
  {
    auto uuid1 = psy::uuid::FromString("cccccccc-cccc-cccc-cccc-cccccccccccc");
    auto uuid2 = psy::uuid::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    EXPECT_TRUE(uuid1 != uuid2);
  }
  {
    auto uuid1 = psy::uuid::FromString("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    auto uuid2 = psy::uuid::FromString("cccccccc-cccc-cccc-cccc-cccccccccccc");
    EXPECT_TRUE(uuid1 != uuid2);
  }
}
