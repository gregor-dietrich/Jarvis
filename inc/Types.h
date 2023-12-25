#pragma once

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0601
#endif

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace gcd
{
	namespace ip = boost::asio::ip;
	namespace http = boost::beast::http;

	using tcp = ip::tcp;
	using port_t = ip::port_type;
	using ResponseType = http::response<http::string_body>;
	using RequestType = http::request<http::string_body>;
	using SocketType = tcp::socket;
}