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
#include <sstream>
#include <vector>

#include <stddef.h>
#include <unistd.h>

#include "utils.h"
#include "session.h"

int main(int argc, char *argv[]) {
  namespace pp = psy::psycal;
  pp::Utils::CreateApplicationDirectories();
  pp::Session session;

  int opt = 0;

  if (argc == 1) {
    std::cerr << "must provide arguments" << std::endl;
    session.PrintUsageAndExit(argv[0], EXIT_SUCCESS);
    return -1;
  }

  while ((opt = getopt(argc, argv, "pshft:")) != -1) {
    switch (opt) {
    case 'p':
      session.PortStrIntoInt(optarg);
      break;
    case 's':
      session.server_mode_ = true;
      break;
    case 'd':
      session.foreground_ = false;
      break;
    case 'f':
      session.foreground_ = true;
      break;
    case 't':
      session.maybe_timestamp_ = psy::psycal::Utils::ProcessTimestamp(optarg);
      break;
    case 'h':
      session.PrintUsageAndExit(argv[0], EXIT_SUCCESS);
      break;
    default:
      session.PrintUsageAndExit(argv[0], EXIT_FAILURE);
      break;
    }
  }

  if (session.server_mode_) {
    // TODO: make server return error to have something more
    //   informative here for debugging
    session.StartServer(session.port_);
    exit(EXIT_SUCCESS);
  }

  if (optind >= argc) {
    std::cerr << "expected argument after options" << std::endl;
    session.PrintUsage(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (!session.maybe_timestamp_.has_value()) {
    std::cerr << "invalid timestamp" << std::endl;
    exit(EXIT_FAILURE);
  }

  // TODO: remove this
  std::vector<std::string> words;
  for (int i = optind; i < argc; ++i) words.push_back(argv[optind]);

  session.SendEvent(psy::psycal::Event(std::move(session.maybe_timestamp_.value()),
                                       std::move(words)), session);

  return 0;
}
