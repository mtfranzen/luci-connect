#include "luciconnect/node_tests.h"

namespace {

  class ServiceMock : luciconnect::Node {
  public:
    ServiceMock(std::shared_ptr<luciconnect::core::Connection> connection) : luciconnect::Node(connection) {}

    void Run() {
      this->Connect();

      json register_message = {
        {"serviceName", "addingNumbers"},
        {"description", "Adds two numbers"},
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
    void HandleRun(int64_t callId, std::string serviceName, json inputs, std::vector<luciconnect::core::Attachment*> attachments) {
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

    void HandleResult(int64_t callId, json result, std::vector<luciconnect::core::Attachment*> attachments) {
      this->result = result;
      LOG(DEBUG) << "result: " << result.dump();
    };

    void HandleProgress(int64_t callId, int64_t percentage, std::vector<luciconnect::core::Attachment*> attachments, json intermediateResult) {
      this->progress = percentage;
      LOG(DEBUG) << "progress";
    };

    void HandleError(int64_t callId, std::string error) {
      LOG(DEBUG) << "error: " << error;
      this->error = true;
    };
  };

  // Mock simply sending one message
  class ClientMock : luciconnect::Node {
  public:
    ClientMock(std::shared_ptr<luciconnect::core::Connection> connection) : luciconnect::Node(connection) {}

    void Run() {
      this->Connect();
      this->SendRun(0, "addingNumbers", {{"arg1", 5}, {"arg2", 7}});
    }

    json result;
    bool canceled = false;
    bool error = false;
    int64_t progress = -1;

  protected:
    void HandleRun(int64_t callId, std::string serviceName, json inputs, std::vector<luciconnect::core::Attachment*> attachments) {
    };

    void HandleCancel(int64_t callId) {
      this->canceled = true;
    };

    void HandleResult(int64_t callId, json result, std::vector<luciconnect::core::Attachment*> attachments) {
      this->result = result;
    };

    void HandleProgress(int64_t callId, int64_t percentage, std::vector<luciconnect::core::Attachment*> attachments, json intermediateResult) {
      this->progress = percentage;
    };

    void HandleError(int64_t callId, std::string error) {
      this->error = true;
    };
  };

  TEST_F(AbstractNodeTest, ClientRunWrongService) {
    std::shared_ptr<luciconnect::core::Connection> connection = std::make_shared<luciconnect::core::Connection>("127.0.0.1", 7654);
    ClientMock* node_ = new ClientMock(connection);
    node_->Run();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    ASSERT_EQ(node_->error, true);
  }

  TEST_F(AbstractNodeTest, ServiceRegister) {
    std::shared_ptr<luciconnect::core::Connection> connection = std::make_shared<luciconnect::core::Connection>("127.0.0.1", 7654);
    ServiceMock* node_ = new ServiceMock(connection);
    node_->Run();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    ASSERT_EQ(node_->result.count("registeredName"), 1);
    ASSERT_EQ(node_->result["registeredName"], "addingNumbers");
  }

  TEST_F(AbstractNodeTest, ClientServiceRun) {
    std::shared_ptr<luciconnect::core::Connection> connection1 = std::make_shared<luciconnect::core::Connection>("127.0.0.1", 7654);
    ServiceMock* service = new ServiceMock(connection1);
    service->Run();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::shared_ptr<luciconnect::core::Connection> connection2 = std::make_shared<luciconnect::core::Connection>("127.0.0.1", 7654);
    ClientMock* client = new ClientMock(connection2);
    client->Run();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    json expected_result = {{"sum", 12}};
    ASSERT_EQ(client->result, expected_result);
  }
}
