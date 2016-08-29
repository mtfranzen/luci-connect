#include "lucipp/core/connection_tests.h"

/**
* Contains tests for establishing a connection (low-level), see protocoltest.cc
* for tests on protocol level.
*/
namespace {
  TEST_F(ConnectionTest, SendEmptyMessage) {
    connection->Open();

    lucipp::core::Message* message = new lucipp::core::Message(simple_header_);
    connection->SendAsync(message);
    connection->Close();
  }

  TEST_F(ConnectionTest, SendMessageWhenClosed) {
    connection->Open();

    lucipp::core::Message* message = new lucipp::core::Message(simple_header_);
    connection->Close();
    ASSERT_ANY_THROW(connection->SendAsync(message));
  }

  TEST_F(ConnectionTest, SendAttachment) {
    connection->Open();

    std::string binary_data = "abcdeϮ";
    lucipp::core::Attachment attachment = {binary_data.size(), binary_data.c_str(), "float32 array", "testname"};
    lucipp::core::Message* message = new lucipp::core::Message(simple_header_);
    message->AddAttachment(&attachment);
    connection->SendAsync(message);

    connection->Close();
  }

  TEST_F(ConnectionTest, SendOnConcurrentConnections) {
    std::shared_ptr<lucipp::core::Connection> con1 = std::make_shared<lucipp::core::Connection>("127.0.0.1", 7654);
    std::shared_ptr<lucipp::core::Connection> con2 = std::make_shared<lucipp::core::Connection>("127.0.0.1", 7654);
    std::shared_ptr<lucipp::core::Connection> con3 = std::make_shared<lucipp::core::Connection>("127.0.0.1", 7654);

    con1->Open();
    con2->Open();
    con3->Open();

    lucipp::core::Message* message = new lucipp::core::Message(simple_header_);
    con1->SendAsync(message);
    con2->SendAsync(message);
    con3->SendAsync(message);

    con1->Close();
    con2->Close();
    con3->Close();
  }
}
