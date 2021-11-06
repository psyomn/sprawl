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

#include "parser.h"
#include "state.h"

namespace psy::bk {
  Parser::Parser(std::vector<Token>& tokens) :
    tokens_(tokens), transitions_() {
      {
        /* header 1 definition */
        Token t1 = Token("#", 0, 0);
        t1.type_ = Token::Type::kHeader1;

        Token t2 = Token("##", 0, 0);
        t2.type_ = Token::Type::kHeader2;

        Token t3 = Token("###", 0, 0);
        t3.type_ = Token::Type::kHeader3;
      }

      {
        /* list definition */
      }

      /* code block definition */
  }

  /**
   * Mark nodes that start on a new line.
   *
   * Arguably we could add this in the lexer, but it might be simpler/cleaner
   * here.
   */
  void Parser::MarkNewlineTokens() {
    bool found_newline = false;
    bool first_token = true;

    for (auto& tok : tokens_) {
      if (first_token) {
        tok.is_first_on_newline_ = true;
        first_token = false;
        continue;
      }

      if (tok.type_ == Token::Type::kNewline) {
        found_newline = true;
        continue;
      }

      if (found_newline) {
        tok.is_first_on_newline_ = true;
        found_newline = false;
      }
    }
  }

  /**
   * Logic specific to prunning or other stuff should go here.  So right now,
   * it's not exactly parsing per se, but could be in the future.
   */
  void Parser::ProcessRules() {}

  /**
   * Once the parser is done processing, you can output to a stream the results.
   */
  void Parser::Write(std::ostream& stream) const {
    for (auto e : tokens_) {
      // TODO: eventually I should detect whitespace and tokenize it
      // (unfortunately), and some of the bellow exceptions will be removed.
      stream << e.token_;

      if (e.type_ == Token::Type::kNewline) continue;
      stream << " ";
    }
  }
}
