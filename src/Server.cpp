#include <iostream>

#include "Server.h"

namespace gcd
{
	Server::Server(const port_t port/* = 8080 */, const bool isPublic/* = true */) : m_port(port), m_public(isPublic)
	{
		std::cout << "Booting...\n";
	}

	void Server::handleRequest(RequestType& request, SocketType& socket)
	{
		const http::verb requestVerb = request.method();
		std::cout << "Received a " << requestVerb << " Request...\n";

		// Prepare the response message
		ResponseType response;
		response.version(request.version());
		response.result(http::status::ok);
		response.set(http::field::server, "My HTTP Server");
		response.set(http::field::content_type, "text/html");
		response.body() = "<!DOCTYPE html><html lang=\"en\"><head><title>Jarvis</title></head><body><div>Hello Mars!</div></body></html>";
		response.prepare_payload();

		// Send the response to the client
		http::write(socket, response);
	}

	void Server::run()
	{
		boost::asio::io_context io_context;
		tcp::acceptor acc = m_public ? tcp::acceptor(io_context, { tcp::endpoint(tcp::v4(), m_port) }) : tcp::acceptor(io_context, { tcp::v4(), m_port });

		std::cout << "Listening on port " << m_port << "\n";

		while (true) {
			try {
				SocketType socket(io_context);
				acc.accept(socket);
				std::cout << "Connection accepted.\n";

				// Read the HTTP request
				boost::beast::flat_buffer buffer;
				RequestType request;
				http::read(socket, buffer, request);

				// Handle the request
				handleRequest(request, socket);

				// Close the socket
				socket.shutdown(SocketType::shutdown_send);
				std::cout << "Socket closed.\n";
			} catch (std::exception e) {
				std::cerr << "Exception: " << e.what() << "\n";
			}
		}
	}
}