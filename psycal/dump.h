#ifndef CACOPHAGY_PSYCAL_DUMP_H_
#define CACOPHAGY_PSYCAL_DUMP_H_
#include <fstream>
#include <vector>
#include <queue>
#include "event.h"
namespace psy::psycal {
  std::vector<Event> ReadEventsFrom(std::istream& is);
  void SaveOld(const std::vector<Event>& olds);
  void SaveCurrentEvents(const std::priority_queue<
      Event,
      std::vector<Event>,
      Event::Comparator>& events);
}
#endif
