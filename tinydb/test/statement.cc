#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "tinydb/statement.h"

#include <optional>

const psy::tinydb::Error kDefaultError{"should have error"};

TEST(statement, invalid_statements) {
  using namespace psy;
  {
    psy::tinydb::Schema s;
    auto stat = tinydb::Statement("nope nope nope", s);
    EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Invalid);
    EXPECT_TRUE(stat.GetState().has_value());
    EXPECT_EQ(stat.GetState().value_or(kDefaultError).GetMessage(),
              "bad starter identifier");
  }
  {
    psy::tinydb::Schema s;
    auto stat = tinydb::Statement("", s);
    EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Invalid);
    EXPECT_TRUE(stat.GetState().has_value());
    EXPECT_EQ(stat.GetState().value_or(kDefaultError).GetMessage(),
              "empty query");
  }
}

TEST(statement, select_statement_correct_type) {
  psy::tinydb::Schema s;
  using namespace psy;
  auto stat = tinydb::Statement("select * from test_table", s);
  EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Select);
}

TEST(statement, insert_statement_correct_type) {
  psy::tinydb::Schema s;
  using namespace psy;
  auto stat = tinydb::Statement("insert into test_table col1 col2 col3 values 1 2 3", s);
  EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Insert);
}

TEST(statement, create_table) {
  psy::tinydb::Schema s;
  const std::string create_table =
    "create table people id int name varchar(32) email varchar(256)";

  psy::tinydb::Statement stat(create_table, s);
  EXPECT_FALSE(stat.GetState().has_value());
  EXPECT_EQ(stat.GetStatementType(), psy::tinydb::Statement::Type::Create);

  const auto names = s.GetTableNames();
  EXPECT_EQ(names, std::vector<std::string>{"people"});
}

TEST(statement, unique_columns) {
  psy::tinydb::Schema s;
  const std::string dup_cols =
    "create table people name varchar(32) name varchar(32)";

  psy::tinydb::Statement stat(dup_cols, s);

  EXPECT_TRUE(stat.GetState().has_value());
  EXPECT_EQ(stat.GetState()
                .value_or(kDefaultError)
                .GetMessage(), "duplicate column names");
}
