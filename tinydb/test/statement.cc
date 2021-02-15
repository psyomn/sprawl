#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "tinydb/statement.h"

TEST(statement, invalid_statements) {
  using namespace psy;
  {
    auto stat = tinydb::Statement("nope nope nope");
    EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Invalid);
  }
  {
    auto stat = tinydb::Statement("");
    EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Invalid);
  }
}

TEST(statement, select_statement_correct_type) {
  using namespace psy;
  auto stat = tinydb::Statement("select * from test_table");
  EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Select);
}

TEST(statement, insert_statement_correct_type) {
  using namespace psy;
  auto stat = tinydb::Statement("insert 1, 2, 3 into test_table");
  EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Insert);
}

TEST(statement, statement) {
  const std::string create_table =
    "create table people (id int, name string, email string)";

  psy::tinydb::Statement stat(create_table);
  EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Create);
}
