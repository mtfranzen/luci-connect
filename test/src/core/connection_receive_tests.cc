#include "core/connection_tests.h"

/**
* Contains tests for establishing a connection (low-level), see protocoltest.cc
* for tests on protocol level.
*/
namespace {
  /*
  TEST_F(ConnectionTest, ReceiveMessage) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();

    lc2pp::core::Message* message = new lc2pp::core::Message(simple_header_);
    connection->Send(message);
    connection->Receive();
    connection->Close();
  }

  TEST_F(ConnectionTest, ReceiveMessageWhenClosed) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();

    lc2pp::core::Message* message = new lc2pp::core::Message(simple_header_);
    connection->Send(message);
    connection->Close();
    ASSERT_ANY_THROW(connection->Receive());
  }

  TEST_F(ConnectionTest, ReceiveOnConcurrentConnections) {
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
      connection->Receive();

    for (lc2pp::core::Connection* connection : connections)
      connection->Close();
  }
  */
}
