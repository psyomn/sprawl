#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "tinydb/statement.h"

TEST(core, invalid_statements) {
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

TEST(core, select_statement_correct_type) {
  using namespace psy;
  auto stat = tinydb::Statement("select * from test_table");
  EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Select);
}

TEST(core, insert_statement_correct_type) {
  using namespace psy;
  auto stat = tinydb::Statement("insert 1, 2, 3 into test_table");
  EXPECT_EQ(stat.GetStatementType(), tinydb::Statement::Type::Insert);
}
