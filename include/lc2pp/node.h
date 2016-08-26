#ifndef LC2PP_NODE_H_
#define LC2PP_NODE_H_

#include "core/connection.h"
#include "core/message.h"

using std::placeholders::_1;

namespace lc2pp {
  // TODO: Document Node class
  class Node {
  public:
    Node(std::string host, uint16_t port);
    ~Node();

    // These need to be implemented:
    virtual void Run() = 0;

  protected:
    // the connection
    std::shared_ptr<core::Connection> connection_;

  private:
    // the connection class needs access to the message handlers
    friend class core::Connection;

    // this is implemented
    void HandleReceived(core::Message message);

    void HandleSent(core::Message message);

    void HandleReceivingError(core::Connection::ReceivingError error);

    void HandleSendingError(core::Connection::SendingError error);

    // these need to be implemented:
    // TODO: Add session handling to Node class
  };
}

#endif
