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

#include "tvm/tvm.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

const std::filesystem::path kFixtures = "tvm/test/data";

TEST(tvm, token_int) {
  using psy::tvm::Token;
  EXPECT_EQ(Token("#0", 0).type_, Token::Type::NumberLiteral) << "#0";
  EXPECT_EQ(Token("#1", 0).type_, Token::Type::NumberLiteral) << "#1";
  EXPECT_EQ(Token("#123", 0).type_, Token::Type::NumberLiteral) << "#123";
  EXPECT_EQ(Token("#12341234", 0).type_, Token::Type::NumberLiteral) << "#12341234";
  EXPECT_EQ(Token("1", 0).type_, Token::Type::NumberLiteral) << "1";
  EXPECT_EQ(Token("#-1", 0).type_, Token::Type::NumberLiteral) << "#-1";
  EXPECT_EQ(Token("#-0", 0).type_, Token::Type::NumberLiteral) << "#-0";
  EXPECT_EQ(Token("#-1234", 0).type_,Token::Type::NumberLiteral) << "#-1234";
}

TEST(tvm, token_label) {
  using psy::tvm::Token;
  EXPECT_EQ(Token("HW", 0).type_, Token::Type::Label);
  EXPECT_EQ(Token("THELABEL", 0).type_, Token::Type::Label);
  EXPECT_EQ(Token("THE_LABEL", 0).type_, Token::Type::Label);
}

TEST(tvm, token_register) {
  using psy::tvm::Token;
  EXPECT_EQ(Token("R0", 0).type_, Token::Type::Register);
  EXPECT_EQ(Token("R1", 0).type_, Token::Type::Register);
  EXPECT_EQ(Token("R2", 0).type_, Token::Type::Register);
  EXPECT_EQ(Token("R3", 0).type_, Token::Type::Register);

  EXPECT_NE(Token("R5", 0).type_, Token::Type::Register);
  EXPECT_NE(Token("R6", 0).type_, Token::Type::Register);
  EXPECT_NE(Token("R7", 0).type_, Token::Type::Register);
  EXPECT_NE(Token("R8", 0).type_, Token::Type::Register);
}

TEST(tvm, token_segment) {
  using psy::tvm::Token;
  EXPECT_EQ(Token(".HELLO", 0).type_, Token::Type::Segment);
  EXPECT_EQ(Token(".NOPE", 0).type_, Token::Type::Segment);
}

TEST(tvm, token_hex) {
  using psy::tvm::Token;
  EXPECT_EQ(Token("x0000", 0).type_, Token::Type::HexNumber);
  EXPECT_EQ(Token("x0001", 0).type_, Token::Type::HexNumber);
  EXPECT_EQ(Token("x1234", 0).type_, Token::Type::HexNumber);
  EXPECT_EQ(Token("xFFFF", 0).type_, Token::Type::HexNumber);
  EXPECT_EQ(Token("xffff", 0).type_, Token::Type::HexNumber);
}

TEST(tvm, token_instruction) {
  using psy::tvm::Token;

  const std::vector<std::string> instructions = {
    "AND","ADD",
    "BR","BRn","BRzp","BRnz","BRp","BRz","BRnp",
    "HALT",
    "LD","LEA","LDR",
    "PUTS",
    "ST",
    "NOT",
  };

  for (const auto& el : instructions)
    EXPECT_EQ(Token(std::string(el), 0).type_, Token::Type::Instruction);
}

TEST(tvm, tokenize_string) {
  using psy::tvm::Token;
  const auto input = {
    "\"hello\"",
    "\"hello0there\"",
    "\"hello there\"",
    "\"hello, there!\"",
    "\"hello, there! how are you today, number 199231!? hope you're good; otherwise...\"",
  };

  for (const auto& el : input)
    EXPECT_EQ(Token(std::string(el), 0).type_, Token::Type::String) << el;
}

TEST(tvm, utils_as_sr1) {
  EXPECT_EQ(0b0000'000'001'000000, psy::tvm::AsSR1(1));
  EXPECT_EQ(0b0000'000'010'000000, psy::tvm::AsSR1(2));
  EXPECT_EQ(0b0000'000'011'000000, psy::tvm::AsSR1(3));
  EXPECT_EQ(0b0000'000'100'000000, psy::tvm::AsSR1(4));
}

TEST(tvm, utils_as_dr1) {
  EXPECT_EQ(0b0000'001'000'000000, psy::tvm::AsDR(1));
  EXPECT_EQ(0b0000'010'000'000000, psy::tvm::AsDR(2));
  EXPECT_EQ(0b0000'011'000'000000, psy::tvm::AsDR(3));
  EXPECT_EQ(0b0000'100'000'000000, psy::tvm::AsDR(4));
}
