#include "lc2pp/node.h"

namespace lc2pp {
  Node::Node(std::shared_ptr<core::Connection> connection) {
    this->connection_ = connection;

    // register delegates for the connection
    this->connection_->RegisterOnReceived(std::bind(&Node::HandleReceived, this, _1));
    this->connection_->RegisterOnSent(std::bind(&Node::HandleSent, this, _1));
    this->connection_->RegisterOnReceivingError(std::bind(&Node::HandleReceivingError, this, _1));
    this->connection_->RegisterOnSendingError(std::bind(&Node::HandleSendingError, this, _1, _2));

    // open connection
    this->connection_->Open();
  }

  void Node::HandleReceived(core::Message message) {
    json header = message.GetHeader();
    int64_t callId = header["callID"];

    switch (message.GetType()) {
      case core::MessageType::run:
        {
          std::string serviceName = header["run"];
          json inputs = {};
          for (json::iterator it = header.begin(); it != header.end(); ++it)
            if (it.key() != "callID" && it.key() != "run")
              inputs[it.key()] = it.value();

          this->HandleRun(callId, serviceName, inputs);
        }
        break;

      case core::MessageType::cancel:
        {
          this->HandleCancel(callId);
        }
        break;

      case core::MessageType::result:
        {
          json result = header["result"];

          this->HandleResult(callId, result);
        }
        break;

      case core::MessageType::progress:
        {
          int64_t percentage = header["progress"];
          if (header.count("intermediateResult") > 0) {
            json intermediateResult = header["intermediateResult"];
            this->HandleProgress(callId, percentage, intermediateResult);
          }
          else {
            this->HandleProgress(callId, percentage);
          }
        }
        break;

      case core::MessageType::error:
        {
          std::string error = header["error"];

          this->HandleError(callId, error);
        }
        break;
    }
  }

  void Node::HandleSent(core::Message message) {
    // TODO: Implement HandleSent in Node class
  }

  void Node::HandleReceivingError(core::Connection::ReceivingError) {
    // TODO: Implement HandleReceivingError in Node class
  }

  void Node::HandleSendingError(core::Message message, core::Connection::SendingError) {
    // TODO: Implement HandleSendingError in Node class
  }

  void Node::SendRun(int64_t callId, std::string serviceName, json inputs) {
    core::Message* message = core::Message::RunMessage(callId, serviceName, inputs);
    this->connection_->SendAsync(message);
  }

  void Node::SendCancel(int64_t callId) {
    core::Message* message = core::Message::CancelMessage(callId);
    this->connection_->SendAsync(message);
  }

  void Node::SendResult(int64_t callId, json result) {
    core::Message* message = core::Message::ResultMessage(callId, result);
    this->connection_->SendAsync(message);
  }

  void Node::SendProgress(int64_t callId, int64_t percentage, json intermediateResult) {
    core::Message* message = core::Message::ProgressMessage(callId, percentage, intermediateResult);
    this->connection_->SendAsync(message);
  }

  void Node::SendError(int64_t callId, std::string error) {
    core::Message* message = core::Message::ErrorMessage(callId, error);
    this->connection_->SendAsync(message);
  }
}
