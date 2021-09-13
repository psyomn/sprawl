/*
   Copyright 2019 Simon (psyomn) Symeonidis

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
#include "symbol_table.h"

#include <vector>
#include <string>

namespace psy::bk {
  class Module {
  public:

    Module(const std::string& filename,
           std::vector<bk::Token>& tokens) :
      filename_(filename),
      tokens_(tokens)
      {}

    inline std::string filename() const { return filename_; }
    inline std::vector<bk::Token>tokens_tokens() const { return tokens_; }

    void BuildSymbolTable();

    std::string filename_;
    std::vector<bk::Token> tokens_;
    SymbolTable mSymbolTable;

    friend std ::ostream& operator<<(std::ostream& stream, const Module& module) {
      stream << module.filename_ << " "
             << module.tokens_.size()
             << std::endl << "   ";

      for (auto& token : module.tokens_) stream << token << " ";

      return stream;
    }
  };
}
