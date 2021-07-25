#include "server.h"
#include "message.h"

namespace psy::psycal {
  void Server::Start() {
    auto constexpr max_sz = psy::net::kMaxUDPSize;
    auto listen_fn = [this](std::uint8_t bf[max_sz]) noexcept -> void {
      std::lock_guard guard(lock_);

      for (const auto& received_event : Message::FromBuffer(bf, max_sz)) {
        events_.push(std::move(received_event));
      }
    };

    server_.ListenWith(listen_fn);
  }

  void Server::Tick() {
    namespace sc = std::chrono;

    while (true) {
      std::this_thread::sleep_for(sc::seconds(1));

      std::lock_guard guard{lock_};
      if (events_.size() == 0) continue;

      const sc::time_point<sc::system_clock> now = sc::system_clock::now();

      sc::system_clock::time_point event_timepoint{sc::seconds{events_.top().GetUnixTimestamp()}};

      while (events_.size() > 0 && now > event_timepoint) {
        event_timepoint =
          sc::system_clock::time_point{
          sc::seconds{events_.top().GetUnixTimestamp()}
        };

        old_.push_back(std::move(events_.top()));

        events_.pop();
      }

    }
  }
}
