#ifndef lucipp_TEST_CONNECTION_H
#define lucipp_TEST_CONNECTION_H

#include "lucipp/core/message.h"
#include "lucipp/core/connection.h"

#include "json/src/json.hpp"
#include "easylogging/src/easylogging++.h"
#include "googletest/googletest/include/gtest/gtest.h"

#include <string>
#include <vector>
#include <iostream>
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
    //connection = new lucipp::core::Connection("127.0.0.1", 7654);
    connection = std::make_shared<lucipp::core::Connection>("127.0.0.1", 7654);
  }

  virtual void TearDown() {
  }

  json simple_header_;
  //lucipp::core::Connection* connection;
  std::shared_ptr<lucipp::core::Connection> connection;
};

#endif
