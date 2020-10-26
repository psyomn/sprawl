#ifndef CACOPHAGY_RFC4122_MAC_H_
#define CACOPHAGY_RFC4122_MAC_H_

#include <cstdint>
#include <string>

namespace psy::uuid {
  std::uint64_t MacStringToInt(const std::string& mac_string);
}

#endif
