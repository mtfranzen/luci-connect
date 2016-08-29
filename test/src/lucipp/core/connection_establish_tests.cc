#include "lucipp/core/connection_tests.h"

/**
* Contains tests for establishing a connection (low-level), see protocoltest.cc
* for tests on protocol level.
*/
namespace {
  TEST_F(ConnectionTest, EstablishConnection) {
    connection->Open();
    connection->Close();
  }

  TEST_F(ConnectionTest, OpenCloseOpenCloseConnection) {
    connection->Open();
    connection->Close();
    connection->Open();
    connection->Close();
  }

  TEST_F(ConnectionTest, EstablishWrongConnection) {
    lucipp::core::Connection* con = new lucipp::core::Connection("127.0.0.1", 9000);
    ASSERT_ANY_THROW(con->Open());
  }

  TEST_F(ConnectionTest, EstablishConcurrentConnections) {
    std::shared_ptr<lucipp::core::Connection> con1 = std::make_shared<lucipp::core::Connection>("127.0.0.1", 7654);
    std::shared_ptr<lucipp::core::Connection> con2 = std::make_shared<lucipp::core::Connection>("127.0.0.1", 7654);
    std::shared_ptr<lucipp::core::Connection> con3 = std::make_shared<lucipp::core::Connection>("127.0.0.1", 7654);

    con1->Open();
    con2->Open();
    con3->Open();

    con1->Close();
    con2->Close();
    con3->Close();
  }
}
