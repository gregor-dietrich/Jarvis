#include <iostream>
#include <string>

#include "Logger.h"
#include "ResponseFactory.h"
#include "Server.h"

namespace gcd
{
	Server::Server(const port_t port/* = 8080 */, const bool isPublic/* = true */) : m_port(port), m_public(isPublic)
	{
		Logger::info("Booting up...");
	}

	void Server::handleRequest(const HttpRequest& request, TcpSocket& socket)
	{
		const auto response = ResponseFactory::createResponse(request);
		http::write(socket, response);
	}

	void Server::run()
	{
		boost::asio::io_context io_context;
		tcp::acceptor acc = m_public ? tcp::acceptor(io_context, { tcp::endpoint(tcp::v4(), m_port) }) : tcp::acceptor(io_context, { tcp::v4(), m_port });

		Logger::info("Listening on port " + std::to_string(m_port));

		while (true) {
			TcpSocket socket(io_context);
			acc.accept(socket);

			const tcp::endpoint endpoint = socket.remote_endpoint();
			const ip::address ip_address = endpoint.address();
			const port_t port = endpoint.port();
			Logger::info("Connection accepted from " + ip_address.to_string() + ":" + std::to_string(port) + ".");

			boost::beast::flat_buffer buffer;
			HttpRequest request;
			http::read(socket, buffer, request);

			handleRequest(request, socket);

			socket.shutdown(TcpSocket::shutdown_send);
			Logger::info("Socket with " + ip_address.to_string() + ":" + std::to_string(port) + " has shut down gracefully.");
		}
	}
}