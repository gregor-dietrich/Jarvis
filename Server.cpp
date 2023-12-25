#include "Server.h"

namespace gcd
{
	void Server::handleRequest(http::request<http::string_body>& request, tcp::socket& socket) {
		// Prepare the response message
		http::response<http::string_body> response;
		response.version(request.version());
		response.result(http::status::ok);
		response.set(http::field::server, "My HTTP Server");
		response.set(http::field::content_type, "text/plain");
		response.body() = "Hello, World!";
		response.prepare_payload();

		// Send the response to the client
		boost::beast::http::write(socket, response);
	}

	void Server::run() {
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, { tcp::v4(), 8080 });

		while (true) {
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			// Read the HTTP request
			boost::beast::flat_buffer buffer;
			http::request<http::string_body> request;
			boost::beast::http::read(socket, buffer, request);

			// Handle the request
			handleRequest(request, socket);

			// Close the socket
			socket.shutdown(tcp::socket::shutdown_send);
		}
	}
}