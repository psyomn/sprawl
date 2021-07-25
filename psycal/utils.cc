#include "utils.h"

#include "common/filesystem.h"

namespace psy::psycal::Utils {
  std::optional<std::tm> ProcessTimestamp(const char* timestamp) {
    std::tm tm = {0};

    char* try1 = ::strptime(timestamp, "%Hh%M", &tm);
    if (try1 != NULL) {
      const time_t now = time(nullptr);
      std::tm merge = {0};
      (void) localtime_r(&now, &merge);

      merge.tm_hour = tm.tm_hour;
      merge.tm_min = tm.tm_min;
      merge.tm_sec = 0;

      return merge;
    }

    char* try2 = ::strptime(timestamp, "%Y-%m-%dT%Hh%M", &tm);
    if (try2 != NULL) return tm;

    return std::nullopt;
  }

  std::optional<std::string> CreateApplicationDirectories() {
    std::optional<std::string> ret = std::nullopt;

    if (!psy::common::filesystem::DirExists("/")) {
      ret = "creating application directory in ... ";
    }

    return ret;
  }
}
