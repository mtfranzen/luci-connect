#ifndef luciconnect_ASIO_H
#define luciconnect_ASIO_H

#ifdef ASIO_STANDALONE
#include <asio.hpp>
#else
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/connect.hpp>
namespace asio
{
	using namespace boost::asio;
	using namespace boost::system;
}
#endif

#endif
