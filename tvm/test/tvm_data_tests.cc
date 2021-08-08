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

#include "test.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

TEST(tvm_data, tokenize_simple) {
  std::filesystem::path file = kFixtures / "simple.asm";
  std::ifstream input(file);
  if (!input.is_open()) FAIL();

  auto tokens = psy::tvm::Tokenize(input);

  for (auto& tok : tokens)
    EXPECT_NE(tok.TypeString(), "unknown") << tok.value_ << tok.line_number_;
}

TEST(tvm_data, tokenize_hello_world) {
  std::filesystem::path file = kFixtures / "hello-world.asm";
  std::ifstream input(file);
  if (!input.is_open()) FAIL();

  auto tokens = psy::tvm::Tokenize(input);

  for (auto& tok : tokens)
    EXPECT_NE(tok.TypeString(), "unknown") << tok.value_ << tok.line_number_;
}

TEST(tvm_data, parse_hello_world) {
  std::filesystem::path file = kFixtures / "hello-world.asm";
  std::ifstream input(file);
  if (!input.is_open()) FAIL();

  auto tokens = psy::tvm::Tokenize(input);
  auto instruction_set = psy::tvm::Parse(tokens);

  std::cout << instruction_set << std::endl;
}

TEST(tvm_data, parse_arithmetic) {
  std::filesystem::path file = kFixtures / "arithmetic.asm";
  std::ifstream input(file);
  if (!input.is_open()) FAIL();

  auto tokens = psy::tvm::Tokenize(input);
  auto instruction_set = psy::tvm::Parse(tokens);

  std::cout << instruction_set << std::endl;
}

TEST(tvm_data, DISABLED_parse_bad_no_ORIG) {}

TEST(tvm_data, parse_many_variables) {
  std::filesystem::path file = kFixtures / "many-variables.asm";
  std::ifstream input(file);
  if (!input.is_open()) FAIL();

  auto tokens = psy::tvm::Tokenize(input);
  auto instruction_set = psy::tvm::Parse(tokens);

  std::cout << instruction_set << std::endl;
}
