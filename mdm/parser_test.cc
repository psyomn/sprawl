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
#include "parser.h"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace psy::bk;

/* you can use this if you need to debug:
 * Lexer::PrintTokenList(tokens);
 */

TEST(bk, parse_first_on_newline) {
  std::stringstream ss(
      "# header\n"
      "a b c\n"
      "a b c\n"
      "- l1\n"
      "- l2\n"
      "- l l l l 1\n"
  );

  Lexer lex(ss);
  auto tokens = lex.Process();

  Parser parser(tokens);
  parser.MarkNewlineTokens();

  EXPECT_EQ(tokens[2].type_, Token::Type::kNewline);
  EXPECT_EQ(tokens[3].token_, "a");
  EXPECT_TRUE(tokens[3].is_first_on_newline_);

  EXPECT_EQ(tokens[11].token_, "-");
  EXPECT_TRUE(tokens[11].is_first_on_newline_);

  EXPECT_EQ(tokens[14].token_, "-");
  EXPECT_TRUE(tokens[14].is_first_on_newline_);

  EXPECT_EQ(tokens[17].token_, "-");
  EXPECT_TRUE(tokens[17].is_first_on_newline_);

  /* indices of tokens that start at a new line */
  std::vector<size_t> newline_indices = {0, 3, 7, 11, 14, 17};

  /* check that certain indices are indeed newlines */
  for (auto index : newline_indices)
    EXPECT_TRUE(tokens[index].is_first_on_newline_)
      << "token with value "
      << tokens[index]
      << " was not marked as first in line";

  /* any other token not inside the defined indices should not be first in
   * newline
   */
  for (size_t i = 0; i < tokens.size(); ++i) {
    if (std::any_of(
          std::begin(newline_indices),
          std::end(newline_indices),
          [&i](const decltype(newline_indices[0])& val) { return val == i; }))
      continue;

    EXPECT_FALSE(tokens[i].is_first_on_newline_);
  }

  {
    std::stringstream output;
    parser.Write(output);
    std::cout << output.str() << std::endl;
  }
}

