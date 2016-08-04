#include "include/service.h"

#include <iostream>
#include <string>

using namespace std;

class SimpleService : lc2pp::Service {
public:
  SimpleService(string name) {
    cout << name << endl;
  }
};

int main(int args, const char* argv[]) {
  SimpleService* node = new SimpleService("simple_service_name");
}
