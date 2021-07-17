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
#include <cstdint>
#include <iostream>
#include <optional>
#include <vector>

#include <stddef.h>
#include <unistd.h>

#include "utils.h"
#include "psycal.h"

namespace psycal {
struct Session {
  bool foreground;
  std::int64_t timestamp;
  std::optional<std::tm> maybe_timestamp;

  void PrintUsage(const char* name) {
    std::cout << "usage:" << std::endl
              << "  " << name  << std::endl
              << "  -d daemonize" << std::endl
              << "  -f foreground" << std::endl
              << "  -t add event" << std::endl;
  }

  void PrintUsageAndExit(const char* name,
                         int exit_code) {
    this->PrintUsage(name);
    exit(exit_code);
  }
};
}

int main(int argc, char *argv[]) {
  psycal::Utils::CreateApplicationDirectories();

  psycal::Session session = {0};
  int opt = 0;

  while ((opt = getopt(argc, argv, "hft:")) != -1) {
    switch (opt) {
    case 'd':
      session.foreground = false;
      break;
    case 'f':
      session.foreground = true;
      break;
    case 't':
      session.maybe_timestamp = psycal::Utils::ProcessTimestamp(optarg);
      break;
    case 'h':
      session.PrintUsageAndExit(argv[0], EXIT_SUCCESS);
      break;
    default:
      session.PrintUsageAndExit(argv[0], EXIT_FAILURE);
      break;
    }
  }

  if (optind >= argc) {
    std::cerr << "expected argument after options" << std::endl;
    session.PrintUsage(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (!session.maybe_timestamp.has_value()) {
    std::cerr << "invalid timestamp" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<std::string> words;
  for (int i = optind; i < argc; ++i) words.push_back(argv[optind]);

  psycal::Psycal cal(std::move(session.maybe_timestamp.value()),
                     std::move(words));

  return 0;
}
