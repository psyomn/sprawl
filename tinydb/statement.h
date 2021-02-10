#pragma once

#include <string>
#include <vector>

namespace psy::tinydb {
  class Statement {
  public:
    enum class Type { Undefined, Insert, Select, Invalid };
    enum class ExecutionResult { Undefined, Success, Failure };

    explicit Statement(const std::string& statement) :
      original_statement_(statement),
      statement_type_(Type::Undefined),
      execution_result_(ExecutionResult::Undefined)
    { Parse(); }

    inline enum Type GetStatementType(void) const noexcept { return statement_type_; }
    enum ExecutionResult Execute() noexcept;

  private:
    std::string original_statement_;
    enum Type statement_type_;
    enum ExecutionResult execution_result_;

    void Parse() noexcept;
    void ParseInsert(const std::vector<std::string>& tokens) noexcept;
    void ParseSelect(const std::vector<std::string>& tokens) noexcept;
  };
}
