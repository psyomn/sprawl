#include "psycal.h"

namespace psycal {
  Psycal::Psycal(std::tm&& tm, std::vector<std::string>&& words) :
    tm_(std::move(tm)),
    words_(std::move(words)) {}
}
