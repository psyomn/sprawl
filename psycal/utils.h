#include <ctime>
#include <filesystem>
#include <optional>
#include <string>

namespace psy::psycal::Utils {
  const char * const kApplicationName = "psycal";
  std::optional<std::tm> ProcessTimestamp(const char* timestamp);

  enum class CreateAppDirStatus {OkCreate, OkExists, ErrGenConfigPath, Error};
  CreateAppDirStatus CreateApplicationDirectories();

  std::filesystem::path ApplicationPath();
  std::filesystem::path HistoryFilePath();
  std::filesystem::path EventsFilePath();
}
