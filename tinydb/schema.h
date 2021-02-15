#pragma once

#include <string>
#include <cstddef>
#include <vector>

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
  private:
    std::vector<Table> tables_;
  };
}
