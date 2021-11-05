/*
   Copyright 2019-2021 Simon (psyomn) Symeonidis

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

#include "state.h"

#include <vector>
#include <map>
#include <ostream>

namespace psy::bk {
  struct Parser {
  public:
    Parser(std::vector<Token>& tokens);
    void MarkNewlineTokens();

    void Parse();

    void Write(std::ostream& stream) const;
  private:
    std::vector<Token>& tokens_;
    std::map<State, State> transitions_;
  };
}
