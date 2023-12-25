#pragma once

#include "Types.h"

namespace gcd
{
	class Server
	{
		port_t m_port;

		void handleRequest(RequestType& request, SocketType& socket);
	public:
		explicit Server(const port_t port = 8080);

		void run();
	};
}