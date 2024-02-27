#pragma once

#include <memory>
#include <vector>

#include "aliases.h"

namespace Jarvis
{
	using port_t = decltype(std::declval<tcp::endpoint>().port());

	class Server
	{
		port_t m_port;
		bool m_alive;
		std::vector<std::thread> m_threads;
		std::shared_ptr<asio::io_context> m_ioContext;
		std::unique_ptr<tcp::acceptor> m_acceptor;

		void listen();
		void handleRequest(std::shared_ptr<tcp::socket> socket);
		std::unique_ptr<HttpRequest> readRequest(tcp::socket& socket);
		i16 writeResponse(tcp::socket& socket, const HttpRequest& request);
	public:
		Server(const port_t port = 80);

		void run();
		void quit();
	};
}