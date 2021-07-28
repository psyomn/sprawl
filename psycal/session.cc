#include "session.h"
#include "server.h"
#include "message.h"
#include "utils.h"

#include <sstream>

namespace psy::psycal {
  void Session::PortStrIntoInt(const char* opt) {
    std::stringstream ss(opt);
    ss >> port_;
  }

  void Session::PrintUsage(const char* name) const {
    std::cout << "usage:" << std::endl
              << "  " << name  << std::endl
              << "  -d daemonize" << std::endl
              << "  -f foreground" << std::endl
              << "  -s server mode" << std::endl
              << "  -p port " << std::endl
              << "  -t add event" << std::endl;
  }

  void Session::PrintUsageAndExit(const char* name, const int exit_code) const {
    this->PrintUsage(name);
    exit(exit_code);
  }

  void Session::StartServer(const std::uint16_t port) {
    psy::psycal::Server server(port);
    server.Start();
  }

  void Session::SendEvent(const psy::psycal::Event& event, const Session& session) const {
    namespace pp = psy::psycal;
    auto buffer = pp::Message::IntoBuffer(std::vector<pp::Event>{event});

    psy::net::UDPClient client(session.host_, session.port_);

    client.Send(buffer.data());
    if (client.Errored()) std::cerr << "error sending message" << std::endl;
  }

  void Session::MakeDirs() const {
    namespace u = psy::psycal::Utils;

    std::string message;

    switch (u::CreateApplicationDirectories()) {
    case u::CreateAppDirStatus::OkCreate:
      message = "creating";
      break;
    case u::CreateAppDirStatus::OkExists:
      message = "exists";
      break;
    case u::CreateAppDirStatus::ErrGenConfigPath:
      message = "error generating config path -- do you have a home?";
      break;
    case u::CreateAppDirStatus::Error:
      message = "general error";
      break;
    default:
      std::cerr << "and in strange aeons even death might die" << std::endl;
      abort();
    }
  }
}
