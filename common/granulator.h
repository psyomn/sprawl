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
#ifndef SPRAWL_COMMON_GRANULATOR_H_
#define SPRAWL_COMMON_GRANULATOR_H_

#include <cstddef>
#include <utility>
#include <thread>

namespace psy::common {
  /**
   * a glorified calculator
   *
   * should take a number of elements and suggest best partitioning
   * between threads
   */
  class Granulator {
  public:
    explicit Granulator(size_t num_elements) :
      threshold_(128),
      num_elements_(num_elements),
      num_threads_(std::thread::hardware_concurrency()),
      grain_(num_elements_ / num_threads_),
      step_(0)
    {
      if (num_elements_ <= num_threads_ * threshold_) {
        // to trigger a sane work sharing environment, we'll make sure
        // that each thread has at least "threshold_" elements to deal
        // with
        num_threads_ = 1;
        grain_ = num_elements_;
      }
    }

    inline size_t GetNumThreads() const noexcept { return num_threads_; }
    inline size_t GetGrain() const noexcept { return grain_; }
    inline void ResetSteps(void) { step_ = 0; }

    // this returns _inclusive_ ranges, so your loops must be from
    std::pair<size_t, size_t> Step();

  private:
    size_t threshold_;
    size_t num_elements_;
    size_t num_threads_;
    size_t grain_;
    size_t step_;
  };

}

#endif
