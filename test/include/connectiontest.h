#ifndef LC2PP_TEST_CONNECTION_H
#define LC2PP_TEST_CONNECTION_H

#include "lc2pp/service.h"
#include "lc2pp/core/message.h"
#include "lc2pp/core/connection.h"

#include "json/src/json.hpp"
#include "easylogging/src/easylogging++.h"
#include "googletest/googletest/include/gtest/gtest.h"

#include <string>
#include <vector>

// Only a stub for later use
class ConnectionTest : public ::testing::Test {
 protected:
  ConnectionTest() {
  }

  virtual ~ConnectionTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

#endif
