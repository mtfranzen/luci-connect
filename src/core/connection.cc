#include "lc2pp/core/connection.h"
#include "lc2pp/core/message.h"
#include "json/src/json.hpp"

#include <boost/asio.hpp>

#include <vector>
#include <string>

using json = nlohmann::json;

namespace lc2pp {
  namespace core {
    /**
    * The method creates an LC2 connection instance using the assigned host,
    * port and timeout.
    */
    Connection::Connection(std::string host, uint8_t port, uint timeout = 10) {
      this->host_ = host;
      this->port_ = port;
      this->timeout_ = timeout;

      // create I/O iterator from host / port
      boost::asio::ip::tcp::resolver resolver(this->io_service_);
      this->iterator_ = resolver.resolve({host, std::to_string(port)});
    }

    /**
     * The method creates a new socket to the specified address. If there is
     * an existing socket, it makes sure to close it before opening up
     * a new connection.
    */
    void Connection::Open() {
      // TODO: Add timeout and make asynchronous

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
        // TODO: log error
      }
    }

    void Connection::Close() {
      boost::system::error_code ec;
      this->socket_->close(ec);

      if (ec) {
        // TODO: log error
      }
    }

    Connection::~Connection() {
      this->Close();
    }
  }
}
