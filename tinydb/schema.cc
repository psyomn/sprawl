#include "schema.h"

namespace psy::tinydb {
  void Schema::AddTable(Table&& table) noexcept {
    tables_.push_back(std::move(table));
  }

  std::vector<std::string> Schema::GetTableNames(void) const noexcept {
    std::vector<std::string> ret;
    for (auto const& e : tables_) ret.push_back(e.name_);
    return ret;
  }
}
