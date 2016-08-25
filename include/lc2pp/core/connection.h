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
    // TODO: Add custom exception in connection class

    /** The connection class handles all low-level tcp communication with the
     * LC2 instance. It can send and receive messages, keeps a record of the
     * connection and parsing state and takes care of miscarried packages
     * (aka panicking). The class works **synchronously** but handles connection
     * problems using an assigned timeout value.
     *
     * Usage:
     * ```
     * void receiveHandler(Message* message) {
     *   // do something with the message
     * }
     *
     * json header = {{"name", "test"}, {"abc", "def"}};
     * Attachment attachment = {5, "abcde"};
     *
     * // connect to luci
     * std::shared_ptr<Connection> connection = std::make_shared<Connection>("127.0.0.1", 8008);
     * connection->RegisterDelegate(std::function<void(Message*>(receiveHandler)));
     * connection->Open();
     *
     * // compose message
     * Message* message = new Message(header);
     * message->AddAttachment(attachment);
     *
     * // send message
     * connection->SendAsync(message);
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
      void SendAsync(Message* message);

      /**
      * The method registers a handler method for asynchronous message receiving.
      * If a message is received, the type is checked and forwarded to the
      * appointed callback functions. Not that multiple functions can
      * handle a certain message type.
      */
      void RegisterDelegate(std::function<void(Message)> callback);

      /** Closes all left-over sockets and cleanly destroys the object */
      ~Connection();

    private:
      // connection parameters
      std::string host_;
      uint16_t port_;

      // handlers, registered by nodes and called upon message arrival
      std::vector<std::function<void(Message)>> receive_handlers_;

      // state indicators
      bool is_connected_ = false;
      std::atomic<bool> is_disconnecting_;
      std::atomic<bool> is_receiving_;

      // socket stuff
      asio::ip::tcp::socket* socket_;
      asio::io_service* io_service_;
      asio::ip::tcp::acceptor* acceptor_;
      asio::ip::tcp::resolver::iterator iterator_;

      // threading
      std::thread* thread_waiting_for_events_;
      std::thread* thread_waiting_for_receival_;

      // the message that is currently being processed. Used for both sending
      // and receiving.
      Message* recv_message_;
      size_t recv_validation_size_;
      std::vector<char> recv_buf_header_size_;
      std::vector<char> recv_buf_body_size_;
      std::vector<char> recv_buf_header_;
      std::vector<char> recv_buf_num_attachments_;
      std::vector<char> recv_buf_attachment_size_;
      std::vector<char> recv_buf_attachment_data_;

      void HandleMessageReceived();

      void HandleBufferSent(const asio::error_code& error,
        std::size_t bytes_transferred);

      void ReceiveAsync();

      void WaitForHandlers(); // called when sending / receiving is finished

      void WaitForReceival(); // called when message is ready to be received

      void ReceiveHeaderSize(); // x = 8 byte bigendian signed

      void ReceiveBodySize(const asio::error_code& error,
        std::size_t size_transferred); // y = 8 byte bigendian signed

      void ReceiveHeader(const asio::error_code& error,
        std::size_t size_transferred); // x bytes bigendian

      void ReceiveNumberOfAttachments(const asio::error_code& error,
        std::size_t size_transferred); // N = 8 byte bigendian signed

      void ReceiveAttachmentSize(const asio::error_code& error,
        std::size_t size_transferred); // yi = 8 byte bigendian signed

      void ReceiveAttachmentData(const asio::error_code& error,
        std::size_t size_transferred); // yi by

      void ReceiveNextAttachment(const asio::error_code& error,
        std::size_t size_transferred);

      // member functions for message sending
      void SendHeaderSize(Message* message);
      void SendBodySize(Message* message);
      void SendHeader(Message* message);
      void SendNumberOfAttachments(Message* message);
      void SendAttachmentSize(Message* message, size_t position);
      void SendAttachment(Message* message, size_t position);

      // Low-Level send functions
      void SendString(std::string data);
      void SendInt64(int64_t data);
      void SendBuffer(asio::const_buffers_1 data);

      // Low-Level receive functions
      int64_t ParseInt64(std::vector<char> buffer);
      std::string ParseString(std::vector<char> buffer);
    };

    /** Connects to a LC2 instance given its host address and tcp port. The host
     * should be an ip-address or valid dns-name. The port should be the an
     * unsigned short as a string. */
    Connection* connect(std::string host, std::string port);
  }
}

#endif
