#ifndef CACOPHAGY_PSYCAL_EVENT_H_
#define CACOPHAGY_PSYCAL_EVENT_H_

#include <chrono>
#include <functional>
#include <string>
#include <vector>

namespace psy::psycal {
  class Event {
    /**
     * format:
     * - current is a form of an accumulator, which is a min heap
     * - past is pruning the heap, storing, and compressing the
     *   information if possible for future inquiries
     */
  public:
    Event(std::tm&&, std::string&&);
    std::uint64_t GetUnixTimestamp() const;
    inline const std::tm& GetRawTime() const { return tm_; }
    std::vector<std::uint8_t> GetEncodedMessage() const;
    inline const std::string& GetMessage() const { return message_; }

    static bool SoonerCmp(const Event& a, const Event& b) {
      return a.GetUnixTimestamp() < b.GetUnixTimestamp();
    };

    typedef std::function<bool(const Event&, const Event&)> Comparator;
  private:
    std::tm tm_;
    std::string message_;
  };

  // Event::Comparator SoonerEventCmp = [](const Event& a, const Event& b) noexcept -> bool {
  //   return a.GetUnixTimestamp() < b.GetUnixTimestamp();
  // };
}

#endif
