#include "luciconnect/node.h"
#include <signal.h>

INITIALIZE_EASYLOGGINGPP

class SimpleService : luciconnect::Node {

public:
  SimpleService(std::shared_ptr<luciconnect::core::Connection> connection) : luciconnect::Node(connection) {}

  void Run() {
    this->Connect();
    this->SendRun(0, "RemoteRegister", this->register_message_);
    while(1);
  }

protected:
  const json register_message_ = {
    {"serviceName", "AddingNumbersService"},
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

  void HandleRun(int64_t callId, std::string serviceName, json inputs, std::vector<luciconnect::core::Attachment*> attachments) {
    if (inputs.count("arg1") != 1 || inputs.count("arg2") != 1)
      throw "Erroneous message";

    int64_t arg1 = inputs["arg1"];
    int64_t arg2 = inputs["arg2"];
    json result = {{"sum", arg1 + arg2}};

    this->SendResult(callId, result);
  };

  void HandleCancel(int64_t callId) {};
  void HandleResult(int64_t callId, json result, std::vector<luciconnect::core::Attachment*> attachments) {};
  void HandleProgress(int64_t callId, int64_t percentage, std::vector<luciconnect::core::Attachment*> attachments, json intermediateResult) {};
  void HandleError(int64_t callId, std::string error) {};
};

void exithandler(int param) {
  exit(1);
}

int main(int argc, char **argv) {
  signal(SIGINT, exithandler);

  std::shared_ptr<luciconnect::core::Connection> connection = std::make_shared<luciconnect::core::Connection>("127.0.0.1", 7654);
  SimpleService* service = new SimpleService(connection);
  service->Run();
}
