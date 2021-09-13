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
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "lexer.h"

#include <algorithm>
#include <iostream>
#include <fstream>

using namespace psy::bk;

/* you can use this if you need to debug:
 * Lexer::PrintTokenList(tokens);
 */

TEST(bk, lex_simple_words) {
  std::stringstream ss("hello there i am token");
  Lexer lex(ss);
  const auto tokens = lex.Process();
  EXPECT_EQ(tokens.size(), 5);
}

TEST(bk, lex_simple_words_with_newline) {
  std::stringstream ss(
      "this is one line\n" /* 5 */
      "it would be terrible     if a lof of whitespace   broke this\n" /* 12 */
      "the author would be mad and sad." /* 7 */
  );
  Lexer lex(ss);
  const auto tokens = lex.Process();
  EXPECT_EQ(tokens.size(), 25);
}

TEST(bk,lex_codeblock) {
  std::stringstream ss(
      "```erlang\n"
      "hello(there) -> boom.\n"
      "```"
  );

  Lexer lex(ss);
  const auto tokens = lex.Process();

  EXPECT_EQ(tokens.size(), 13);
  EXPECT_EQ(tokens[0].type_, Token::Type::kCodeDelimiter);
  EXPECT_EQ(tokens[1].type_, Token::Type::kWord);
  EXPECT_EQ(tokens[tokens.size()-1].type_, Token::Type::kCodeDelimiter);
}

TEST(bk, lex_multiple_codeblocks) {
  std::stringstream ss(
      "```erlang\n"
      "hello(world) -> arghmytoe.\n"
      "```\n"
      "\n"
      "```cpp\n"
      "delete this; // no really don't\n"
      "```\n"
      "that's all folks"
  );

  Lexer lex(ss);
  const auto tokens = lex.Process();

  EXPECT_EQ(tokens.size(), 32);
  EXPECT_EQ(tokens[0].type_, Token::Type::kCodeDelimiter);
  EXPECT_EQ(tokens[1].type_, Token::Type::kWord);
  EXPECT_EQ(tokens[12].type_, Token::Type::kCodeDelimiter);

  EXPECT_EQ(tokens[15].type_, Token::Type::kCodeDelimiter);
  EXPECT_EQ(tokens[16].type_, Token::Type::kWord);

  EXPECT_EQ(tokens[tokens.size()-1].type_, Token::Type::kWord);
}

TEST(bk, lex_headers) {
  std::stringstream ss(
      "# hello there\n"
      "these are simple words\n"
      "## this is a subsection\n"
      "and these are more words\n"
      "### another section\n"
      "and even more words!\n"
  );

  Lexer lex(ss);
  const auto tokens = lex.Process();

  Lexer::PrintTokenList(tokens);
}

TEST(bk, lex_bad_headers) {
  std::stringstream ss(
      "### another section\n"
      "#### we dont support level 4 -- and that's ok\n"
      "##### we dont support level 5 -- and that's also ok\n"
  );

  Lexer lex(ss);
  const auto tokens = lex.Process();
  Lexer::PrintTokenList(tokens);
}

TEST(bk, file_lex_codeblock) {
  std::ifstream ifs("mdm/data/code-blocks.md");
  Lexer lex(ifs);
  const auto tokens = lex.Process();

  EXPECT_EQ(tokens.size(), 74);
}

