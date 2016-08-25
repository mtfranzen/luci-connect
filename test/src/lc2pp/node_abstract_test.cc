#include "lc2pp/node_tests.h"

namespace {

  // Mock simply sending one message
  class NodeMock : lc2pp::Node {
  public:
    NodeMock(std::string host, uint16_t port) : lc2pp::Node(host, port) {}

    void Run() {
      json simple_header_ = {{"run", "testService"},{"callID", 0}};
      lc2pp::core::Message* message = new lc2pp::core::Message(simple_header_);
      this->connection_->SendAsync(message);
    }
  };


  TEST_F(AbstractNodeTest, Simple) {
    NodeMock* node_ = new NodeMock("127.0.0.1", 7654);
    node_->Run();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
