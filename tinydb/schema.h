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

#include <cstddef>
#include <string>
#include <vector>
#include <optional>

namespace psy::tinydb {
  enum class ColumnType { Undefined, String, Integer };

  struct Column {
    std::string label_;
    std::size_t size_;
    enum ColumnType type_;
    /* TODO: attributes such as primary key, autoincrement, etc? */
  };

  struct Table {
    std::string name_;
    std::vector<Column> columns_;
  };

  class Schema {
  public:
    void AddTable(Table&& table) noexcept;
    std::vector<std::string> GetTableNames(void) const noexcept;
    std::optional<const Table*> FindTableByName(const std::string& name) const noexcept;
  private:
    std::vector<Table> tables_;
  };
}
