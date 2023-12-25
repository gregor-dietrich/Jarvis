#include <iostream>

#include "Server.h"

namespace gcd
{
	Server::Server(const port_t port/* = 8080 */) : m_port(port)
	{
		std::cout << "Booting...\n";
	}

	void Server::handleRequest(RequestType& request, SocketType& socket)
	{
		// Prepare the response message
		ResponseType response;
		response.version(request.version());
		response.result(http::status::ok);
		response.set(http::field::server, "My HTTP Server");
		response.set(http::field::content_type, "text/plain");
		response.body() = "Hello, World!";
		response.prepare_payload();

		// Send the response to the client
		http::write(socket, response);
	}

	void Server::run()
	{
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, { tcp::v4(), m_port });
		std::cout << "Listening on port " << m_port << "\n";

		while (true) {
			SocketType socket(io_context);
			acceptor.accept(socket);
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
		}
	}
}