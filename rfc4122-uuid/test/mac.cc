#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "rfc4122-uuid/common.h"
#include "rfc4122-uuid/uuid.h"

TEST(uiid, mac_generate) {
  using namespace psy;
  auto generator = uuid::Generator();
  auto uuid = generator.Generate(uuid::Method::name);
  auto niluuid = generator.Generate(uuid::Method::nil);
  EXPECT_TRUE(*uuid != *niluuid);
}
