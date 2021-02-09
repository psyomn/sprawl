#include "repl.h"

#include <iostream>

namespace psy::tinydb {
  void REPL::Run() {
    std::string input;

    std::cout << "tinydb> ";
    while (std::cin >> input) {
      if (input == "exit") break;

      ProcessInput(input);
      std::cout << "tinydb> ";
    }
  }

  void REPL::ProcessInput(const std::string& input) {
    if (input == "tables") {
      std::cout << "table information goes here" << std::endl;
    } else if (input == "help") {
      std::cout << "tables - prints tables" << std::endl
                << "help - prints this" << std::endl
                << "exit - quit repl" << std::endl;
    } else {
      std::cout << "bad command" << std::endl;
    }
  }
}
