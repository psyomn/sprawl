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
#include <string>
#include <vector>
#include <sstream>

#include "utils.h"

namespace psy::pshy {
  std::vector<std::string> IntoTokens(const std::string& line) {
    const std::string delimiters = " \t";

    std::vector<std::string> ret;
    std::stringstream ss;
    for (auto c : line) {
      auto isDelimited = delimiters.find_first_of(c) != std::string::npos;

      if (isDelimited && ss.str().size() > 0) {
        ret.push_back(ss.str());
        ss.str("");
      }

      if (!isDelimited)
        ss << c;
    }

    if (ss.str().size() > 0)
      ret.push_back(ss.str());

    return ret;
  }
}
