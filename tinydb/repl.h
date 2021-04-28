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
#pragma once

#include <string>

namespace psy::tinydb {
  class REPL {
  public:
    enum class CommandType { Meta, SQL };

    explicit REPL(const std::string label) : label_(label) {}
    ~REPL(){}
    REPL(const REPL& other) = delete;
    REPL(REPL&& other) = delete;
    REPL& operator=(REPL&& other) = delete;

    void Run();

  private:
    std::string label_;

    enum CommandType GetCommandType(const std::string& cmd) const noexcept;

    void ProcessInput(const std::string& input);
  };
}
