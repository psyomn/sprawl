#include "utils.h"

#include "common/filesystem.h"

namespace psy::psycal::Utils {
  std::optional<std::tm> ProcessTimestamp(const char* timestamp) {
    std::tm tm = {0};

    char* try1 = ::strptime(timestamp, "%Hh%m", &tm);
    if (try1 != NULL) return tm;

    char* try2 = ::strptime(timestamp, "%Y-%m-%dT%Hh%m", &tm);
    if (try2 != NULL) return std::nullopt;

    return tm;
  }

  std::optional<std::string> CreateApplicationDirectories() {
    std::optional<std::string> ret = std::nullopt;

    if (!psy::common::filesystem::DirExists("/")) {
      ret = "creating application directory in ... ";
    }

    return ret;
  }
}
