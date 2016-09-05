#ifndef luciconnect_TEST_CONNECTION_H
#define luciconnect_TEST_CONNECTION_H

#include "luciconnect/luciconnect.h"
#include <gtest/gtest.h>

#include <string>
#include <memory>

// Only a stub for later use
class ConnectionTest : public ::testing::Test {
 protected:
  ConnectionTest() {
    simple_header_ = {
      {"run", "testService"},
      {"callID", 0}
    };
  }

  virtual ~ConnectionTest() {
  }

  virtual void SetUp() {
    //connection = new luciconnect::core::Connection("127.0.0.1", 7654);
    connection = std::make_shared<luciconnect::core::Connection>("127.0.0.1", 7654);
  }

  virtual void TearDown() {
  }

  json simple_header_;
  //luciconnect::core::Connection* connection;
  std::shared_ptr<luciconnect::core::Connection> connection;
};

#endif
