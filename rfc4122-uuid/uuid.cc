#include "uuid.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <sstream>

#include <iostream> // TODO

namespace psy::uuid {
  std::unique_ptr<UUID> FromString(const std::string& str) {
    if (str.find_first_not_of("0123456789abcdef-") != std::string::npos)
      throw std::runtime_error("non uuid character in: " + str);

    // 36 bytes (128 bits -> 2 x 64bits, 32 bytes in string format +
    // 4x '-' character)
    if (str.size() != 36)
      throw std::runtime_error("malformed uuid: " + str);

    if (str[8] != '-' || str[13] != '-' || str[18] != '-' || str[23] != '-')
      throw std::runtime_error("malformed uuid (check hyphens): " + str);

    std::array<std::uint64_t, 2> arr = {0, 0};

    std::size_t count = 0;
    auto it = str.begin();
    while (it != str.end()) {
      if (*it == '-') {
        it++;
        continue;
      }

      std::string curr;
      curr += *it;
      it++;
      curr += *it;
      it++;

      const std::uint8_t value = std::stoull(curr, 0, 16);

      const size_t idx = count >= 8 ? 1 : 0;
      arr[idx] <<= 8;
      arr[idx] |= value;

      ++count;
    }
    return std::make_unique<UUID>(Method::name, std::move(arr));
  }

  std::string UUID::String() const {
    std::stringstream ss;
    ss << std::hex
       << std::setfill('0')
       << std::setw(16)
       << uuid_[0];

    ss << std::hex
       << std::setfill('0')
       << std::setw(16)
       << uuid_[1];

    std::string ret = ss.str();
    ret.insert(8, "-");
    ret.insert(8+4+1, "-");
    ret.insert(8+4+4+2, "-");
    ret.insert(8+4+4+4+3, "-");

    return ret;
  }

  bool operator< (const UUID& uuid1, const UUID& uuid2) {
    const auto raw1 = uuid1.Raw();
    const auto raw2 = uuid2.Raw();
    return raw1[0] < raw2[0] && raw1[1] < raw2[1];
  }

  bool operator> (const UUID& uuid1, const UUID& uuid2) {
    const auto raw1 = uuid1.Raw();
    const auto raw2 = uuid2.Raw();
    return raw1[0] > raw2[0] && raw1[1] > raw2[1];
  }

  bool operator== (const UUID& uuid1, const UUID& uuid2) {
    const auto raw1 = uuid1.Raw();
    const auto raw2 = uuid2.Raw();
    return raw1[0] == raw2[0] && raw1[1] == raw2[1];
  }

  bool operator!= (const UUID& uuid1, const UUID& uuid2) {
    return !(uuid1 == uuid2);
  }

  bool operator<=(const UUID& uuid1, const UUID& uuid2) {
    const auto raw1 = uuid1.Raw();
    const auto raw2 = uuid2.Raw();
    return raw1[0] <= raw2[0] && raw1[1] <= raw2[1];
  }

  bool operator>= (const UUID& uuid1, const UUID& uuid2) {
    const auto raw1 = uuid1.Raw();
    const auto raw2 = uuid2.Raw();
    return raw1[0] >= raw2[0] && raw1[1] >= raw2[1];
  }

  std::unique_ptr<UUID> Generator::Generate(const Method& m) {
    switch(m) {
    case Method::time:
      break;
    case Method::name:
      break;
    case Method::rand:
      break;
    case Method::nil:
      return std::make_unique<UUID>(Method::nil, std::move(GenMAC()));
    default:
      throw std::runtime_error("can not generate unknown type");
    }

    return std::make_unique<UUID>(Method::nil, std::move(Nil()));
  }

  std::array<std::uint64_t, 2> Generator::GenMAC() {
    // right now only support linux, or anything that has sysfs
    //   /sys/class/net/wlp3s0/address
    std::uint64_t hi = 0, lo = 0;

    // TODO: this might bet better to be passed through a function for
    //   easier testing.
    auto now =
      std::chrono::high_resolution_clock::now().time_since_epoch();
    auto nanonow =
      std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

    std::cout << nanonow << std::endl;
    return {hi, lo};
  }

  std::array<std::uint64_t, 2> Generator::GenRand() {
    std::uint64_t hi = 0, lo = 0;
    return {hi, lo};
  }

  std::array<std::uint64_t, 2> Generator::GenHash() {
    std::uint64_t lo = 0, hi = 0;
    return {hi, lo};
  }

  std::array<std::uint64_t, 2> Generator::Nil() { return {0, 0}; }
}
