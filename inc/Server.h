#pragma once

#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

#include "Types.h"

namespace Jarvis
{
	class Server
	{
		port_t m_port;
		bool m_alive;
		std::vector<std::thread> m_threads;
		std::shared_ptr<boost::asio::io_context> m_ioContext;
		std::unique_ptr<tcp::acceptor> m_acceptor;

		void listen();
		void handleRequest(std::shared_ptr<tcp::socket> socket);
		std::unique_ptr<HttpRequest> readRequest(tcp::socket& socket);
		int16_t writeResponse(tcp::socket& socket, const HttpRequest& request);
	public:
		Server(const port_t port = 80);

		void run();
		void quit();
	};
}