#include "mac.h"

namespace psy::uuid {
  std::uint64_t MacStringToInt(const std::string& mac_string)  {
    const std::string delim = ":";
    std::uint64_t ret = 0;

    auto start = 0U;
    auto end =  mac_string.find(delim);
    while (end != std::string::npos) {
      const std::string octet = mac_string.substr(start, end - start);
      ret = (ret << 8) | std::stoul(octet, 0, 16);

      start = end + delim.length();
      end = mac_string.find(delim, start);
    }

    if (start < mac_string.size() - 1) {
      const std::string octet = mac_string.substr(start, mac_string.size() - 1);
      ret = (ret << 8) | std::stoul(octet, 0, 16);
    }

    return ret;
  }
}
