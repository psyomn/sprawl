#include <ctime>
#include <optional>
#include <string>

namespace psy::psycal {
  namespace Utils {
    const char * const kApplicationName = "psycal";
    std::optional<std::tm> ProcessTimestamp(const char* timestamp);

    enum class CreateAppDirStatus {OkCreate, OkExists, ErrGenConfigPath, Error};
    CreateAppDirStatus CreateApplicationDirectories();
  }
}
