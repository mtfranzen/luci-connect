#ifndef LC2PP_CONNECTION_H
#define LC2PP_CONNECTION_H

#include <boost/asio.hpp>
#include <string>

#include "message.h"

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
      Connection(std::string host, uint8_t port, uint timeout = 10);

      /** Opens up the connection to the LC2 instance */
      void Open();

      /** Terminates the connections */
      void Close();

      /** Sends a message to the connected LC2 instance */
      void Send(Message* message);

      /** Returns the last successfully parsed message from Luci */
      Message* Receive();

    private:
      // connection parameters
      const std::string host_;
      const uint8_t port_;
      const uint timeout_;

      // the message that is currently being processed. Used for both sending
      // and receiving.
      Message* tmp_message_;

      // member functions for message receiving
      uint8_t ReceiveHeaderSize(); // x = 1 byte
      uint8_t ReceiveBodySize(); // y = 1 byte
      char* ReceiveHeader(uint8_t length); // x bytes
      uint8_t ReceiveNumberOfAttachments(); // N = 1 byte
      uint8_t ReceiveAttachmentSize(); // yi = 1 byte
      char* ReceiveAttachment(uint8_t length); // yi bytes

      // member functions for message sending
      void SendHeaderSize(Message* message);
      void SendBodySize(Message* message);
      void SendHeader(Message* message);
      void SendNumberOfAttachments(Message* message);
      void SendAttachmentSize(Message* message);
      void SendAttachment(Message* message);
    };

    /** Connects to a LC2 instance given its host address and tcp port. The host
     * should be an ip-address or valid dns-name. The port should be the an
     * unsigned short as a string. */
    Connection* connect(std::string host, std::string port);
  }
}

#endif