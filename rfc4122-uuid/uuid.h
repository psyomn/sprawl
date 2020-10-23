#ifndef CACOPHAGY_RFC4122_H_
#define CACOPHAGY_RFC4122_H_

#include <array>
#include <cstdint>
#include <memory>
#include <string>

namespace psy::uuid {
  enum class Method { time, name, rand, nil };

  class UUID {
  public:
    /* should look like this:
       ffffffff-ffff-ffff-ffff-ffffffffffff */
    UUID(Method m, std::array<std::uint64_t, 2>&& arr)
      : method_(m),
        uuid_(std::move(arr))
    {}

    inline std::array<std::uint64_t, 2> Raw() const { return uuid_; }
    std::string String() const;
  private:
    enum Method method_;
    std::array<uint64_t, 2> uuid_;
  };

  bool operator< (const UUID& uuid1, const UUID& uuid2);
  bool operator> (const UUID& uuid1, const UUID& uuid2);
  bool operator== (const UUID& uuid1, const UUID& uuid2);
  bool operator!= (const UUID& uuid1, const UUID& uuid2);
  bool operator<= (const UUID& uuid1, const UUID& uuid2);
  bool operator>= (const UUID& uuid1, const UUID& uuid2);

  class Generator {
  public:
    static std::unique_ptr<UUID> FromString(const std::string& str);
    static std::unique_ptr<UUID> Generate(const Method& m);
  private:
    static std::array<std::uint64_t, 2> GenMAC();
    static std::array<std::uint64_t, 2> GenRand();
    static std::array<std::uint64_t, 2> GenHash();
    static std::array<std::uint64_t, 2> Nil();
  };
}

#endif /* CACOPHAGY_RFC4122_H_ */
