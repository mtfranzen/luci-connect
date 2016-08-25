#include "lc2pp/core/connection.h"

using std::placeholders::_1;
using std::placeholders::_2;

namespace lc2pp {
  namespace core {
    Connection::Connection(std::string host, uint16_t port) {
      this->host_ = host;
      this->port_ = port;

      // initial state
      this->is_connected_ = false;
      this->is_disconnecting_ = false;
      this->is_receiving_ = false;

      // create I/O iterator from host / port
      this->io_service_ = new asio::io_service();
      asio::ip::tcp::resolver resolver(*this->io_service_);
      this->iterator_ = resolver.resolve({host, std::to_string(port)});
      this->acceptor_ = new asio::ip::tcp::acceptor(*this->io_service_);
    }

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
        this->thread_waiting_for_receival_ = new std::thread(&Connection::WaitForReceival, shared_from_this());
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

      LOG(INFO) << "Closing connection.";

      // Notify waiting threads to stop execution
      this->is_disconnecting_ = true;
      this->io_service_->stop();

      // Pulling back threads
      this->thread_waiting_for_events_->join();
      this->thread_waiting_for_receival_->join();

      // Closing connection
      asio::error_code ec;
      this->socket_->cancel();
      this->socket_->close(ec);

      if (ec) {
        LOG(ERROR) << "An error occured while closing connection: " << ec;
        throw "An error occured while closing connection.";
      }
      else {
        LOG(INFO) << "Connection closed.";
        this->is_connected_ = false;
        this->is_disconnecting_ = false;
      }
    }

    void Connection::RegisterDelegate(std::function<void(Message)> callback) {
      this->receive_handlers_.push_back(callback);
    }

    void Connection::SendAsync(Message* message) {
      // receiving procedure.
      LOG(INFO) << "Starting to send Message " << message->GetHeader().dump();
      
      if (!this->is_connected_) {
        LOG(ERROR) << "Trying to send while not connected.";
        throw "Trying to send while not connected.";
      }

      this->SendHeaderSize(message);
      this->SendBodySize(message);
      this->SendHeader(message);
      this->SendNumberOfAttachments(message);
      for (size_t i = 0; i < message->GetNumAttachments(); i++) {
        this->SendAttachmentSize(message, i);
        this->SendAttachment(message, i);
      }
    }

    void Connection::WaitForHandlers() {
      // Waits for asynchronous events, like the finishing of the Send() procedure
      // or the receival of some bytes from the socket. Therefore, the sending
      // and receiving take place in this thread.
      while (!this->is_disconnecting_) {
        this->io_service_->run();
        this->io_service_->reset();
      }
    }

    void Connection::WaitForReceival() {
      // Tries to receive a message in a loop. This method is spawned
      // in a new thread during Connection::Open()
      while (!this->is_disconnecting_) {
        if (!this->is_receiving_)
          this->ReceiveAsync();
      }
    }

    void Connection::HandleBufferSent(const asio::error_code& error, std::size_t size_transferred) {
      // TODO: Make Sending procedure asynchronously chained like receive
      if (!error)
      {
        LOG(INFO) << "Buffer sent successfully.";
      }
      else
      {
        LOG(ERROR) << "An error occured when sending a message: " << error;
      }
    }

    void Connection::HandleMessageReceived() {
      // Note that this method is called in the i/o handling thread. So no
      // send or receive is being handled at the same time!
      LOG(DEBUG) << "Message Received!";
      LOG(DEBUG) << "\tNumber of Attachments: " << std::to_string(this->recv_message_->GetNumAttachments());
      LOG(DEBUG) << "\tHeader: " << this->recv_message_->GetHeader().dump();
      LOG(DEBUG) << "\tBody-Size: " << std::to_string(this->ParseInt64(this->recv_buf_body_size_));
      LOG(DEBUG) << "\tValidation-Size: " << std::to_string(this->recv_validation_size_);

      // Check LC2 validation method
      if (this->recv_validation_size_ != (size_t)this->ParseInt64(this->recv_buf_body_size_)) {
        LOG(ERROR) << "Message validation failed: Wrong body size.";
        throw "Message validation failed: Wrong body size.";
      }

      // create copy of message
      Message received_message = *this->recv_message_;

      // run registered delegates
      for (std::function<void(Message)> handler : this->receive_handlers_)
        handler(received_message);


      // This variable is set at the beginning of a receive operation
      // and avoids that is put on the stack an infinite number of times
      // however, receive() is blocking the i/o thread anyway
      this->is_receiving_ = false;

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
      }

