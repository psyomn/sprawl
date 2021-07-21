#include <ctime>
#include <optional>
#include <string>

namespace psy::psycal {
  namespace Utils {
    std::optional<std::tm> ProcessTimestamp(const char* timestamp);
    std::optional<std::string> CreateApplicationDirectories();
  }
}
