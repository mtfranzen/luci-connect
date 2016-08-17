#include "connectiontest.h"

/**
* Contains tests for establishing a connection (low-level), see protocoltest.cc
* for tests on protocol level.
*/
namespace {
  TEST(ConnectionTest, EstablishConnection) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();
    connection->Close();
  }

  TEST(ConnectionTest, EstablishWrongConnection) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 9000, 10);
    ASSERT_ANY_THROW(connection->Open());
  }

  TEST(ConnectionTest, SendEmptyMessage) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();

    json header = {};
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    connection->Send(message);
    connection->Close();
  }

  TEST(ConnectionTest, SendMessageWhenClosed) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();

    json header = {};
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    connection->Close();
    ASSERT_ANY_THROW(connection->Send(message));
  }

  TEST(ConnectionTest, SendAttachment) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();


    json header = {};
    std::string binary_data = "abcdeϮ";
    lc2pp::core::Attachment attachment = {binary_data.size(), binary_data.c_str(), "float32 array", "testname"};
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    message->AddAttachment(attachment);
    connection->Send(message);
    connection->Close();
  }

  TEST(ConnectionTest, EstablishConcurrentConnections) {
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

  TEST(ConnectionTest, SendOnConcurrentConnections) {
    std::vector<lc2pp::core::Connection*> connections;

    size_t N = 64;
    for (size_t i = 0; i < N; i++) {
      lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
      connections.push_back(connection);
    }

    for (lc2pp::core::Connection* connection : connections)
      connection->Open();

    json header = {};
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    for (lc2pp::core::Connection* connection : connections)
      connection->Send(message);

    for (lc2pp::core::Connection* connection : connections)
      connection->Close();
  }

  TEST(ConnectionTest, ReceiveMessage) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();

    json header = {};
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    connection->Send(message);
    connection->Receive();
    connection->Close();
  }

  TEST(ConnectionTest, ReceiveMessageWhenClosed) {
    lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
    connection->Open();

    json header = {};
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    connection->Send(message);
    connection->Close();
    ASSERT_ANY_THROW(connection->Receive());
  }

  TEST(ConnectionTest, ReceiveOnConcurrentConnections) {
    std::vector<lc2pp::core::Connection*> connections;

    size_t N = 64;
    for (size_t i = 0; i < N; i++) {
      lc2pp::core::Connection* connection = new lc2pp::core::Connection("127.0.0.1", 7654, 10);
      connections.push_back(connection);
    }

    for (lc2pp::core::Connection* connection : connections)
      connection->Open();

    json header = {};
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    for (lc2pp::core::Connection* connection : connections)
      connection->Send(message);

    for (lc2pp::core::Connection* connection : connections)
      connection->Receive();

    for (lc2pp::core::Connection* connection : connections)
      connection->Close();
  }

  TEST(ConnectionTest, ConnectivityConsistencyTest) {
    // TODO: Check whether the state of a connection is always consistent
  }
}
