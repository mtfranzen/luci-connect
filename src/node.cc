#include "lc2pp/node.h"

namespace lc2pp {
  Node::Node(std::string host, uint16_t port) {
    this->connection_ = std::make_shared<core::Connection>(host, port);

    // register delegates for the connection
    this->connection_->RegisterOnReceived(std::bind(&Node::HandleReceived, this, _1));
    this->connection_->RegisterOnSent(std::bind(&Node::HandleSent, this));
    this->connection_->RegisterOnReceivingError(std::bind(&Node::HandleReceivingError, this));
    this->connection_->RegisterOnSendingError(std::bind(&Node::HandleSendingError, this));

    // open connection
    this->connection_->Open();
  }

  void Node::HandleReceived(core::Message message) {
    LOG(DEBUG) << "Message received.";
    // TODO: Implement HandleReceived in Node class
  }

  void Node::HandleSent() {
    // TODO: Implement HandleSent in Node class
  }

  void Node::HandleReceivingError() {
    // TODO: Implement HandleReceivingError in Node class
  }

  void Node::HandleSendingError() {
    // TODO: Implement HandleSendingError in Node class
  }

  Node::~Node() {
    // TODO: Add destructor to node class in Node class
  }
}
