#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "rfc4122-uuid/common.h"
#include "rfc4122-uuid/mac.h"
#include "rfc4122-uuid/uuid.h"

#include <cstdint>

TEST(uiid, mac_generate) {
  using namespace psy;
  auto generator = uuid::Generator();
  auto uuid = generator.Generate(uuid::Method::name);
  auto niluuid = generator.Generate(uuid::Method::nil);
  EXPECT_TRUE(*uuid != *niluuid);
}

TEST(mac, mac_string_to_int) {
  using namespace psy;
  auto val = uuid::MacStringToInt("aa:aa:aa:aa:aa:aa");
  const std::uint64_t expected = 0x0000AAAAAAAAAAAA;
  EXPECT_EQ(val, expected);
}
