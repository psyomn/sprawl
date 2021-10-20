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
#ifndef SPRAWL_PSYCAL_SESSION_H_
#define SPRAWL_PSYCAL_SESSION_H_

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

#include <cstdint>
#include <ctime>

#include "event.h"

#include "common/xdg.h"

namespace psy::psycal {
  struct Session {
    Session() :
      foreground_(false), timestamp_(0),
      maybe_timestamp_(std::nullopt), server_mode_(false),
      port_(9995), host_("127.0.0.1"), dump_filepath_() { MakeDirs(); }

    bool foreground_;
    std::int64_t timestamp_;
    std::optional<std::tm> maybe_timestamp_;
    bool server_mode_;
    std::uint16_t port_;
    std::string host_;
    std::string dump_filepath_;

    std::filesystem::path old_events_file_;
    std::filesystem::path upcoming_events_file_;

    void PortStrIntoInt(const char* opt);
    void PrintUsage(const char* name) const;
    void PrintUsageAndExit(const char* name, const int exit_code) const;
    void StartServer(const std::uint16_t port);
    void SendEvent(const psy::psycal::Event& event, const Session& session) const;

    void DumpEvents(std::string path) const;

    void MakeDirs() const;
  };
}
#endif /* SPRAWL_PSYCAL_SESSION_H_ */
