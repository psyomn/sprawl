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
