#ifndef LC2PP_CONNECTION_H
#define LC2PP_CONNECTION_H

#include <string>
#include <boost/asio.hpp>
#include <iostream>

namespace lc2pp {
  class Connection {
  public:
    Connection(void) {};
    ~Connection(void) {};

    virtual void send(std::string message);
    virtual std::string wait_recv();
  };
}

#endif
