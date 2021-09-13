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
#include "module.h"

#include <iostream>
#include <cstdint>
#include <string>
#include <istream>
#include <vector>

namespace psy::bk {
  struct Lexer {
  public:
    Lexer(std::istream& stream);
    std::vector<Token> Process();

    static void PrintTokenList(const std::vector<Token>& tokens) {
      for (const auto& token : tokens)
        std::cout << token << " ";
      std::cout << std::endl;
    }

    std::size_t line_num_;
    std::size_t col_num_;
    std::string root_;
  private:
    void TokenizeFile(const std::string&);
    std::istream& input_;
  };
}
