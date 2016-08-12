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
  json header = {
    {"name", "testϮ"},
    {"abc", 5}
  };
  string binary_data = "abcdeϮ";
  lc2pp::core::Attachment attachment = {5, binary_data.c_str()};

  // compose message
  lc2pp::core::Message* message = new lc2pp::core::Message(header);
  message->AddAttachment(attachment);

  SimpleService* node = new SimpleService("simple_service_name");
  node->PrintName();

  // connect somewhere
  lc2pp::core::Connection* connection = new lc2pp::core::Connection("google.com", 80, 10);
  connection->Open();
  connection->Send(message);
  connection->Close();
}
