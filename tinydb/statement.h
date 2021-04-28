/*
   Copyright 2021 Simon (psyomn) Symeonidis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
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
      schema_(schema),
      wildcard_values_(false),
      values_({}),
      table_name_({})
    { Parse(); }

    inline enum Type GetStatementType(void) const noexcept { return statement_type_; }
    enum ExecutionResult Execute() noexcept;
    const std::optional<Error> GetState() const noexcept;
    bool HasWildcardValues() const noexcept { return wildcard_values_; }
    const std::vector<std::string>& GetValues() const noexcept { return values_; }
    const std::optional<std::string>& GetTableName() const noexcept { return table_name_; }

  private:
    std::string original_statement_;
    enum Type statement_type_;
    enum ExecutionResult execution_result_;
    std::optional<Error> error_;
    Schema& schema_;
    bool wildcard_values_;
    std::vector<std::string> values_;
    std::optional<std::string> table_name_;

    void Parse() noexcept;
    void ParseInsert(const std::vector<std::string>& tokens) noexcept;
    void ParseSelect(const std::vector<std::string>& tokens) noexcept;
    void ParseCreate(const std::vector<std::string>& tokens) noexcept;
  };

  std::optional<std::uint64_t> ParseColumnSize(const std::string& column) noexcept;
  std::optional<ColumnType> ParseColumnType(const std::string& column) noexcept;
}
