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
