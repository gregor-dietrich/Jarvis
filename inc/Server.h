#pragma once

#include <cstdint>
#include <memory>

#include "Types.h"

namespace gcd
{
	class Server
	{
		port_t m_port;
		bool m_alive;
		std::shared_ptr<boost::asio::io_context> m_ioContext;
		std::shared_ptr<tcp::acceptor> m_acceptor;

		void handleRequest(TcpSocket& socket);
		std::unique_ptr<HttpRequest> readRequest(TcpSocket& socket);
		int16_t writeResponse(TcpSocket& socket, const HttpRequest& request);
	public:
		Server(const port_t port = 80);

		void run();
		void quit();
	};
}