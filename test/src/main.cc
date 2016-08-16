#include "lc2pp/service.h"
#include "lc2pp/core/message.h"
#include "lc2pp/core/connection.h"

#include "json/src/json.hpp"
#include "easylogging/src/easylogging++.h"
#include "gtest/gtest.h"

#include <clocale>

#include <iostream>
#include <string>

using namespace std;

INITIALIZE_EASYLOGGINGPP

namespace {

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FooTest() {
    // You can do set-up work for each test here.
  }

  virtual ~FooTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST(ConnectionTest, EstablishConnection) {
  lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
  connection->Open();
  connection->Close();
}

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
