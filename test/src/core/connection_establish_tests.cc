#include "core/connection_tests.h"

/**
* Contains tests for establishing a connection (low-level), see protocoltest.cc
* for tests on protocol level.
*/
namespace {
  TEST_F(ConnectionTest, EstablishConnection) {
    connection->Open();
    connection->Close();
  }

  TEST_F(ConnectionTest, EstablishWrongConnection) {
    lc2pp::core::Connection* con = new lc2pp::core::Connection("127.0.0.1", 9000);
    ASSERT_ANY_THROW(con->Open());
  }

  TEST_F(ConnectionTest, EstablishConcurrentConnections) {
    std::vector<lc2pp::core::Connection*> connections;

    size_t N = 64;
    for (size_t i = 0; i < N; i++) {
      lc2pp::core::Connection* con = new lc2pp::core::Connection("127.0.0.1", 7654);
      connections.push_back(con);
    }

    for (lc2pp::core::Connection* con : connections)
      con->Open();

    for (lc2pp::core::Connection* con : connections)
      con->Close();
  }
}
