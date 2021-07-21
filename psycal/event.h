#ifndef CACOPHAGY_PSYCAL_PSYCAL_H_
#define CACOPHAGY_PSYCAL_PSYCAL_H_

#include <ctime>
#include <string>
#include <vector>

namespace psy::psycal {
  /**
   * format:
   * - current is a form of an accumulator, which is a min heap
   * - past is pruning the heap, storing, and compressing the
   *   information if possible for future inquiries
   */
  class Event {
  public:
    Event(std::tm&&, std::vector<std::string>&&);
    std::uint64_t GetUnixTimestamp() const;
    std::vector<std::uint8_t> GetEncodedMessage() const;
  private:
    std::tm tm_;
    std::vector<std::string> words_;
  };
}

#endif /* CACOPHAGY_PSYCAL_PSYCAL_H_ */
