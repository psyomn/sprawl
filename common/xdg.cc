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
#include <unistd.h>

#include "xdg.h"

namespace psy::common::XDG {
  std::optional<std::filesystem::path> GetHomePath() {
    char* home = getenv("HOME");
    if (home == nullptr) return std::nullopt;
    return std::string(home);
  }

  std::optional<std::filesystem::path> GetConfigPath() {
    auto home_path = GetHomePath();
    if (!home_path) return std::nullopt;
    return home_path.value() / std::filesystem::path(".config");
  }

  std::optional<std::filesystem::path> MakeApplicationPath(const std::string& name) {
    auto config_path = GetConfigPath();
    if (!config_path) return std::nullopt;
    return config_path.value() / std::filesystem::path(name);
  }
}
