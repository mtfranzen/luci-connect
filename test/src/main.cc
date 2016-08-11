#include "include/service.h"

#include <iostream>
#include <string>

using namespace std;

class SimpleService : lc2pp::Service {
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
  SimpleService* node = new SimpleService("simple_service_name");
  node->PrintName();
}
