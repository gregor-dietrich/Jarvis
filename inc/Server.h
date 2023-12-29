#pragma once

#include <memory>

#include "Types.h"

namespace gcd
{
	class Server
	{
		port_t m_port;

		void handleRequest(TcpSocket& socket);
		std::unique_ptr<HttpRequest> readRequest(TcpSocket& socket);
		void writeResponse(TcpSocket& socket, const HttpRequest& request);
	public:
		Server(const port_t port = 80);

		void run();
	};
}