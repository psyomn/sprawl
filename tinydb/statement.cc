#include "statement.h"

#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>

namespace psy::tinydb {
  void Statement::Parse() noexcept {
    std::istringstream iss(original_statement_);
    std::vector<std::string> tokens{
      std::istream_iterator<std::string>{iss},
      std::istream_iterator<std::string>{},
    };

    if (tokens.size() < 1) {
      statement_type_ = Type::Invalid;
      return;
    }

    // todo: might want to care about casing
    if (tokens[0] == "insert") ParseInsert(tokens);
    else if (tokens[0] == "select") ParseSelect(tokens);
    else if (tokens[0] == "create") ParseCreate(tokens);
    else statement_type_ = Type::Invalid;
  }

  void Statement::ParseInsert(const std::vector<std::string>& tokens) noexcept {
    statement_type_ = Type::Insert;
  }

  void Statement::ParseSelect(const std::vector<std::string>& tokens) noexcept {
    statement_type_ = Type::Select;
  }

  void Statement::ParseCreate(const std::vector<std::string>& tokens) noexcept {
    statement_type_ = Type::Create;

    // create table
    if (tokens.size() < 2) {
      error_ = Error{"invalid create statement: expecting at least `create table`"};
      return;
    }

    if (tokens[0] != "create" || tokens[1] != "table") {
      error_ = Error{"queries should begin with `create table`"};
      return;
    }

    // create table blah colname type
    if (tokens.size() < 3) {
      error_ = Error{"invalid create statement: expecting table name with at least a column"};
      return;
    }

    auto col_count = tokens.size() - 3;

    if (col_count == 0) {
      error_ = Error{"must specify at least one column in the table"};
      return;
    }

    if (col_count % 2 != 0) {
      error_ = Error{"bad column definition"};
      return;
    }

    std::vector<Column> columns;
    auto it = tokens.cbegin() + 3;
    for (; it != tokens.cend(); it += 2)
      columns.push_back({.label_ = std::string(*it)});

    auto table_name = tokens[2];

    schema_.AddTable(std::move(Table{
      .name_ = table_name,
      .columns_ = std::move(columns),
    }));
  }

  enum Statement::ExecutionResult Statement::Execute() noexcept {
    switch(statement_type_) {
    case Type::Insert:
      std::cout << "insert stuff here" << std::endl;
      break;
    case Type::Select:
      std::cout << "select stuff here" << std::endl;
      break;
    case Type::Create:
      std::cout << "create the table here" << std::endl;
      break;
    case Type::Undefined:
    case Type::Invalid:
    default:
      return Statement::ExecutionResult::Failure;
    }

    return Statement::ExecutionResult::Failure;
  }

  const std::optional<Error> Statement::GetState() const noexcept {
    return error_;
  }
}
