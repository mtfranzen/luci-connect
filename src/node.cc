#include "lc2pp/node.h"

namespace lc2pp {
  Node::Node(std::string host, uint16_t port) {
    this->connection_ = std::make_shared<core::Connection>(host, port);
    this->connection_->RegisterDelegate(std::bind(&Node::HandleMessage, this, _1));
    this->connection_->Open();
  }

  void Node::HandleMessage(core::Message message) {
    LOG(DEBUG) << "Message received.";
  }

  Node::~Node() {
    // TODO: Add destructor to node class
  }
}
