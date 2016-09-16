#ifndef luciconnect_TEST_NODE_H
#define luciconnect_TEST_NODE_H

#include "luciconnect/luciconnect.h"
#include <gtest/gtest.h>

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
