#pragma once

#include "page.h"

namespace psy::tinydb {
  /* Page table */
  class Table {
  public:
    void Write() noexcept;
  private:
    std::vector<Page> pages_;
  };
}
