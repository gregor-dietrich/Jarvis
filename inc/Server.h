#pragma once

#include "Types.h"

namespace gcd
{
	class Server
	{
		port_t m_port;
		bool m_public;

		void handleRequest(TcpSocket& socket);
		HttpResponse createResponse();
	public:
		Server(const port_t port = 8080, const bool isPublic = true);

		void run();
	};
}