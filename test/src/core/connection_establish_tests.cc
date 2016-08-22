#include "core/connection_tests.h"

/**
* Contains tests for establishing a connection (low-level), see protocoltest.cc
* for tests on protocol level.
*/
namespace {
  TEST_F(ConnectionTest, EstablishConnection) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();
    connection->Close();
  }

  TEST_F(ConnectionTest, EstablishWrongConnection) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 9000, 10);
    ASSERT_ANY_THROW(connection->Open());
  }
  
  TEST_F(ConnectionTest, EstablishConcurrentConnections) {
    std::vector<lc2pp::core::Connection*> connections;

    size_t N = 64;
    for (size_t i = 0; i < N; i++) {
      lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
      connections.push_back(connection);
    }

    for (lc2pp::core::Connection* connection : connections)
      connection->Open();

    for (lc2pp::core::Connection* connection : connections)
      connection->Close();
  }
}
