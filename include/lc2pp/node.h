#ifndef LC2PP_NODE_H_
#define LC2PP_NODE_H_

#include "core/connection.h"
#include "core/message.h"

using std::placeholders::_1;

namespace lc2pp {
  class Node {
  public:
    Node(std::string host, uint16_t port);
    ~Node();

    // These need to be implemented:
    virtual void Run() = 0;

  protected:
    // the connection class needs access to the message handlers
    friend class core::Connection;

    // the connection
    std::shared_ptr<core::Connection> connection_;

    // this is implemented
    void HandleMessage(core::Message message);

    // these need to be implemented:
    // TODO:
  };
}

#endif
