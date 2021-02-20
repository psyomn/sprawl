#pragma once

#include <optional>
#include <string>
#include <cstdint>
#include <vector>

#include "error.h"
#include "schema.h"

namespace psy::tinydb {
  class Statement {
  public:
    enum class Type { Undefined, Insert, Select, Create, Invalid };
    enum class ExecutionResult { Undefined, Success, Failure };

    explicit Statement(const std::string& statement, Schema& schema) :
      original_statement_(statement),
      statement_type_(Type::Undefined),
      execution_result_(ExecutionResult::Undefined),
      error_(std::nullopt),
      schema_(schema)
    { Parse(); }

    inline enum Type GetStatementType(void) const noexcept { return statement_type_; }
    enum ExecutionResult Execute() noexcept;
    const std::optional<Error> GetState() const noexcept;

  private:
    std::string original_statement_;
    enum Type statement_type_;
    enum ExecutionResult execution_result_;
    std::optional<Error> error_;
    Schema& schema_;

    void Parse() noexcept;
    void ParseInsert(const std::vector<std::string>& tokens) noexcept;
    void ParseSelect(const std::vector<std::string>& tokens) noexcept;
    void ParseCreate(const std::vector<std::string>& tokens) noexcept;
  };

  std::optional<std::uint64_t> ParseColumnSize(const std::string& column) noexcept;
}
