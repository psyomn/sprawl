#include "utils.h"

#include <sstream>

namespace psy::gamejam {
  std::optional<size_t> ParseInt(std::istream& mis) {
    char c = mis.get();

    if (c != ':') return std::nullopt;
    if (mis.peek() == '\n' || mis.peek() == EOF) return std::nullopt;

    std::stringstream buff;
    while (mis.peek() != ':' && (c = mis.get()) != '\n' && c != EOF)
      buff << c;

    size_t temp_i = 0;
    buff >> temp_i;

    return temp_i;
  };

  std::optional<std::string> ParseStr(std::istream& mis) {
    std::stringstream buff;

    char c = mis.get();

    if (c != ':') return std::nullopt;
    if (mis.peek() == '\n' || mis.peek() == EOF) return std::nullopt;

    while ((c = mis.get()) != '\n' && c != EOF)
      buff << c;

    return buff.str();
  };
}
