#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "tinydb/schema.h"

TEST(schema, add_table) {
  psy::tinydb::Schema schema;

  schema.AddTable(std::move(
    psy::tinydb::Table{
      .name_ = "person",
      .columns_ = {
        {"id", 8}, {"name", 32}, {"email", 256},
      }
    }
  ));

  schema.AddTable(std::move(
    psy::tinydb::Table{
      .name_ = "monster",
      .columns_ = {
        {"id", 8}, {"name", 32}, {"description", 256},
        {"friendly_to_humans", 1},
      }
    }
  ));

  const auto added = schema.GetTableNames();
  EXPECT_THAT(added, ::testing::ContainerEq(std::vector<std::string>{"person", "monster"}));
}
