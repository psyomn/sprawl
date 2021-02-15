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
}
