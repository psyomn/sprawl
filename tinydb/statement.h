#pragma once

#include <string>
#include <vector>

namespace psy::tinydb {
  class Statement {
  public:
    enum class Type { Undefined, Insert, Select, Invalid };

    explicit Statement(const std::string& statement) :
      original_statement_(statement),
      statement_type_(Type::Undefined) { Parse(); }

    enum Type GetStatementType(void) const noexcept {
      return statement_type_;
    }

  private:
    std::string original_statement_;
    enum Type statement_type_;

    void Parse() noexcept;
    void ParseInsert(const std::vector<std::string>& tokens) noexcept;
    void ParseSelect(const std::vector<std::string>& tokens) noexcept;
  };
}
