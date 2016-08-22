#ifndef LC2PP_TEST_MESSAGE_H
#define LC2PP_TEST_MESSAGE_H

#include "lc2pp/service.h"
#include "lc2pp/core/message.h"

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
  }

  virtual ~MessageTest() {
  }

  virtual void SetUp() {
    simple_header_ = {
      {"run", "testService"},
      {"callID", 0}
    };
  }

  virtual void TearDown() {
  }

  json simple_header_;
};

#endif
