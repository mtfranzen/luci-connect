#ifndef LC2PP_CONNECTION_H
#define LC2PP_CONNECTION_H

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "lib/json/src/json.hpp"

namespace lc2pp {
  namespace connection {

    /** A container for LC2 messages. A message can contain a *body* and an
     * arbitrary amount of binary attachments. The body **must** be JSON
     * encoded. */
    class Message {
      // TODO: add header, add attachments etc
    };

    /** The connection class handles all low-level tcp communication with the
     * LC2 instance. It can send and receive messages, keeps a record of the
     * connection and parsing state and takes care of miscarried packages
     * (aka panicking). The class works **synchronously** but handles connection
     * problems using an assigned timeout value.
     *
     * Usage:
     * ```
     *  Connection* connection = new Connection(...);
     *  connection->Start();
     *  connection->Send(...);
     *  connection->ReceiveMessage();
     *  connection->Stop();
     * ```
     */
    class Connection {
    public:
      /** Initializes the connection to a specific LC2 instance. The host
       * should be a valid ip or dns address like "127.0.0.1" or "localhost".
       * The port should be an unsigned short. The timeout is specified in
       * seconds and determines the timeout of the tcp connection **and** when
       * receiving parts of a message. */
      Connection(std::string host, uint8_t port, uint timeout = 10);

      /** The number of seconds after which the connection breaks up when a
       * messages is expeced but not received. */
      const uint timeout() const;

      /** The assigned target host address */
      const char* host() const;

      /** The assigned target port */
      const uint8_t port() const;

      /** Opens up the connection to the LC2 instance */
      void Open();

      /** Terminates the connections */
      void Close();

      /** Sends a message to the connected LC2 instance */
      void Send(Message* message);

      /** Returns the last successfully parsed message from Luci */
      Message* ReceiveMessage();

    private:
      // connection-specific data members
      const std::string host_;
      const std::string port_;
      const uint timeout_;

      // member functions for message receiving
      uint8_t ReceiveHeaderSize(); // x = 1 byte
      uint8_t ReceiveBodySize(); // y = 1 byte
      char* ReceiveHeader(size_t length); // x bytes
      uint8_t ReceiveNumberOfAttachments(); // N = 1 byte
      uint8_t ReceiveAttachmentSize(); // yi = 1 byte
      char* ReceiveAttachment(size_t length); // yi bytes
    };

    /** Connects to a LC2 instance given its host address and tcp port. The host
     * should be an ip-address or valid dns-name. The port should be the an
     * unsigned short as a string. */
    Connection* connect(std::string host, std::string port);

  }
}

#endif
