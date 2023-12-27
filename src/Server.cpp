#include <exception>
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

	void Server::handleRequest(TcpSocket& socket)
	{
		ip::address ip_address;
		port_t port;
		try {
			const tcp::endpoint endpoint = socket.remote_endpoint();
			ip_address = endpoint.address();
			port = endpoint.port();

			Logger::info("Connection accepted from " + ip_address.to_string() + ":" + std::to_string(port) + ".");

			// Read Request
			boost::beast::flat_buffer buffer;
			HttpRequest request;
			http::read(socket, buffer, request);

			// Send Response
			const auto response = ResponseFactory::createResponse(request);
			http::write(socket, response);
		} catch (const boost::system::system_error& e) {
			if (e.code() == boost::beast::http::error::end_of_stream) {
				Logger::warning("end_of_stream Error in Socket " + ip_address.to_string() + ":" + std::to_string(port) + ", connection closed.");
			}
			else {
				Logger::error("Boost System Error: " + std::string(e.what()));
			}
		} catch (const std::exception& e) {
			Logger::error("Exception in Server::handleRequest: " + std::string(e.what()));
		}
		socket.shutdown(TcpSocket::shutdown_send);
		Logger::info("Socket " + ip_address.to_string() + ":" + std::to_string(port) + " closed gracefully.");
	}

	void Server::run()
	{
		boost::asio::io_context io_context;
		tcp::acceptor acc = m_public ? tcp::acceptor(io_context, { tcp::endpoint(tcp::v4(), m_port) }) : tcp::acceptor(io_context, { tcp::v4(), m_port });

		Logger::info("Listening on port " + std::to_string(m_port));
		
		while (true) {
			TcpSocket socket(io_context);
			acc.accept(socket);			

			handleRequest(socket);
		}
	}
}