#ifndef CACOPHAGY_PSYCAL_H_
#define CACOPHAGY_PSYCAL_H_

#include <ctime>
#include <string>
#include <vector>

namespace psycal {
  /**
   * format:
   * - current is a form of an accumulator, which is a min heap
   * - past is pruning the heap, storing, and compressing the
   *   information if possible for future inquiries
   */
  class Psycal {
  public:
    Psycal(std::tm&&, std::vector<std::string>&&);
  private:
    std::tm tm_;
    std::vector<std::string> words_;
  };
}

#endif
