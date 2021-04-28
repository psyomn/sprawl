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
