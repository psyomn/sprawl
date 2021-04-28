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
#include "repl.h"

#include <iostream>

namespace psy::tinydb {
  void REPL::Run() {
    std::string input;

    std::cout << "tinydb> ";
    while (std::cin >> input) {
      if (input == ".exit") break;

      ProcessInput(input);
      std::cout << "tinydb> ";
    }
  }

  enum REPL::CommandType REPL::GetCommandType(const std::string& cmd) const noexcept {
    if (cmd.at(0) == '.') return CommandType::Meta;
    return CommandType::SQL;
  }

  void REPL::ProcessInput(const std::string& input) {
    switch(GetCommandType(input)) {
    case CommandType::Meta:
        if (input == ".tables") {
          std::cout << "table information goes here" << std::endl;
        } else if (input == ".help") {
          std::cout << ".tables - prints tables" << std::endl
                    << ".help - prints this" << std::endl
                    << ".exit - quit repl" << std::endl;
        } else {
          std::cout << "bad command" << std::endl;
        }
      break;

    case CommandType::SQL:
      std::cout << "not implemented yet" << std::endl;
      break;

    default:
      std::cout << "unknown command type" << std::endl;
    }

  }
}
