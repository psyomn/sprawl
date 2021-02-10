#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "tinydb/page.h"

TEST(core, page_default_size) {
  psy::tinydb::Page page;
  EXPECT_EQ(page.GetPageSize(), psy::tinydb::Page::GetDefaultPageSize());
}
