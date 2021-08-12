#ifndef CACOPHAGY_PSYCAL_DUMP_H_
#define CACOPHAGY_PSYCAL_DUMP_H_
#include <fstream>
#include <vector>
#include "event.h"
namespace psy::psycal {
  std::vector<Event> ReadEventsFrom(std::istream& is);
}
#endif
