#include "clock.h"

namespace psy::uuid {
  using HRC = std::chrono::high_resolution_clock;

  std::chrono::time_point<HRC> Clock::Now() const {
    return clock_.now();
  }
}
