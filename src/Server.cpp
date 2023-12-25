#include "Server.h"

namespace gcd
{
	void Server::handleRequest(Request& request, Socket& socket) const {
		// Prepare the response message
		Response response;
		response.version(request.version());
		response.result(http::status::ok);
		response.set(http::field::server, "My HTTP Server");
		response.set(http::field::content_type, "text/plain");
		response.body() = "Hello, World!";
		response.prepare_payload();

		// Send the response to the client
		http::write(socket, response);
	}

	void Server::run() const {
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, { tcp::v4(), m_port });

		while (true) {
			Socket socket(io_context);
			acceptor.accept(socket);

			// Read the HTTP request
			boost::beast::flat_buffer buffer;
			Request request;
			http::read(socket, buffer, request);

			// Handle the request
			handleRequest(request, socket);

			// Close the socket
			socket.shutdown(Socket::shutdown_send);
		}
	}
}