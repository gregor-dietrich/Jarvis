#pragma once

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0601
#endif

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/mysql.hpp>

namespace Jarvis
{
	namespace asio = boost::asio;
	namespace beast = boost::beast;
	namespace http = beast::http;
	namespace ip = asio::ip;
	namespace mysql = boost::mysql;
	namespace ssl = asio::ssl;

	using HttpStringResponse = http::response<http::string_body>;
	using HttpFileResponse = http::response<http::file_body>;
	using HttpRequest = http::request<http::string_body>;

	using ip::tcp;
	using port_t = decltype(std::declval<tcp::endpoint>().port());

}