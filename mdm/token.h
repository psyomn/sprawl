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

#include <string>
#include <cstdint>
#include <ostream>

namespace psy::bk {
  struct Token {
    enum class Type {
      kUnknown, kID, kWord, kPunct, kSymbol,
      kNewline, kWhitespace, kCodeDelimiter,

      kHeader1, kHeader2, kHeader3
    };

    Token(
      const std::string& value,
      std::size_t line,
      std::size_t column);

    ~Token();

    inline bool MatchesStr(const std::string& str) const { return str == token_; }

    bool operator==(const Token& other) {
      return token_ == other.token_;
    }

    Type type_;
    std::string token_;
    std::size_t line_number_;
    std::size_t column_;
    bool is_first_on_newline_;

    friend std::ostream& operator<<(std::ostream&, const Token&);
  };
}
