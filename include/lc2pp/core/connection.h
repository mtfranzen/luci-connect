#ifndef LC2PP_CONNECTION_H
#define LC2PP_CONNECTION_H

#include "lc2pp/core/message.h"

#include "json/src/json.hpp"
#include "easylogging/src/easylogging++.h"

#include <asio.hpp>
#include <vector>
#include <string>
#include <algorithm> // std::reverse
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono> // for sleeping

using json = nlohmann::json;

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
    class Connection : public std::enable_shared_from_this<Connection> {
    public:
      /** Initializes the connection to a specific LC2 instance. The host
       * should be a valid ip or dns address like "127.0.0.1" or "localhost".
       * The port should be an unsigned short. The timeout is specified in
       * seconds and determines the timeout of the tcp connection **and** when
       * receiving parts of a message. */
      Connection(std::string host, uint16_t port);

      /** Opens up the connection to the LC2 instance */
      void Open();

      /** Terminates the connections */
      void Close();

      /** Sends a message to the connected LC2 instance */
      void Send(Message* message);

      /** Receives a message and delegates it*/
      void Receive();

      /**
      * The method registers a handler method for asynchronous message receiving.
      * If a message is received, the type is checked and forwarded to the
      * appointed callback functions. Not that multiple functions can
      * handle a certain message type.
      */
      void RegisterDelegate(MessageType messagetype, std::function<void(Message*)> callback);

      // TODO: Document handlers in connection class
      void SendHandler(const asio::error_code& error, std::size_t bytes_transferred);
      void WaitForHandlers(); // called when sending / receiving is finished
      void WaitForMessageReceival(); // called when message is ready to be received

      /** Closes all left-over sockets and cleanly destroys the object */
      ~Connection();

    private:
      // connection parameters
      std::string host_;
      uint16_t port_;
      bool is_connected_;

      // socket stuff
      asio::ip::tcp::socket* socket_;
      asio::io_service* io_service_;
      asio::ip::tcp::acceptor* acceptor_;
      asio::ip::tcp::resolver::iterator iterator_;

      // threading
      std::thread* thread_waiting_for_events_;
      std::mutex mutex_sending_;
      std::mutex mutex_receiving_;
      std::mutex mutex_waiting_for_events_;

      // the message that is currently being processed. Used for both sending
      // and receiving.
      Message* tmp_message_;


      // handlers for asynchronous message handling
      void DelegateMessage(Message* message);

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
      void SendAttachmentSize(Message* message, size_t position);
      void SendAttachment(Message* message, size_t position);

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
