#pragma once

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0601
#endif

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace Jarvis
{
	namespace asio = boost::asio;
	namespace beast = boost::beast;
	namespace http = beast::http;
	namespace ip = asio::ip;
	namespace ssl = asio::ssl;

	using HttpRequest = http::request<http::string_body>;

	using ip::tcp;
}