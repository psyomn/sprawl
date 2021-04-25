#pragma once

#include <cstdint>
#include <deque>
#include <string>

namespace psy::ds {
  class BigInt {
  public:
    explicit BigInt(const std::string& str);
    explicit BigInt(std::deque<std::uint8_t>&& digits);
    const std::deque<std::uint8_t>& GetDigits() const;
    std::string ToString() const;

    BigInt operator+(const BigInt& other) const;
  private:
    std::deque<std::uint8_t> digits_;
    std::uint8_t sign_;
  };
}
