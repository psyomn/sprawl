#pragma once

#include <string>

namespace psy::tinydb {
  class REPL {
  public:
    explicit REPL(const std::string label) : label_(label) {}
    ~REPL(){}
    REPL(const REPL& other) = delete;
    REPL(REPL&& other) = delete;
    REPL& operator=(REPL&& other) = delete;

    void Run();

  private:
    std::string label_;

    void ProcessInput(const std::string& input);
  };
}
