#pragma once

#include "Types.h"

namespace gcd
{
	class Server
	{
		port_t m_port;

		void handleRequest(TcpSocket& socket);
		HttpResponse createResponse();
	public:
		Server(const port_t port = 80);

		void run();
	};
}