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
#include <iterator>
#include <stdexcept>
#include <sstream>

#include <algorithm>

#include "event.h"

#include "common/codec.h"

namespace psy::psycal {
  Event::Event(std::tm&& tm, std::vector<std::string>&& words) :
    tm_(std::move(tm)),
    words_(std::move(words)) {}

  std::uint64_t Event::GetUnixTimestamp() const {
    std::tm copy = tm_;
    auto ret = mktime(&copy);
    if (ret == -1) throw std::logic_error("mktime returned error");
    if (ret < -1) throw std::domain_error("we are not logging things before 1970");
    return std::uint64_t(ret);
  }

  std::vector<std::uint8_t> Event::GetEncodedMessage() const {
    std::vector<std::uint8_t> ret;

    std::size_t each_word_size = 0;
    std::for_each(words_.begin(), words_.end(),
                  [&](const std::string& s){ each_word_size += s.size(); });

    const std::size_t size = words_.size() - 1 + each_word_size;

    psy::common::Codec::EncodeU64LE(std::uint64_t(size), ret);

    {
      auto it = words_.begin();
      for (; it != words_.end() - 1; ++it) {
        std::copy(std::begin(*it), std::end(*it), std::back_inserter(ret));
        ret.push_back(' ');
      }

      std::copy(std::begin(*it), std::end(*it), std::back_inserter(ret));
    }

    return ret;
  }

  const std::vector<std::string>& Event::GetWords() const {
    return words_;
  }
}
