#ifndef LUCI_CONNECT_TEST_CONNECTION_H
#define LUCI_CONNECT_TEST_CONNECTION_H

#include "luci-connect/core/message.h"
#include "luci-connect/core/connection.h"

#include "json/src/json.hpp"
#include "easylogging/src/easylogging++.h"
#include "googletest/googletest/include/gtest/gtest.h"

#include <string>
#include <vector>
#include <iostream>

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
    connection = new luci-connect::core::Connection("127.0.0.1", 7654);
  }

  virtual void TearDown() {
    delete connection;
  }

  json simple_header_;
  luci-connect::core::Connection* connection;
};

#endif