#include "lc2pp/core/connection.h"

using std::placeholders::_1;
using std::placeholders::_2;

namespace lc2pp {
  namespace core {
    // TODO: Make connection class asynchronous

    Connection::Connection(std::string host, uint16_t port) {
      this->host_ = host;
      this->port_ = port;
      this->is_connected_ = false;

      // create I/O iterator from host / port
      this->io_service_ = new asio::io_service();
      asio::ip::tcp::resolver resolver(*this->io_service_);
      this->iterator_ = resolver.resolve({host, std::to_string(port)});
      this->acceptor_ = new asio::ip::tcp::acceptor(*this->io_service_);
    }

/// OPEN AND CLOSE

    void Connection::Open() {
      // Checking if connected
      if (this->is_connected_) {
        LOG(ERROR) << "Socket already opened.";
        throw "Socket already opened.";
      }

      // Creating socket
      this->socket_ = new asio::ip::tcp::socket(*this->io_service_);

      // Connecting...
      try {
        LOG(INFO) << "Connecting to " << std::string(this->host_) << ":" << std::to_string(this->port_);
        asio::connect(*this->socket_, this->iterator_);
        asio::socket_base::keep_alive option(true);
        this->socket_->set_option(option);
        this->is_connected_ = true;
      }
      catch (std::exception& err) {
        LOG(ERROR) << "Connection could not be established.";
        throw "Connection could not be established.";
      }

      // Starting threads...
      try {
        LOG(INFO) << "Starting threads.";
        this->thread_waiting_for_events_ = new std::thread(&Connection::WaitForHandlers, shared_from_this());
        this->thread_waiting_for_events_ = new std::thread(&Connection::WaitForReceival, shared_from_this());
      }
      catch (std::exception& err) {
        LOG(ERROR) << "An occured while starting threads: " << err.what();
      }
    }

    void Connection::Close() {
      // Check if we're connected
      if (!this->is_connected_) {
        LOG(WARNING) << "Socket already closed.";
        return;
      }

      // Pulling back worker thread
      this->mutex_waiting_for_events_.lock();
      this->mutex_waiting_for_receival_.lock();
      this->is_connected_ = false; // notifies waiting threads to finish execution
      this->mutex_waiting_for_receival_.unlock();
      this->mutex_waiting_for_events_.unlock();

      this->thread_waiting_for_events_->join();
      this->thread_waiting_for_receival_->join();

      // Closing connection
      LOG(INFO) << "Closing connection.";
      asio::error_code ec;
      this->socket_->close(ec);

      if (ec) {
        LOG(ERROR) << "An error occured while closing connection: " << ec;
        throw "An error occured while closing connection.";
      }
      else {
        LOG(INFO) << "Connection closed.";
      }
    }

/// THREADING

    void Connection::WaitForHandlers() {
      while (this->is_connected_) {
        this->mutex_waiting_for_events_.lock();
        this->io_service_->run();
        this->io_service_->reset();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        this->mutex_waiting_for_events_.unlock();
      }
    }

    void Connection::WaitForReceival() {
      while (this->is_connected_) {
        this->mutex_waiting_for_receival_.lock();
        this->ReceiveAsync();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        this->mutex_waiting_for_receival_.unlock();
      }
    }

    void Connection::SendHandler(const asio::error_code& error, std::size_t size_transferred) {
        if (!error)
        {
          LOG(INFO) << "Buffer sent successfully.";
        }
        else
        {
          LOG(ERROR) << "An error occured when sending a message: " << error;
          throw "An error occured when sending a message.";
        }
      }

    void Connection::RegisterDelegate(MessageType messagetype, std::function<void(Message*)> callback) {
      // TODO: Write registration method for message delegations
    }

    void Connection::DelegateMessage(Message* message) {
      // TODO: Write dmethod performating the message delegations
    }

/// SENDING: Stepwise

    void Connection::SendAsync(Message* message) {
      LOG(INFO) << "Starting to send Message " << message->GetHeader().dump();
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

      LOG(DEBUG) << "Sending header size " << std::to_string(header_size);
      this->SendInt64(header_size);
    }

    void Connection::SendBodySize(Message* message) {
      int64_t body_size = 8;
      for (size_t i = 0; i < message->GetNumAttachments(); i++)
        body_size += message->GetAttachment(i)->size + 8;

      LOG(DEBUG) << "Sending body size " << std::to_string(body_size);
      this->SendInt64(body_size);
    }

    void Connection::SendHeader(Message* message) {
      std::string header = message->GetHeader().dump();

      LOG(DEBUG) << "Sending header " << header;
      this->SendString(header);
    }

    void Connection::SendNumberOfAttachments(Message* message) {
      int64_t num_attachments = message->GetNumAttachments();

      LOG(DEBUG) << "Sending number of attachments " << std::to_string(num_attachments);
      this->SendInt64(num_attachments);
    }

    void Connection::SendAttachmentSize(Message* message, size_t index) {
      int64_t attachment_size = message->GetAttachment(index)->size;

      LOG(DEBUG) << "Sending attachment size #" << std::to_string(index+1) << ": " << std::to_string(attachment_size);
      this->SendInt64(attachment_size);
    }

    void Connection::SendAttachment(Message* message, size_t index) {
      std::string attachment(message->GetAttachment(index)->data);

      LOG(DEBUG) << "Sending attachment #" << std::to_string(index+1);
      this->SendString(attachment);
    }

/// SENDING: Primitives

