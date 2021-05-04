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

    return BigInt(std::move(ret));
  }

  std::string BigInt::ToString() const {
    std::stringstream ss;
    for (auto it = digits_.cbegin(); it != digits_.cend(); ++it)
      ss << int(*it);
    return ss.str();
  }
}
