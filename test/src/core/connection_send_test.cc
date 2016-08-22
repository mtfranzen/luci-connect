#include "core/connection_tests.h"

/**
* Contains tests for establishing a connection (low-level), see protocoltest.cc
* for tests on protocol level.
*/
namespace {
  TEST_F(ConnectionTest, SendEmptyMessage) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();

    lc2pp::core::Message* message = new lc2pp::core::Message(simple_header_);
    connection->Send(message);
    connection->Close();
  }

  TEST_F(ConnectionTest, SendMessageWhenClosed) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();

    lc2pp::core::Message* message = new lc2pp::core::Message(simple_header_);
    connection->Close();
    ASSERT_ANY_THROW(connection->Send(message));
  }

  TEST_F(ConnectionTest, SendAttachment) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();

    std::string binary_data = "abcdeϮ";
    lc2pp::core::Attachment attachment = {binary_data.size(), binary_data.c_str(), "float32 array", "testname"};
    lc2pp::core::Message* message = new lc2pp::core::Message(simple_header_);
    message->AddAttachment(&attachment);
    connection->Send(message);
    connection->Close();
  }

  TEST_F(ConnectionTest, SendOnConcurrentConnections) {
    std::vector<lc2pp::core::Connection*> connections;

    size_t N = 64;
    for (size_t i = 0; i < N; i++) {
      lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
      connections.push_back(connection);
    }

    for (lc2pp::core::Connection* connection : connections)
      connection->Open();

    lc2pp::core::Message* message = new lc2pp::core::Message(simple_header_);
    for (lc2pp::core::Connection* connection : connections)
      connection->Send(message);

    for (lc2pp::core::Connection* connection : connections)
      connection->Close();
  }
}
