#ifndef LC2PP_SERVICE_H_
#define LC2PP_SERVICE_H_

#include "core/connection.h"
#include "core/message.h"

namespace lc2pp {
  // TODO: Document service class
  class Node {
  public:
    Service(void) {};
    ~Service(void) {};
    void Register(Connection* connection);
    void Unregister(Connection* connection);
  
  protected:
    friend void Connection::DelegateMessage HandleRunMessage(Message* message);
    friend void Connection::DelegateMessage HandleCancelMessage(Message* message);
    friend void Connection::DelegateMessage HandleProgressMessage(Message* message);
    friend void Connection::DelegateMessage HandleResultMessage(Message* message);
    friend void Connection::DelegateMessage HandleErrorMessage(Message* message);
  };
}

#endif
