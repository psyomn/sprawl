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
      .name_ = "monsters",
      .columns_ = {
        {"id", 8, CT::Integer},
        {"name", 32, CT::String},
        {"description", 256, CT::String},
      }
    }
  ));

  const auto added = schema.GetTableNames();
  EXPECT_THAT(added, ::testing::ContainerEq(std::vector<std::string>{"person", "monsters"}));

  {
    const auto monster_table = schema.FindTableByName("monsters");
    ASSERT_TRUE(monster_table.has_value());

    const auto columns = monster_table.value()->columns_;
    EXPECT_EQ(columns.size(), 3);

    std::vector<std::string> actual;
    for (const auto& e : columns) actual.push_back(e.label_);
    const auto expected = std::vector<std::string>({"id", "name", "description"});
    EXPECT_THAT(actual, ::testing::ContainerEq(expected));
  }

  {
    const auto people_table = schema.FindTableByName("person");
    ASSERT_TRUE(people_table.has_value());

    const auto columns = people_table.value()->columns_;
    EXPECT_EQ(columns.size(), 3);

    std::vector<std::string> actual;
    for (const auto& e : columns) actual.push_back(e.label_);
    const auto expected = std::vector<std::string>({"id", "name", "email"});
    EXPECT_THAT(actual, ::testing::ContainerEq(expected));
  }

  {
    const auto no_such_table = schema.FindTableByName("nope");
    ASSERT_FALSE(no_such_table.has_value());
  }
}
