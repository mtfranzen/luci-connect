#include "luciconnect/connection_tests.h"

/**
* Contains tests for establishing a connection (low-level), see protocoltest.cc
* for tests on protocol level.
*/
namespace {
  TEST_F(ConnectionTest, SendEmptyMessage) {
    connection->Open();

    luciconnect::Message* message = new luciconnect::Message(simple_header_);
    connection->SendAsync(message);
    connection->Close();
  }

  TEST_F(ConnectionTest, SendMessageWhenClosed) {
    connection->Open();

    luciconnect::Message* message = new luciconnect::Message(simple_header_);
    connection->Close();
    ASSERT_ANY_THROW(connection->SendAsync(message));
  }

  TEST_F(ConnectionTest, SendAttachment) {
    connection->Open();

    std::string binary_data = "abcdeϮ";
    luciconnect::Attachment attachment = {binary_data.size(), binary_data.c_str(), "float32 array", "testname"};
    luciconnect::Message* message = new luciconnect::Message(simple_header_);
    message->AddAttachment(&attachment);
    connection->SendAsync(message);

    connection->Close();
  }

  TEST_F(ConnectionTest, SendOnConcurrentConnections) {
    std::shared_ptr<luciconnect::Connection> con1 = std::make_shared<luciconnect::Connection>("127.0.0.1", 7654);
    std::shared_ptr<luciconnect::Connection> con2 = std::make_shared<luciconnect::Connection>("127.0.0.1", 7654);
    std::shared_ptr<luciconnect::Connection> con3 = std::make_shared<luciconnect::Connection>("127.0.0.1", 7654);

    con1->Open();
    con2->Open();
    con3->Open();

    luciconnect::Message* message = new luciconnect::Message(simple_header_);
    con1->SendAsync(message);
    con2->SendAsync(message);
    con3->SendAsync(message);

    con1->Close();
    con2->Close();
    con3->Close();
  }
}
