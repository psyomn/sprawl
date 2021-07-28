#include "utils.h"

#include "common/filesystem.h"
#include "common/xdg.h"

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

  CreateAppDirStatus CreateApplicationDirectories() {
    const auto app_path = psy::common::XDG::MakeApplicationPath(kApplicationName);
    if (!app_path) return CreateAppDirStatus::ErrGenConfigPath;
    if (common::filesystem::DirExists(app_path.value())) return CreateAppDirStatus::OkExists;

    common::filesystem::MkdirP(app_path.value());

    return CreateAppDirStatus::OkCreate;
  }
}
