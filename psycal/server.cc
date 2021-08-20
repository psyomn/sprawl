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
#include "dump.h"
#include "message.h"
#include "server.h"
#include "utils.h"

#include <fstream>
#include <locale>

namespace psy::psycal {
  void Server::Start() {
    auto constexpr max_sz = psy::net::kMaxUDPSize;
    auto listen_fn = [this](std::uint8_t bf[max_sz]) noexcept -> void {
      std::lock_guard guard(lock_);

      for (const auto& received_event : Message::FromBuffer(bf, max_sz))
        events_.push(std::move(received_event));

      make_snapshot_ = true;
      snapshot_cv_.notify_one();
    };

    /* load previous run's untriggered events */
    std::ifstream is(Utils::EventsFilePath(), std::fstream::binary);
    for (auto& el : ReadEventsFrom(is))
      events_.push(std::move(el));

    server_.ListenWith(listen_fn);
  }

  void Server::Tick() {
    namespace sc = std::chrono;

    while (run_server_) {
      std::this_thread::sleep_for(sc::seconds(1));

      std::lock_guard guard{lock_};
      if (events_.size() == 0) continue;

      const sc::time_point<sc::system_clock> now = sc::system_clock::now();

      sc::system_clock::time_point event_timepoint{sc::seconds{events_.top().GetUnixTimestamp()}};

      bool popped_event = false;
      while (events_.size() > 0 && now > event_timepoint) {
        old_.push_back(std::move(events_.top()));

        events_.pop();

        // TODO: libnotify or other hooks can/should be added here
        popped_event = true;

        // load next timepoint
        event_timepoint =
          sc::system_clock::time_point{
          sc::seconds{events_.top().GetUnixTimestamp()}
        };
      }

      if (popped_event) {
        make_snapshot_ = true;
        snapshot_cv_.notify_one();
      }
    }
  }

  void Server::Snapshot() {
    while (run_server_) {
      std::unique_lock<std::mutex> lk(lock_);
      snapshot_cv_.wait(lk, [this]{return make_snapshot_ == true;});

      SaveCurrentEvents(events_);
      SaveOld(old_);

      std::vector<Event>().swap(old_);
      make_snapshot_ = false;
    }
  }
}
