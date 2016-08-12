#include "lc2pp/service.h"
#include "lc2pp/core/message.h"
#include "lc2pp/core/connection.h"

#include "json/src/json.hpp"

#include <clocale>

#include <iostream>
#include <string>

using namespace std;

class SimpleService: lc2pp::Service {
public:
  string name;

  SimpleService(string name) {
    this->name = name;
  }

  void PrintName() {
    cout << this->name << endl;
  }
};

int main(int args, const char* argv[]) {
  json header = { {"run", "test.Randomly", "input", { "amount", 1} } };
  //string binary_data = "abcdeϮ";
  //lc2pp::core::Attachment attachment = {5, binary_data.c_str()};

  // compose message
  lc2pp::core::Message* message = new lc2pp::core::Message(header);
  //message->AddAttachment(attachment);

  SimpleService* node = new SimpleService("simple_service_name");
  node->PrintName();

  // connect somewhere
  lc2pp::core::Connection* connection = new lc2pp::core::Connection("0.0.0.0", 7654, 10);
  connection->Open();
  connection->Send(message);
  lc2pp::core::Message* reply = connection->Receive();
  std::cout << reply->GetHeader() << std::endl;
  connection->Close();
}
