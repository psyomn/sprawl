#include "server.h"
#include "message.h"

#include <fstream>

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
        event_timepoint =
          sc::system_clock::time_point{
          sc::seconds{events_.top().GetUnixTimestamp()}
        };

        old_.push_back(std::move(events_.top()));

        events_.pop();
        popped_event = true;
      }

      if (popped_event) {
        make_snapshot_ = true;
        snapshot_cv_.notify_one();
      }
    }
  }

  void SaveOld(const std::vector<Event>& olds);
  void SaveOld(const std::vector<Event>& olds) {
    std::ofstream of("data", std::ofstream::binary | std::ofstream::app | std::ofstream::out);
    for (const auto& el : olds) {
        of << el.GetUnixTimestamp();
    }
  }

  void Server::Snapshot() {
    while (run_server_) {
      std::unique_lock<std::mutex> lk(lock_);
      snapshot_cv_.wait(lk, [this]{return make_snapshot_ == true;});

      events_;
      SaveOld(old_);

      std::vector<Event>().swap(old_);
      make_snapshot_ = false;
    }
  }
}
