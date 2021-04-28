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

#include <cstdint>
#include <vector>
#include <cstddef>

namespace psy::tinydb {
  class Page {
  public:
    Page() :
      data_(std::vector<std::uint8_t>(default_page_size_)),
      cursor_(0),
      write_offset_(0) {}

    ~Page() {}

    void Write() noexcept;
    std::size_t GetPageSize() const noexcept { return data_.capacity(); }
    static std::size_t GetDefaultPageSize() noexcept { return Page::default_page_size_; }

  private:
    // might be better to ask the OS what a better page size is
    static const std::size_t default_page_size_ = 4096;

    std::vector<std::uint8_t> data_;
    std::size_t cursor_;
    std::size_t write_offset_;
  };
}
