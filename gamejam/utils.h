#ifndef _SPRAWL_GAMEJAM_UTILS_H
#define _SPRAWL_GAMEJAM_UTILS_H

#include <cstddef>
#include <optional>
#include <istream>

namespace psy::gamejam {
  std::optional<size_t> ParseInt(std::istream& mis);
  std::optional<std::string> ParseStr(std::istream& mis);
}

#endif
