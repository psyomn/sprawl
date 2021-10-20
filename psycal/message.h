#ifndef SPRAWL_PSYCAL_MESSAGE_H_
#define SPRAWL_PSYCAL_MESSAGE_H_
#include <cstdint>
#include <vector>

#include "event.h"
namespace psy::psycal::Message {
  std::vector<std::uint8_t> IntoBuffer(const std::vector<Event>& event);
  std::vector<Event> FromBuffer(const std::uint8_t buffer[], const size_t size);
}
#endif /* SPRAWL_PSYCAL_MESSAGE_H_ */
