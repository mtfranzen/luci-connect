#ifndef LC2PP_CONNECTION_H
#define LC2PP_CONNECTION_H

#include <string>
#include <iostream>
#include <boost/asio.hpp>

namespace lc2pp {
  namespace connection {
    // A container for LC2 messages. A message can contain a *body* and an
    // arbitrary amount of binary attachments. The body **must** be JSON
    // encoded.
    class Message {
    public:
      Message(void) {}
      ~Message(void) {}
    };

    // The connection class handles all low-level tcp communication with the
    // LC2 instance. It can send and receive messages, keeps a record of the
    // connection and parsing state and takes care of miscarried packages
    // (aka panicking).
    class Connection {

    public:
      // Initializes the connection to a specific LC2 instance. The host
      // should be a valid ip or dns address like "127.0.0.1" or "localhost".
      // The port should be an unsigned short. The timeout is specified in
      // seconds and determines the timeout of the tcp connection.
      Connection(std::string host, std::string port, uint timeout = 10);

      // Returns the connection's timeout
      const uint timeout() const;

      // Returns the LC2 host address
      const std::string host() const;

      // Returns the LC2 port
      const std::string port() const;

      // Sends a message to the connected LC2 instance
      void Send(Message* message);

      // Waits for a message from the connected LC2 instance. Throws an
      // exception when the timeout is reached
      Message* WaitForMessage();

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

    // Connects to a LC2 instance given its host address and tcp port. The host
    // should be an ip-address or valid dns-name. The port should be the an
    // unsigned short as a string.
    Connection* connect(std::string host, std::string port);

  }
}

#endif
