#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "rfc4122-uuid/uuid.h"

#include <memory>

TEST(uuid, from_string) {
  using namespace psy;
  {
    std::unique_ptr<uuid::UUID> uuid = uuid::Generator::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    auto expected = std::array<std::uint64_t, 2> { 0xffffffffffffffff, 0xffffffffffffffff };
    EXPECT_THAT(uuid->Raw(), ::testing::ContainerEq(expected));
  }
  {
    std::unique_ptr<uuid::UUID> uuid = uuid::Generator::FromString("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee");
    auto expected = std::array<std::uint64_t, 2> { 0xaaaaaaaabbbbcccc, 0xddddeeeeeeeeeeee };
    EXPECT_THAT(uuid->Raw(), ::testing::ContainerEq(expected));
  }
}

TEST(uuid, cmp_eq) {
  using namespace psy;
  {
    auto uuid1 = uuid::Generator::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    auto uuid2 = uuid::Generator::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    EXPECT_TRUE(uuid1 == uuid2);
  }
  {
    auto uuid1 = uuid::Generator::FromString("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    auto uuid2 = uuid::Generator::FromString("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    EXPECT_TRUE(uuid1 == uuid2);
  }
  {
    auto uuid1 = uuid::Generator::FromString("cccccccc-cccc-cccc-cccc-cccccccccccc");
    auto uuid2 = uuid::Generator::FromString("cccccccc-cccc-cccc-cccc-cccccccccccc");
    EXPECT_TRUE(uuid1 == uuid2);
  }
}

TEST(uuid, cmp_neq) {
  using namespace psy;
  {
    auto uuid1 = uuid::Generator::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    auto uuid2 = uuid::Generator::FromString("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    EXPECT_TRUE(uuid1 != uuid2);
  }
  {
    auto uuid1 = uuid::Generator::FromString("cccccccc-cccc-cccc-cccc-cccccccccccc");
    auto uuid2 = uuid::Generator::FromString("ffffffff-ffff-ffff-ffff-ffffffffffff");
    EXPECT_TRUE(uuid1 != uuid2);
  }
  {
    auto uuid1 = uuid::Generator::FromString("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    auto uuid2 = uuid::Generator::FromString("cccccccc-cccc-cccc-cccc-cccccccccccc");
    EXPECT_TRUE(uuid1 != uuid2);
  }
}
