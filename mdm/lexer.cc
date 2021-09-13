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

#include "intrinsics.h"

#include "lexer.h"
#include "token.h"

#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>

namespace psy::bk {
  const std::string kPunct("{}.,;:\\\"!?-()[]<>`");
  const std::string kSymbol("@#$^&*|\\/");
  const std::string kWhitespace(" \t\r");
  inline bool IsPunct(char c)      { return kPunct.find_first_of(c) != std::string::npos; }
  inline bool IsSymbol(char c)     { return kSymbol.find_first_of(c) != std::string::npos; }
  inline bool IsNewline(char c)    { return c == '\n'; }
  inline bool IsWhitespace(char c) { return kWhitespace.find_first_of(c) != std::string::npos; }

  Lexer::Lexer(std::istream& is) :
    line_num_(0),
    col_num_(0),
    input_(is) {}

  std::vector<Token> Lexer::Process() {
    std::vector<Token> tokens;
    std::size_t linecount = 0;
    std::size_t colcount = 0;
    std::stringstream ss;
    char c = 0;

    while (input_.get(c)) {
      ++colcount;

      if (ss.str() != "" &&
             (IsSymbol(c)
           || IsPunct(c)
           || IsWhitespace(c)
           || IsNewline(c))) {
        if (ss.str() == "") continue;

        Token tok(ss.str(), linecount, colcount);
        tok.type_ = Token::Type::kWord;
        tokens.push_back(std::move(tok));

        ss.str(std::string());

        // NB: no continue here -- we want to fall through,
        // and process the 'c'
      }

      if (IsWhitespace(c)) {
        continue;
      }

      if (IsNewline(c)) {
        ++linecount;
        Token tok("", linecount, colcount);
        tok.type_ = Token::Type::kNewline;
        tokens.push_back(tok);

        ss.str({});
        continue;
      }

      if (IsPunct(c)) {
        if (c == '`') {
          /* Start of code blocks have three backticks (and an id):
           *
           * ```nocode
           * blah deeh blah
           * ```
           *
           * ```erlang
           * hello(there) -> stranger.
           * ```
           *
           * ```potatolang
           * potato. potato. potato. potato? potato.
           * ```
           */
          const std::streampos original = input_.tellg();
          const char prev = c;

          /* we're parsing the second ` here */
          input_.get(c);
          if (c == EOF || c != '`') {
            input_.seekg(original);
            c = prev;
            goto generic_punct;
          }

          /* and the third ` here ... */
          input_.get(c);
          if (c == EOF || c != '`') {
            input_.seekg(int(original) + 1);
            c = prev;
            goto generic_punct;
          }

          /* if we successfully found that the third ` exists,
           * then we add a "codeblock-delim" token, and continue
           */
          auto tok = Token("```", linecount, colcount);
          tok.type_ = Token::Type::kCodeDelimiter;
          tokens.push_back(std::move(tok));
          continue;
        }

generic_punct:
        ss << c;
        Token tok(std::move(ss.str()), linecount, colcount);
        tok.type_ = Token::Type::kPunct;
        tokens.push_back(std::move(tok));

        ss.str(std::string());
        continue;
      }

      if (IsSymbol(c)) {
        /* we might be able to parse headers here. Headers are in the form of
         *
         * # blah
         * ## blah
         * ### blah
         *
         * in the case that we get something like this:
         */
        if (c == '#') {
          const char prev = c;
          const std::streampos original = input_.tellg();

          input_.get(c);
          if (c == EOF || c != '#') {
            input_.seekg(original);
            c = prev;
            auto tok = Token("#", linecount, colcount);
            tok.type_ = Token::Type::kHeader1;
            tokens.push_back(std::move(tok));
            continue;
          }

          input_.get(c);
          if (c == EOF || c != '#') {
            input_.seekg(int(original) + 1);
            c = prev;
            auto tok = Token("##", linecount, colcount);
            tok.type_ = Token::Type::kHeader2;
            tokens.push_back(std::move(tok));
            continue;
          }

          auto tok = Token("###", linecount, colcount);
          tok.type_ = Token::Type::kHeader3;
          tokens.push_back(std::move(tok));
          continue;
        }

        ss << c;
        Token tok(ss.str(), linecount, colcount);
        tok.type_ = Token::Type::kSymbol;
        tokens.push_back(std::move(tok));

        ss.str(std::string());
        continue;
      }

      ss << c;
    }

    /* TODO: lexer needs rethinking ... */
    const std::string maybe_last = ss.str();
    if (maybe_last != "") {
      auto tok = Token(maybe_last, linecount, colcount);
      if (IsPunct(maybe_last[0])) tok.type_ = Token::Type::kPunct;
      else if (IsSymbol(maybe_last[0])) tok.type_ = Token::Type::kSymbol;
      else if (IsNewline(maybe_last[0])) tok.type_ = Token::Type::kNewline;
      else tok.type_ = Token::Type::kWord;

      tokens.push_back(std::move(tok));
    }

    return tokens;
  }
}
