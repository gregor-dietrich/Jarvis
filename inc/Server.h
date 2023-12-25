#pragma once

#include "Types.h"

namespace gcd
{
	class Server
	{
		const i32 m_port;

		void handleRequest(Request& request, Socket& socket) const;
	public:
		Server(const i32 port = 8080) : m_port(port) {};

		void run() const;
	};
}