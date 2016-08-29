#ifndef lucipp_TEST_MESSAGE_H
#define lucipp_TEST_MESSAGE_H

#include "lucipp/core/message.h"

#include "json/src/json.hpp"
#include "easylogging/src/easylogging++.h"
#include "googletest/googletest/include/gtest/gtest.h"

#include <string>
#include <vector>
#include <iostream>

// Only a stub for later use
class MessageTest : public ::testing::Test {
 protected:
  MessageTest() {
    simple_header_ = {
      {"run", "testService"},
      {"callID", 0}
    };
  }

  virtual ~MessageTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  json simple_header_;
};

#endif
