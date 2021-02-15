#include "schema.h"

#include <algorithm>

namespace psy::tinydb {
  void Schema::AddTable(Table&& table) noexcept {
    tables_.push_back(std::move(table));
  }

  std::vector<std::string> Schema::GetTableNames(void) const noexcept {
    std::vector<std::string> ret;
    for (auto const& e : tables_) ret.push_back(e.name_);
    return ret;
  }

  std::optional<const Table*>
  Schema::FindTableByName(const std::string& name) const noexcept {
    std::optional<const Table*> ret(std::nullopt);

    auto it = std::find_if(tables_.cbegin(),
                           tables_.cend(),
                           [&name](const Table& table){ return table.name_ == name; });

    if (it != tables_.cend()) return std::optional<const Table*>(&*it);

    return std::nullopt;
  }
}
