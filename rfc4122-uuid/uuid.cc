#include "uuid.h"
#include "mac.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <ctime>

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

  static void WriteTimeLow(std::uint64_t& val, std::tm& time) {
  }

  static void WriteTimeMid(std::uint64_t& val, std::tm& time) {
  }

  static void WriteTimeHiAndVersion(std::uint64_t& val, std::tm& time, std::uint8_t version) {
  }

  static void WriteClockSeqLow(std::uint64_t& val, std::tm& time) {
  }

  static void WriteNode(std::uint64_t& val) {
  }

  std::array<std::uint64_t, 2> Generator::GenMAC() {
    std::uint64_t hi = 0, lo = 0;

    { // time and version writing
      const std::uint8_t version = 3;
      auto tval = std::tm(); // this is init material
      auto greg_tm = strptime("1582-10-15 00:00:00",
                              "%Y-%m-%d %H:%M:%S",
                              &tval); // this is init material
      std::time_t tt = std::mktime(&tval);

      namespace sc = std::chrono;
      const auto tp = sc::system_clock::from_time_t(tt);
      const auto now = sc::system_clock::now() - tp;
      auto nanonow = sc::duration_cast<sc::nanoseconds>(now).count() / 100;
      std::cout << nanonow << std::endl;
    }

    { // mac / name
      // TODO: move this in constructor initialization. this is a
      //   bottleneck to perf atm
      // TODO: right now only support linux, or anything that has sysfs
      //   eg: /sys/class/net/wlp3s0/address
      // TODO: need a better picking algo for the below (pick by
      //   looking at operate)
      std::ifstream ifile("/sys/class/net/wlp3s0/address");
      std::stringstream contents;
      contents << ifile.rdbuf();
      const std::uint64_t mac = MacStringToInt(contents.str());
    }

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
