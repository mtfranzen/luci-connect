#ifndef LC2PP_CONNECTION_H
#define LC2PP_CONNECTION_H

#include <string>
#include <boost/asio.hpp>

namespace lc2pp {
  class Connection {
  public:
    Connection(void) {};
    ~Connection(void) {};

    virtual void send(std::string message);
    virtual std::string wait_recv();
  };

  Connection* connect(std::string host, unsigned short port) {};
}

#endif
