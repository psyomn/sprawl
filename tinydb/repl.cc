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
