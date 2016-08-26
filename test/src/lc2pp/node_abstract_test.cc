#include "lc2pp/node_tests.h"

namespace {

  // Mock simply sending one message
  class NodeMock : lc2pp::Node {
  public:
    NodeMock(std::shared_ptr<lc2pp::core::Connection> connection) : lc2pp::Node(connection) {}

    void Run() {
      this->connection_->SendAsync(lc2pp::core::Message::RunMessage(0, "ServiceList"));
    }

    void HandleRun(int64_t callId, std::string serviceName, json inputs) {
      LOG(DEBUG) << "handling run: " << serviceName << "\n" << inputs.dump();
    };

    void HandleCancel(int64_t callId) {
      LOG(DEBUG) << "handling cancel: " << std::to_string(callId);
    };

    void HandleResult(int64_t callId, json result) {
      LOG(DEBUG) << "handling result: " << "\n" << result.dump();
    };

    void HandleProgress(int64_t callId, int64_t percentage, json intermediateResult) {
      LOG(DEBUG) << "handling progress (" << std::to_string(percentage) << "\n" << intermediateResult.dump();
    };

    void HandleError(int64_t callId, std::string error) {
      LOG(DEBUG) << "handling error: " << error;
    };
  };


  TEST_F(AbstractNodeTest, Simple) {
    std::shared_ptr<lc2pp::core::Connection> connection = std::make_shared<lc2pp::core::Connection>("127.0.0.1", 7654);
    NodeMock* node_ = new NodeMock(connection);
    node_->Run();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
