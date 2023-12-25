#pragma once

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0601
#endif

#include <cstdint>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace gcd
{
	using tcp = boost::asio::ip::tcp;
	namespace http = boost::beast::http;

	using Response = http::response<http::string_body>;
	using Request = http::request<http::string_body>;
	using Socket = tcp::socket;

	using i32 = int32_t;
}