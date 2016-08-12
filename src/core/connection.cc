#include "lc2pp/core/connection.h"
#include "lc2pp/core/message.h"
#include "json/src/json.hpp"

#include <boost/asio.hpp>

#include <vector>
#include <string>
#include <iostream>

using json = nlohmann::json;

namespace lc2pp {
  namespace core {
    // TODO: Make connection class asynchronous

    Connection::Connection(std::string host, uint8_t port, uint timeout = 10) {
      this->host_ = host;
      this->port_ = port;
      this->timeout_ = timeout;

      // create I/O iterator from host / port
      boost::asio::ip::tcp::resolver resolver(this->io_service_);
      this->iterator_ = resolver.resolve({host, std::to_string(port)});
    }

    void Connection::Open() {
      // TODO: Add timeout to socket connection

      if (this->socket_ != NULL) {
        this->Close();
      }
      this->socket_ = new boost::asio::ip::tcp::socket(this->io_service_);

      try {
        boost::asio::connect(*(this->socket_), this->iterator_);
        boost::asio::socket_base::keep_alive option(true);
        this->socket_->set_option(option);
      }
      catch (std::exception& err) {
        // TODO: Handle error when connection failed
      }
    }

    void Connection::Close() {
      boost::system::error_code ec;
      this->socket_->close(ec);

      if (ec) {
        // TODO: Handle error when closing socket failed
      }
    }

    void Connection::Send(Message* message) {
      // TODO: Test UTF-8 support for message sending / Receiving
      this->SendHeaderSize(message);
      this->SendBodySize(message);
      this->SendHeader(message);
      this->SendNumberOfAttachments(message);
      for (size_t i = 0; i < message->GetNumAttachments(); i++) {
        this->SendAttachmentSize(message, i);
        this->SendAttachment(message, i);
      }
    }

    void Connection::SendHeaderSize(Message* message) {
      std::string header = message->GetHeader().dump();
      int64_t header_size = header.size() * sizeof(char);
      this->SendInt64(header_size);
    }

    void Connection::SendBodySize(Message* message) {
      int64_t body_size = 8;
      for (size_t i = 0; i < message->GetNumAttachments(); i++)
        body_size += message->GetAttachment(i).size;
      this->SendInt64(body_size);
    }

    void Connection::SendHeader(Message* message) {
      std::string header = message->GetHeader().dump();
      this->SendString(header);
    }

    void Connection::SendNumberOfAttachments(Message* message) {
      int64_t num_attachments = message->GetNumAttachments();
      this->SendInt64(num_attachments);
    }

    void Connection::SendAttachmentSize(Message* message, size_t index) {
      int64_t attachment_size = message->GetAttachment(index).size;
      this->SendInt64(attachment_size);
    }

    void Connection::SendAttachment(Message* message, size_t index) {
      // TODO: Handle error when attachment pointer is null
      std::string attachment(message->GetAttachment(index).data);
      this->SendString(attachment);
    }

    void Connection::SendInt64(int64_t data) {
      // we are converting the data here to a constant so both SendInt64 and
      // SendString share the same buffer output type (const_buffers_1).
      // TODO: Check if int64_t needs to be reversed as in old library.
      const int64_t c_data = data;
      this->SendBuffer(boost::asio::buffer(&c_data, sizeof(int64_t)));
    }

    void Connection::SendString(std::string data) {
      const char* c_data = data.c_str();
      this->SendBuffer(boost::asio::buffer(c_data, data.size()));
    }

    void Connection::SendBuffer(boost::asio::const_buffers_1 data) {
      try {
        this->socket_->set_option(boost::asio::ip::tcp::no_delay(true));
        boost::asio::write(*(this->socket_), data);
      }
      catch (std::exception& e) {
        // TODO: Handle error when sending failed.
      }
    }

    Message* Connection::Receive() {
      int64_t validation_size = 8; // see LC2 documentation

      // receive header
      int64_t header_size = this->ReceiveHeaderSize();
      int64_t body_size = this->ReceiveBodySize();
      // TODO: Handle error when received header is corrupted
      json header = json::parse(this->ReceiveHeader(header_size));

      // construct message
      Message* message = new Message(header);

      // add attachments
      int64_t num_attachments = this->ReceiveNumberOfAttachments();
      for (int i = 0; i < num_attachments; i++) {
        int64_t attachment_size = this->ReceiveAttachmentSize();
        validation_size += attachment_size + 8;
        char* attachment_data = this->ReceiveAttachmentData(attachment_size);

        message->AddAttachment({attachment_size, attachment_data});
      }

      if (body_size != validation_size) {
        // TODO: validate received message.
      }
      return message;
    }

    int64_t Connection::ReceiveHeaderSize() {
      return this->ReceiveInt64();
    }

    int64_t Connection::ReceiveBodySize() {
      return this->ReceiveInt64();
    }

    std::string Connection::ReceiveHeader(size_t length) {
      return this->ReceiveString(length);
    }

    int64_t Connection::ReceiveNumberOfAttachments() {
      return this->ReceiveInt64();
    }

    int64_t Connection::ReceiveAttachmentSize() {
      return this->ReceiveInt64();
    }

    char* Connection::ReceiveAttachmentData(size_t length) {
      return this->ReceiveBinary(length);
    }

    int64_t Connection::ReceiveInt64() {
      char* buffer = this->ReceiveBinary(sizeof(int64_t));
      return (int64_t)buffer;
    }

    std::string Connection::ReceiveString(size_t length) {
      // assumes data is null-terminated
      char* c_data = this->ReceiveBinary(length);
      return std::string(c_data);
    }

    char* Connection::ReceiveBinary(size_t length) {
      boost::system::error_code ec;

      std::vector<char> buffer(length);
      try {
        boost::asio::read(*(this->socket_), boost::asio::buffer(buffer));
        if (ec != NULL) {
          // TODO: Handle error when socket unexpectedly closes
          this->Close();
        }
      }
      catch(std::exception& e) {
        // TODO: Handle error when socket raises exception while reading
      }

      return buffer.data();
    }

    Connection::~Connection() {
      this->Close();
    }
  }
}
