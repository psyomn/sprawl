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
#include "token.h"

#include <cstdlib>

namespace psy::bk {
  Token::Token(
      const std::string& value,
      std::size_t line,
      std::size_t column) :
    type_(Type::kUnknown), token_(value),
    line_number_(line), column_(column),
    is_first_on_newline_(false) {}

  Token::~Token() {}

  std::ostream& operator<<(std::ostream& stream, const bk::Token& tok) {
    switch (tok.type_) {
    case Token::Type::kUnknown:
      stream << "unknown(";
      break;
    case Token::Type::kID:
      stream << "id(";
      break;
    case Token::Type::kWord:
      stream << "word(";
      break;
    case Token::Type::kSymbol:
      stream << "symb(";
      break;
    case Token::Type::kPunct:
      stream << "punct(";
      break;
    case Token::Type::kNewline:
      stream << "newline(";
      break;
    case Token::Type::kCodeDelimiter:
      stream << "code-delim(";
      break;
    case Token::Type::kWhitespace:
      abort(); // TODO
      break;
    case Token::Type::kHeader1:
      stream << "header1(";
      break;
    case Token::Type::kHeader2:
      stream << "header2(";
      break;
    case Token::Type::kHeader3:
      stream << "header3(";
      break;
    default:
      abort();
    }

    return stream
      << tok.token_
      << ":" << tok.line_number_
      << ":" << tok.column_ << ")";
  }
}
