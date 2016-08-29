#ifndef luciconnect_TEST_MESSAGE_H
#define luciconnect_TEST_MESSAGE_H

#include "luciconnect/node.h"

#include "json/src/json.hpp"
#include "easylogging/src/easylogging++.h"
#include "googletest/googletest/include/gtest/gtest.h"

#include <string>
#include <vector>
#include <iostream>


// Only a stub for later use
class AbstractNodeTest : public ::testing::Test {
 protected:
  AbstractNodeTest() {
  }

  virtual ~AbstractNodeTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

#endif
