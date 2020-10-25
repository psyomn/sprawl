#ifndef CACOPHAGY_RFC4122_UUID_CLOCK_H_
#define CACOPHAGY_RFC4122_UUID_CLOCK_H_

#include "common.h"

#include <chrono>
#include <memory>

namespace psy::uuid {
  class Clock {
    using HRC = std::chrono::high_resolution_clock;
  public:
    explicit Clock(RunType type) : type_(type), clock_() {};
    std::chrono::time_point<HRC> Now() const;
  private:
    enum RunType type_;
    std::chrono::high_resolution_clock clock_;
    std::uint64_t fake_time_;
  };
}

#endif /* CACOPHAGY_RFC4122_UUID_H_ */
