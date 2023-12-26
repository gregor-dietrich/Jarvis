#include <iostream>
#include <sstream>

#include "Server.h"

namespace gcd
{
	Server::Server(const port_t port/* = 8080 */, const bool isPublic/* = true */) : m_port(port), m_public(isPublic)
	{
		std::cout << "Booting...\n";
	}

	void Server::handleRequest(HttpRequest& request, TcpSocket& socket)
	{
		const http::verb method = request.method();
		std::cout << "Received a " << method << " Request...\n";

		// Prepare the response message
		HttpResponse response;
		response.version(request.version());
		response.set(http::field::server, "Jarvis");
		response.result(http::status::method_not_allowed);

		std::stringstream html;
		switch (method) {
		case http::verb::connect:
			break;
		case http::verb::delete_:
			break;
		case http::verb::get:
			response.set(http::field::content_type, "text/html");
			response.result(http::status::ok);

			html << "<!DOCTYPE html><html lang=\"en\"><head><title>Jarvis</title></head><body>";
			html << "<div><p>Received a " << method << " Request.</p></div>";
			html << "</body></html>";
			response.body() = html.str();
			break;
		case http::verb::head:
			break;
		case http::verb::options:
			break;
		case http::verb::patch:
			break;
		case http::verb::post:
			break;
		case http::verb::put:
			break;
		case http::verb::trace:
			break;
		}
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
				TcpSocket socket(io_context);
				acc.accept(socket);
				std::cout << "Connection accepted.\n";

				// Read the HTTP request
				boost::beast::flat_buffer buffer;
				HttpRequest request;
				http::read(socket, buffer, request);

				// Handle the request
				handleRequest(request, socket);

				// Close the socket
				socket.shutdown(TcpSocket::shutdown_send);
				std::cout << "Socket closed.\n";
			} catch (std::exception e) {
				std::cerr << "Exception: " << e.what() << "\n";
			}
		}
	}
}