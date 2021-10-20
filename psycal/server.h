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
#ifndef SPRAWL_PSYCAL_SERVER_H_
#define SPRAWL_PSYCAL_SERVER_H_

#include <string>
#include <cstdint>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <thread>

#include "event.h"

#include "net/udp.h"

namespace psy::psycal {
  class Server {
  public:
    explicit Server(const std::uint16_t port) :
      server_(port),
      events_(Event::SoonerCmp),
      worker_(&Server::Tick, this),
      worker_snapshot_(&Server::Snapshot, this),
      old_({}),
      run_server_(true),
      snapshot_cv_({}),
      make_snapshot_(false) {}

    void Start();
    void MakeDirs() const;
  private:
    void Save();
    void Tick(); // TODO evaluate use of waitqueue style DS
    void Snapshot();

    psy::net::UDPListener server_;

    std::priority_queue<Event, std::vector<Event>, Event::Comparator> events_;

    std::thread worker_;
    std::thread worker_snapshot_;
    std::mutex lock_;
    std::vector<Event> old_;
    std::atomic<bool> run_server_;
    std::condition_variable snapshot_cv_;
    std::atomic<bool> make_snapshot_;
  };
}

#endif /* SPRAWL_PSYCAL_SERVER_H_ */
