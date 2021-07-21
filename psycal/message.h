#ifndef CACOPHAGY_PSYCAL_MESSAGE_H_
#define CACOPHAGY_PSYCAL_MESSAGE_H_
#include <cstdint>
#include <vector>

#include "event.h"
namespace psy::psycal::Message {
  std::vector<std::uint8_t> IntoBuffer(const std::vector<Event>& event);
  std::vector<Event> FromBuffer(std::uint16_t buffer[], size_t size);
}
#endif /* CACOPHAGY_PSYCAL_MESSAGE_H_ */
