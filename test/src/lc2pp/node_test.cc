#include "lc2pp/node_tests.h"

namespace {

  class ServiceMock : lc2pp::Node {
  public:
    ServiceMock(std::shared_ptr<lc2pp::core::Connection> connection) : lc2pp::Node(connection) {}

    void Run() {
      this->Connect();

      json register_message = {
        {"serviceName", "addingNumbers"},
        {"description", "Adds two numbers"},
        {"qua-view-compliant", false},
        {"inputs", {
          {"arg1", "number"},
          {"arg2", "number"}
        }},
        {"outputs", {
          {"sum", "number"}
        }},
        {"exampleCall", {
          {"run", "addingNumbers"},
          {"callId", 4386},
          {"arg1", 42},
          {"arg2", 1337}
        }}
      };

      this->SendRun(0, "RemoteRegister", register_message);
    }

  json result;
  bool canceled = false;
  bool error = false;
  int64_t progress = -1;

  protected:
    void HandleRun(int64_t callId, std::string serviceName, json inputs, std::vector<lc2pp::core::Attachment*> attachments) {
      if (inputs.count("arg1") != 1 || inputs.count("arg2") != 1)
        throw "Erroneous message";

      int64_t arg1 = inputs["arg1"];
      int64_t arg2 = inputs["arg2"];
      json result = {{"sum", arg1 + arg2}};

      this->SendResult(callId, result);
    };

    void HandleCancel(int64_t callId) {
      this->canceled = true;
    };

    void HandleResult(int64_t callId, json result, std::vector<lc2pp::core::Attachment*> attachments) {
      this->result = result;
    };

    void HandleProgress(int64_t callId, int64_t percentage, std::vector<lc2pp::core::Attachment*> attachments, json intermediateResult) {
      this->progress = percentage;
    };

    void HandleError(int64_t callId, std::string error) {
      this->error = true;
    };
  };

  // Mock simply sending one message
  class ClientMock : lc2pp::Node {
  public:
    ClientMock(std::shared_ptr<lc2pp::core::Connection> connection) : lc2pp::Node(connection) {}

    void Run() {
      this->Connect();
      this->SendRun(0, "addingNumbers", {{"arg1", 5}, {"arg2", 7}});
    }

    json result;
    bool canceled = false;
    bool error = false;
    int64_t progress = -1;

  protected:
    void HandleRun(int64_t callId, std::string serviceName, json inputs, std::vector<lc2pp::core::Attachment*> attachments) {
    };

    void HandleCancel(int64_t callId) {
      this->canceled = true;
    };

    void HandleResult(int64_t callId, json result, std::vector<lc2pp::core::Attachment*> attachments) {
      this->result = result;
    };

    void HandleProgress(int64_t callId, int64_t percentage, std::vector<lc2pp::core::Attachment*> attachments, json intermediateResult) {
      this->progress = percentage;
    };

    void HandleError(int64_t callId, std::string error) {
      this->error = true;
    };
  };

  TEST_F(AbstractNodeTest, ClientRunWrongService) {
    std::shared_ptr<lc2pp::core::Connection> connection = std::make_shared<lc2pp::core::Connection>("127.0.0.1", 7654);
    ClientMock* node_ = new ClientMock(connection);
    node_->Run();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    ASSERT_EQ(node_->error, true);
  }

  TEST_F(AbstractNodeTest, ServiceRegister) {
    std::shared_ptr<lc2pp::core::Connection> connection = std::make_shared<lc2pp::core::Connection>("127.0.0.1", 7654);
    ServiceMock* node_ = new ServiceMock(connection);
    node_->Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    json expected_result = {{"registeredName", "addingNumbers"}};
    ASSERT_EQ(node_->result, expected_result);
  }

  TEST_F(AbstractNodeTest, ClientServiceRun) {
    std::shared_ptr<lc2pp::core::Connection> connection1 = std::make_shared<lc2pp::core::Connection>("127.0.0.1", 7654);
    ServiceMock* service = new ServiceMock(connection1);
    service->Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::shared_ptr<lc2pp::core::Connection> connection2 = std::make_shared<lc2pp::core::Connection>("127.0.0.1", 7654);
    ClientMock* client = new ClientMock(connection2);
    client->Run();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    json expected_result = {{"sum", 12}};
    ASSERT_EQ(client->result, expected_result);
  }
}
