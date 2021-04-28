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
#include "granulator.h"

namespace psy::common {
  // this returns _inclusive_ ranges, so your loops must be from
  std::pair<size_t, size_t> Granulator::Step() {
    const size_t from = step_ * grain_;
    size_t to = (step_ + 1) * grain_ - 1;

    if (to + threshold_ >= num_elements_)
      to = num_elements_ - 1;

    auto pair = std::make_pair(from, to);

    ++step_;

    return pair;
  }
}
