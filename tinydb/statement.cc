#include "statement.h"

#include <sstream>
#include <algorithm>
#include <iterator>

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
    else statement_type_ = Type::Invalid;
  }

  void Statement::ParseInsert(const std::vector<std::string>& tokens) noexcept {
    statement_type_ = Type::Insert;
  }

  void Statement::ParseSelect(const std::vector<std::string>& tokens) noexcept {
    statement_type_ = Type::Select;
  }
}
