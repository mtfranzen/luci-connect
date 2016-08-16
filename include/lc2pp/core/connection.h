#ifndef LC2PP_CONNECTION_H
#define LC2PP_CONNECTION_H

#include "lc2pp/core/message.h"

#include "json/src/json.hpp"
using json = nlohmann::json;

#include "easylogging/src/easylogging++.h"

#include <asio.hpp>
#include <vector>
#include <string>
#include <algorithm> // std::reverse

// TODO: Add logging configuration

namespace lc2pp {
  namespace core {
    /** The connection class handles all low-level tcp communication with the
     * LC2 instance. It can send and receive messages, keeps a record of the
     * connection and parsing state and takes care of miscarried packages
     * (aka panicking). The class works **synchronously** but handles connection
     * problems using an assigned timeout value.
     *
     * Usage:
     * ```
     * json header = {{"name", "test"}, {"abc", "def"}};
     * Attachment attachment = {5, "abcde"};
     *
     * // connect to luci
     * Connection* connection = new Connection("127.0.0.1", 8008);
     * connection->Open();
     *
     * // compose message
     * Message* message = new Message(header);
     * message->AddAttachment(attachment);
     *
     * // send message
     * connection->Send(message);
     *
     * // receive reply
     * Message* reply = connection->Receive();
     * ``` */
    class Connection {
    public:
      /** Initializes the connection to a specific LC2 instance. The host
       * should be a valid ip or dns address like "127.0.0.1" or "localhost".
       * The port should be an unsigned short. The timeout is specified in
       * seconds and determines the timeout of the tcp connection **and** when
       * receiving parts of a message. */
      Connection(std::string host, uint16_t port, uint timeout);

      /** Opens up the connection to the LC2 instance */
      void Open();

      /** Terminates the connections */
      void Close();

      /** Sends a message to the connected LC2 instance */
      void Send(Message* message);

      /** Returns the last successfully parsed message from Luci */
      Message* Receive();

      /** Closes all left-over sockets and cleanly destroys the object */
      ~Connection();

    private:
      // connection parameters
      std::string host_;
      uint16_t port_;
      uint timeout_;
      bool is_connected_;

      // socket stuff
      asio::io_service io_service_;
      asio::ip::tcp::resolver::iterator iterator_;
      asio::ip::tcp::socket* socket_;

      // the message that is currently being processed. Used for both sending
      // and receiving.
      Message* tmp_message_;

      // member functions for message receiving
      int64_t ReceiveHeaderSize(); // x = 8 byte bigendian signed
      int64_t ReceiveBodySize(); // y = 8 byte bigendian signed
      std::string ReceiveHeader(size_t length); // x bytes bigendian
      int64_t ReceiveNumberOfAttachments(); // N = 8 byte bigendian signed
      int64_t ReceiveAttachmentSize(); // yi = 8 byte bigendian signed
      char* ReceiveAttachmentData(size_t length); // yi bytes bigendian

      // member functions for message sending
      void SendHeaderSize(Message* message);
      void SendBodySize(Message* message);
      void SendHeader(Message* message);
      void SendNumberOfAttachments(Message* message);
      void SendAttachmentSize(Message* message, size_t index);
      void SendAttachment(Message* message, size_t index);

      // Low-Level receive functions
      int64_t ReceiveInt64();
      std::string ReceiveString(size_t length);

      // Low-Level send functions
      void SendString(std::string data);
      void SendInt64(int64_t data);
      void SendBuffer(asio::const_buffers_1 data);
    };

    /** Connects to a LC2 instance given its host address and tcp port. The host
     * should be an ip-address or valid dns-name. The port should be the an
     * unsigned short as a string. */
    Connection* connect(std::string host, std::string port);
  }
}

#endif
