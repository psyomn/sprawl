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
