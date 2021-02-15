#pragma once

#include <string>

namespace psy::tinydb {
  class Error {
  public:
    explicit Error(std::string message) : message_(message) {}
    std::string GetMessage() const noexcept { return message_; }
  private:
    std::string message_;
  };
}
