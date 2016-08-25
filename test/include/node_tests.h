#ifndef LC2PP_TEST_MESSAGE_H
#define LC2PP_TEST_MESSAGE_H

#include "lc2pp/node.h"

#include "json/src/json.hpp"
#include "easylogging/src/easylogging++.h"
#include "googletest/googletest/include/gtest/gtest.h"

#include <string>
#include <vector>
#include <iostream>


class NodeMock : lc2pp::Node {
public:

  NodeMock(std::string host, uint16_t port) : lc2pp::Node(host, port) {

  }

  void Run() {
    json simple_header_ = {
      {"run", "testService"},
      {"callID", 0}
    };

    lc2pp::core::Message* message = new lc2pp::core::Message(simple_header_);
    this->connection_->SendAsync(message);
  }
};

// Only a stub for later use
class AbstractNodeTest : public ::testing::Test {
 protected:
  AbstractNodeTest() {
    node_ = new NodeMock("127.0.0.1", 7654);
  }

  virtual ~AbstractNodeTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  NodeMock* node_;
};

#endif
