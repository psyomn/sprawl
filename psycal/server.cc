#include "server.h"
#include "message.h"

#include <iostream>

namespace psy::psycal {
  void Server::Start() {
    auto constexpr max_sz = psy::net::kMaxUDPSize;
    auto listen_fn = [this](std::uint8_t bf[max_sz]) noexcept -> void {
      std::lock_guard guard(lock_);

      for (const auto& received_event : Message::FromBuffer(bf, max_sz))
        events_.push(std::move(received_event));
    };

    server_.ListenWith(listen_fn);
  }

  #pragma GCC diagnostic ignored "-Wstrict-overflow"
  void Server::Tick() {
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));

      {
        if (events_.size() == 0) continue;

        std::lock_guard guard(lock_);

        const std::uint64_t now = std::uint64_t(time(0));
        while (events_.top().GetUnixTimestamp() > now) {
          std::cout << "processing event: "
                    << events_.top().GetWords().size()
                    << std::endl;
          events_.pop();
        }
      }

    }
  }
}
