#ifndef LC2PP_SERVICE_H_
#define LC2PP_SERVICE_H_

#include "core/connection.h"
#include "core/message.h"

namespace lc2pp {
  class Node {
  public:
    Service(void) {};
    ~Service(void) {};
    void Register(Connection* connection);
    void Unregister(Connection* connection);

  protected:
    virtual friend void Connection::DelegateMessage HandleRunMessage(Message* message);
    virtual friend void Connection::DelegateMessage HandleCancelMessage(Message* message);
    virtual friend void Connection::DelegateMessage HandleProgressMessage(Message* message);
    virtual friend void Connection::DelegateMessage HandleResultMessage(Message* message);
    virtual friend void Connection::DelegateMessage HandleErrorMessage(Message* message);
  };
}

#endif
