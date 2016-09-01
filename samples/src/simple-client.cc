#include "luciconnect/node.h"
INITIALIZE_EASYLOGGINGPP

class SimpleClient: luciconnect::Node {

public:
  SimpleClient(std::shared_ptr<luciconnect::core::Connection> connection) : luciconnect::Node(connection) {}

  void Run() {
    this->Connect();
  }

  void AddNumbers(int arg1, int arg2) {
    json input = {
      {"arg1", arg1},
      {"arg2", arg2}
    };
    this->SendRun(0, "AddingNumbersService", input);
  }

  bool waiting_for_result = true;

protected:
  void HandleResult(int64_t callId, json result, std::vector<luciconnect::core::Attachment*> attachments) {
    std::cout << result.dump() << std::endl;
    this->waiting_for_result = false;
  }

  void HandleRun(int64_t callId, std::string serviceName, json inputs, std::vector<luciconnect::core::Attachment*> attachments) {}

  void HandleCancel(int64_t callId) {}

  void HandleProgress(int64_t callId, int64_t percentage, std::vector<luciconnect::core::Attachment*> attachments, json intermediateResult) {}

  void HandleError(int64_t callId, std::string error) {}
};

int main(int argc, char **argv) {
  std::shared_ptr<luciconnect::core::Connection> connection = std::make_shared<luciconnect::core::Connection>("127.0.0.1", 7654);
  SimpleClient* client = new SimpleClient(connection);

  client->Run();
  client->AddNumbers(std::atoi(argv[1]), std::atoi(argv[2]));
  while (client->waiting_for_result);
}
