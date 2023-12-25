#pragma once

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0601
#endif

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace gcd
{
	using tcp = boost::asio::ip::tcp;
	namespace http = boost::beast::http;

	class Server
	{
		void handleRequest(http::request<http::string_body>& request, tcp::socket& socket);
	public:
		void run();
	};
}