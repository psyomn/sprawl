#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "tinydb/schema.h"

TEST(schema, add_table) {
  psy::tinydb::Schema schema;

  using CT = psy::tinydb::ColumnType;

  schema.AddTable(std::move(
    psy::tinydb::Table{
      .name_ = "person",
      .columns_ = {
        {"id", 8, CT::Integer},
        {"name", 32, CT::String},
        {"email", 256, CT::String},
      }
    }
  ));

  schema.AddTable(std::move(
    psy::tinydb::Table{
      .name_ = "monster",
      .columns_ = {
        {"id", 8, CT::Integer},
        {"name", 32, CT::String},
        {"description", 256, CT::String},
      }
    }
  ));

  const auto added = schema.GetTableNames();
  EXPECT_THAT(added, ::testing::ContainerEq(std::vector<std::string>{"person", "monster"}));
}
