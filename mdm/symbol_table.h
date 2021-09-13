/*
   Copyright 2020 Simon (psyomn) Symeonidis

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

#include "token.h"
#include "error.h"

#include <unordered_set>
#include <vector>

namespace psy::bk {
  struct SymbolTable {
  public:
    SymbolTable() : headers_({}), errors_({}) {}
    void FromTokens(const std::vector<Token>&);
    bool HasErrors() const;
  private:
    void clear();
    std::unordered_set<std::string> headers_;
    std::vector<Error> errors_;
  };
}
