#pragma once

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0601
#endif

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace Jarvis
{
	namespace http = boost::beast::http;
	using HttpStringResponse = http::response<http::string_body>;
	using HttpFileResponse = http::response<http::file_body>;
	using HttpRequest = http::request<http::string_body>;

	namespace ip = boost::asio::ip;
	using ip::tcp;
	using port_t = decltype(std::declval<tcp::endpoint>().port());
}