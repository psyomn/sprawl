#include "bigint.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace psy::ds {
  BigInt::BigInt(const std::string& str) :
    digits_({}),
    sign_(0) {
    const auto check_str = str.find_first_not_of("0123456789");

    if (check_str != std::string::npos)
      throw std::domain_error("bigint input should be purely numeric");

    for (auto it = str.crbegin(); it != str.rend(); ++it)
      digits_.push_front((*it) - 0x30);
  }

  BigInt::BigInt(std::deque<std::uint8_t>&& digits) :
    digits_(std::move(digits)) {}

  const std::deque<std::uint8_t>& BigInt::GetDigits() const {
    return digits_;
  }

  BigInt BigInt::operator+(const BigInt& other) const {
    const BigInt& smaller =
      other.GetDigits().size() < this->GetDigits().size() ?
      other : *this;

    const BigInt& bigger =
      other.GetDigits().size() >= this->GetDigits().size() ?
      other : *this;

    size_t index = 0;
    uint8_t carry = 0;
    auto small_it = smaller.GetDigits().crbegin();
    auto bigger_it = bigger.GetDigits().crbegin();
    std::deque<std::uint8_t> ret;

    while (small_it != smaller.GetDigits().rend()) {
      std::uint8_t tmp_digit = *small_it + *bigger_it + carry;

      if (tmp_digit >= 10) {
        carry = 1;
        tmp_digit -= 10;
      }

      ret.push_front(tmp_digit);

      ++small_it;
      ++bigger_it;
    }

    while (bigger_it != bigger.GetDigits().crend() && carry > 0) {
      // exhaust carries
      std::uint8_t tmp_digit = *bigger_it + carry;
      carry = 0;

      if (tmp_digit >= 10) {
        carry = 1;
        tmp_digit -= 10;
      }

      ret.push_front(tmp_digit);

      ++bigger_it;
    }

    if (carry > 0) ret.push_front(1);

    return BigInt(std::move(ret));
  }

  std::string BigInt::ToString() const {
    std::stringstream ss;
    for (auto it = digits_.cbegin(); it != digits_.cend(); ++it)
      ss << int(*it);
    return ss.str();
  }
}
