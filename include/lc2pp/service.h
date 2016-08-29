/*
#ifndef LC2PP_SERVICE_H_
#define LC2PP_SERVICE_H_

#include "core/connection.h"
#include "core/message.h"

using std::placeholders::_1;
using std::placeholders::_2;

namespace lc2pp {
  class Service : Node{
  public:
    Service() {

    }

    void Run() {
    }

  protected:
    void Register(std::string name, json inputs, json outputs) {
      // create inputs for register call
      json header = {
        {"serviceName", name},
        {"inputs", inputs},
        {"outputs", outputs}
      };
      // construct example run message
      json sample_inputs = {};
      for (json::iterator it = inputs.begin(); it != inputs.end(); ++it) {
        if (it.value() == "number")
          example_call[it.key()] = 42.2;
        else if (it.value() == "integer" || it.value() == "int" || it.value() == "long")
          example_call[it.key()] = 42;
        else if (it.value() == "string")
          example_call[it.key()] = "test";
        else if (it.value() == "object")
          example_call[it.key()] = (json){}
      }
      Message* example_call = core:Message::RunMessage(0, name, example_inputs);
      for (json::iterator it = inputs.begin(); it != inputs.end(); ++it) {
        if (it.value() == "attachment") {
          core::Attachment atc = {0, ""};
          example_call.AddAttachment(&atc);
        }
      }
      header["exampleCall"] = example_call.GetHeader();
      this->SendRunMessage(0, "RemoteRegister", header);
    }

    void Deregister() {
      this->SendRunMessage(0, "RemoteDeregister");
    }
  };
}
*/
