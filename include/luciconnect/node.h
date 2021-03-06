#ifndef luciconnect_NODE_H_
#define luciconnect_NODE_H_

#include "connection.h"
#include "message.h"

using std::placeholders::_1;
using std::placeholders::_2;

namespace luciconnect {
  /**
  * The Node class is the super classes for all services and clients of Luci.
  *
  * To use it, overwrite the methods
  *
  *  - `Run`
  *  - `HandleRun`
  *  - `HandleCancel`
  *  - `HandleResult`
  *  - `HandleProgress`
  *  - `HandleError`
  *
  * Have a look at the methods to get a clearer picture. Example implementations
  * of Nodes can be found in the tests.
  */
  class Node {
  public:
    /**
    * Initializes the node. Call Node::Connect() to connect to the server.
    */
    Node(std::shared_ptr<Connection> connection);

    /**
    * Starts the node. This is a required method for a service or client
    * implementation!
    */
    virtual void Run() = 0;

  protected:
    /**
    * A reference to the Luci connection.
    */
    std::shared_ptr<Connection> connection_;

    /**
    * Opens the connection to the Luci server
    */
    void Connect();

    /**
    * Closes the connection to the Luci server
    */
    void Disconnect();

    /**
    * Sends a run message to the connected Luci instance.
    */
    void SendRun(int64_t callId, std::string serviceName, json inputs = {}, std::vector<Attachment*> attachments = {});

    /**
    * Sends a cancel message to the connected Luci instance.
    */
    void SendCancel(int64_t callId);

    /**
    * Sends a result message to the connected Luci instance.
    */
    void SendResult(int64_t callId, json result, std::vector<Attachment*> attachments = {});

    /**
    * Sends a progress message to the connected Luci instance.
    */
    void SendProgress(int64_t callId, int64_t percentage, std::vector<Attachment*> attachments = {}, json intermediateResult = {});

    /**
    * Sends an error message to the connected Luci instance.
    */
    void SendError(int64_t callId, std::string error);

    /**
    * Called when a **run** message has been received. This is a required method
    * for a service or client implementation!
    */
    virtual void HandleRun(int64_t callId, std::string serviceName, json inputs = {}, std::vector<Attachment*> attachments = {}) = 0;

    /**
    * Called when a **cancel** message has been received. This is a required method
    * for a service or client implementation! **Make sure to make HandleRun asynchronous
    * if you want to use this method!**
    */
    virtual void HandleCancel(int64_t callId) = 0;

    /**
    * Called when a **result** message has been received. This is a required method
    * for a service or client implementation!
    */
    virtual void HandleResult(int64_t callId, json result, std::vector<Attachment*> attachments = {}) = 0;

    /**
    * Called when a **progress** message has been received. This is a required method
    * for a service or client implementation!
    */
    virtual void HandleProgress(int64_t callId, int64_t percentage, std::vector<Attachment*> attachments = {}, json intermediateResult = {}) = 0;

    /**
    * Called when an **error** message has been received. This is a required method
    * for a service or client implementation!
    */
    virtual void HandleError(int64_t callId, std::string error) = 0;

  private:
    friend class Connection;

    void HandleReceived(Message message);

    void HandleSent(Message message);

    void HandleReceivingError(Connection::ReceivingError error);

    void HandleSendingError(Message message, Connection::SendingError error);
  };
}

#endif
