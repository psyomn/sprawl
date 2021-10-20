/*
   Copyright 2021 Simon (psyomn) Symeonidis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef SPRAWL_RFC4122_UUID_H_
#define SPRAWL_RFC4122_UUID_H_

#include "common.h"
#include "clock.h"

#include <array>
#include <chrono>
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

  std::unique_ptr<UUID> FromString(const std::string& str);

  class Generator {
  public:
    explicit Generator(RunType run_type) :
      run_type_(run_type),
      clock_(run_type_) {}

    explicit Generator() :
      run_type_(RunType::kProduction),
      clock_(run_type_) {};

    std::unique_ptr<UUID> Generate(const Method& m);
    Clock& GetClock() { return clock_; }
  private:
    std::array<std::uint64_t, 2> GenMAC();
    std::array<std::uint64_t, 2> GenRand();
    std::array<std::uint64_t, 2> GenHash();
    std::array<std::uint64_t, 2> Nil();

    enum RunType run_type_;
    Clock clock_;
  };
}

#endif /* SPRAWL_RFC4122_UUID_H_ */