      try {
        //auto self(this->shared_from_this());
        asio::async_write(*this->socket_, data, std::bind(&Connection::HandleBufferSent, shared_from_this(), _1, _2));
      }
      catch (std::exception& err) {
        LOG(ERROR) << "An error occured when sending a message: " << err.what();
      }
    }

    void Connection::ReceiveAsync() {
      // The ReceiveAsync class creates a chain of receive parts. First
      // ReceiveHeaderSize which runs ReceiveBodySize upon successful
      // receival. This chain is performed until the message has successfully
      // been received. Finally, in FinalizeMessage() the message is delegated
      // to the nodes.
      this->is_receiving_ = true;
      this->ReceiveHeaderSize();
    }

    void Connection::ReceiveHeaderSize() {
      LOG(DEBUG) << "Receiving header size";
      this->recv_buf_header_size_ = std::vector<char>(sizeof(int64_t));
      asio::async_read(*this->socket_, asio::buffer(this->recv_buf_header_size_), std::bind(&Connection::ReceiveBodySize, shared_from_this(), _1, _2));
    }

    void Connection::ReceiveBodySize(const asio::error_code& error, size_t bytes_transferred) {
      if (error) {
        LOG(ERROR) << "An error occured while reading the header size.";
      }
      LOG(DEBUG) << "Receiving body size";
      this->recv_buf_body_size_ = std::vector<char>(sizeof(int64_t));
      asio::async_read(*this->socket_, asio::buffer(this->recv_buf_body_size_), std::bind(&Connection::ReceiveHeader, shared_from_this(), _1, _2));
    }

    void Connection::ReceiveHeader(const asio::error_code& error, size_t bytes_transferred) {
      if (error) {
        LOG(ERROR) << "An error occured while reading the body size.";
      }
      LOG(DEBUG) << "Receiving header";
      this->recv_validation_size_ = 8;
      this->recv_buf_header_ = std::vector<char>(this->ParseInt64(this->recv_buf_header_size_));
      asio::async_read(*this->socket_, asio::buffer(this->recv_buf_header_), std::bind(&Connection::ReceiveNumberOfAttachments, shared_from_this(), _1, _2));
    }

    void Connection::ReceiveNumberOfAttachments(const asio::error_code& error, size_t bytes_transferred) {
      if (error) {
        LOG(ERROR) << "An error occured while reading the header.";
      }

      json header;
      try {
        header = json::parse(this->ParseString(this->recv_buf_header_));
      }
      catch (std::exception& err) {
        LOG(WARNING) << "Message parsing failed: header corrupted.";
      }
      // Construct message
      this->recv_message_ = new Message(header);

      LOG(DEBUG) << "Receiving number of attachments";
      this->recv_buf_num_attachments_ = std::vector<char>(sizeof(int64_t));
      asio::async_read(*this->socket_, asio::buffer(this->recv_buf_num_attachments_), std::bind(&Connection::ReceiveAttachmentSize, shared_from_this(), _1, _2));
    }

    void Connection::ReceiveAttachmentSize(const asio::error_code& error, size_t bytes_transferred) {
      if (error) {
        LOG(ERROR) << "An error occured while reading the number of attachments.";
      }

      if (this->recv_message_->GetNumAttachments() < (size_t)this->ParseInt64(this->recv_buf_num_attachments_)) {
        LOG(DEBUG) << "Receiving attachment size";
        this->recv_buf_attachment_size_ = std::vector<char>(sizeof(int64_t));
        asio::async_read(*this->socket_, asio::buffer(this->recv_buf_attachment_size_), std::bind(&Connection::ReceiveAttachmentData, shared_from_this(), _1, _2));
      }
      else {
        this->HandleMessageReceived();
      }
    }

    void Connection::ReceiveAttachmentData(const asio::error_code& error, size_t bytes_transferred) {
      if (error) {
        LOG(ERROR) << "An error occured while reading an attachment size.";
      }

      LOG(DEBUG) << "Receiving attachment";
      this->recv_validation_size_ += 8 + (size_t)this->ParseInt64(this->recv_buf_attachment_size_);
      this->recv_buf_attachment_data_ = std::vector<char>(this->ParseInt64(this->recv_buf_attachment_size_));
      asio::async_read(*this->socket_, asio::buffer(this->recv_buf_attachment_data_), std::bind(&Connection::ReceiveNextAttachment, shared_from_this(), _1, _2));
    }

    void Connection::ReceiveNextAttachment(const asio::error_code& error, size_t bytes_transferred) {
      if (error) {
        LOG(ERROR) << "An error occured while reading attachment data.";
      }

      size_t attachment_size = this->ParseInt64(this->recv_buf_attachment_size_);
      const char* attachment_data = this->ParseString(this->recv_buf_attachment_data_).c_str();
      Attachment attachment = {attachment_size, attachment_data};
      this->recv_message_->AddAttachment(&attachment);

      if (this->recv_message_->GetNumAttachments() < (size_t)this->ParseInt64(this->recv_buf_num_attachments_)) {
        LOG(DEBUG) << "Receiving attachment size";
        this->recv_buf_attachment_size_ = std::vector<char>(sizeof(int64_t));
        asio::async_read(*this->socket_, asio::buffer(this->recv_buf_attachment_size_), std::bind(&Connection::ReceiveAttachmentData, shared_from_this(), _1, _2));
      }
      else {
        this->HandleMessageReceived();
      }
    }

    int64_t Connection::ParseInt64(std::vector<char> buffer) {
      std::string buffer_s = this->ParseString(buffer);
      const char* c_buffer = buffer_s.c_str();

      int64_t tmp;
      std::memcpy(&tmp, c_buffer, sizeof(int64_t));
      char c_data[sizeof(int64_t)];
      std::memcpy(c_data, &tmp, sizeof(int64_t));
      std::reverse(c_data, c_data + sizeof(int64_t));
      std::memcpy(&tmp, c_data, sizeof(int64_t));

      return tmp;
    }

    std::string Connection::ParseString(std::vector<char> buffer) {
      std::string s_data(buffer.begin(), buffer.end());
      return s_data;
    }

    Connection::~Connection() {
      this->Close();
    }
  }
}