    void Connection::SendInt64(int64_t data) {
      // we are converting the data here to a constant so both SendInt64 and
      // SendString share the same buffer output type (const_buffers_1).
      char c_data[sizeof(int64_t)];
      std::memcpy(c_data, &data, sizeof(int64_t));
      std::reverse(c_data, c_data + sizeof(int64_t));
      const char* r_data = c_data;

      this->SendBuffer(asio::buffer(r_data, sizeof(int64_t)));
    }

    void Connection::SendString(std::string data) {
      const char* c_data = data.c_str();
      this->SendBuffer(asio::buffer(c_data, data.size()));
    }

    void Connection::SendBuffer(asio::const_buffers_1 data) {
      if (!this->is_connected_) {
        LOG(ERROR) << "Trying to send while connection is closed.";
        throw "Trying to send a message while connection is closed.";
      }

      try {
        //auto self(this->shared_from_this());
        asio::async_write(*this->socket_, data, std::bind(&Connection::SendHandler, shared_from_this(), _1, _2));
      }
      catch (std::exception& err) {
        LOG(ERROR) << "An error occured when sending a message: " << err.what();
        throw "An error occured when sending a message.";
      }
    }

/// RECEIVING: Stepwise

    void Connection::ReceiveAsync() {
      // TODO: Make receival async
      int64_t validation_size = 8; // see LC2 documentation

      // Receive header
      int64_t header_size = this->ReceiveHeaderSize();
      int64_t body_size = this->ReceiveBodySize();

      json header;
      try {
        header = json::parse(this->ReceiveHeader(header_size));
      }
      catch (std::exception& err) {
        LOG(WARNING) << "Message parsing failed: header corrupted.";
        throw "Message parsing failed: header corrupted.";
      }

      // Construct message
      Message* message = new Message(header);

      // Add attachments
      int64_t num_attachments = this->ReceiveNumberOfAttachments();
      for (int i = 0; i < num_attachments; i++) {
        size_t attachment_size = this->ReceiveAttachmentSize();
        validation_size += attachment_size + 8;
        char* attachment_data = this->ReceiveAttachmentData(attachment_size);
        Attachment attachment = {attachment_size, attachment_data};
        message->AddAttachment(&attachment);
      }

      if (body_size != validation_size) {
        LOG(WARNING) << "Message validation failed. Entering panic mode.";
        // TODO: Enter panic mode when message validation fails
      }
    }

    int64_t Connection::ReceiveHeaderSize() {
      LOG(DEBUG) << "Receiving header size";
      int64_t header_size = this->ReceiveInt64();
      LOG(DEBUG) << "Received header size: " << std::to_string(header_size);
      return header_size;
    }

    int64_t Connection::ReceiveBodySize() {
      LOG(DEBUG) << "Receiving body size";
      int64_t body_size = this->ReceiveInt64();
      LOG(DEBUG) << "Received body size: " << std::to_string(body_size);
      return body_size;
    }

    std::string Connection::ReceiveHeader(size_t length) {
      LOG(DEBUG) << "Receiving header of length " << std::to_string(length);
      std::string header = this->ReceiveString(length);
      LOG(DEBUG) << "Received header: " << header;
      return header;
    }

    int64_t Connection::ReceiveNumberOfAttachments() {
      LOG(DEBUG) << "Receiving number of attachments";
      int64_t  num_attachments = this->ReceiveInt64();
      LOG(DEBUG) << "Received number of attachments: " << std::to_string(num_attachments);
      return num_attachments;
    }

    int64_t Connection::ReceiveAttachmentSize() {
      LOG(DEBUG) << "Receiving attachment size";
      int64_t attachment_size = this->ReceiveInt64();
      LOG(DEBUG) << "Received attachment size: " << std::to_string(attachment_size);
      return attachment_size;
    }

    char* Connection::ReceiveAttachmentData(size_t length) {
      LOG(DEBUG) << "Receiving attachment of size " << std::to_string(length);
      std::string attachment = this->ReceiveString(length);
      LOG(DEBUG) << "Received attachment.";
      const char* attachment_binary = attachment.c_str();
      return (char*)attachment_binary;
    }

/// RECEIVING: Primitives

    int64_t Connection::ReceiveInt64() {
      std::string buffer = this->ReceiveString(sizeof(int64_t));
      const char* c_buffer = buffer.c_str();

      int64_t tmp;
      std::memcpy(&tmp, c_buffer, sizeof(int64_t));
      char c_data[sizeof(int64_t)];
      std::memcpy(c_data, &tmp, sizeof(int64_t));
      std::reverse(c_data, c_data + sizeof(int64_t));
      std::memcpy(&tmp, c_data, sizeof(int64_t));

      return tmp;
    }

    std::string Connection::ReceiveString(size_t length) {
      if (!this->is_connected_) {
        LOG(ERROR) << "Trying to receive a message while connection is closed.";
        throw "Trying to receive a message while connection is closed.";
      }

      asio::error_code ec;

      std::vector<char> buffer(length);
      try {
        asio::read(*(this->socket_), asio::buffer(buffer));
      }
      catch(std::exception& e) {
        LOG(ERROR) << "An error occured while trying to receive.";
        throw "An error occured while trying to receive.";
      }

      // convert output to char pointer
      std::string s_data(buffer.begin(), buffer.end());

      return s_data;
    }

/// DESTRUCTOR

    Connection::~Connection() {
      // TODO: Free memory of socket, io_service in connection desctructor
      this->Close();
    }
  }
}
