#include <array>
#include <iostream>
#include <string>

#include "Logger.h"
#include "ResponseFactory.h"
#include "Util.h"

#include "Server.h"

namespace gcd
{
	Server::Server(const port_t port/* = 80 */) : m_port(port)
	{
		Logger::trace("Booting...");

		Logger::info("Using Alias: " + ResponseFactory::setServerAlias());
	}

	void Server::run()
	{
		boost::asio::io_context io_context;
		tcp::acceptor acc = tcp::acceptor(io_context, { tcp::endpoint(tcp::v4(), m_port) });

		Logger::print("Listening on port " + std::to_string(m_port));

		while (true) {
			TcpSocket socket(io_context);
			acc.accept(socket);
			handleRequest(socket);
		}

		Logger::trace("Shutting down...");
	}

	void Server::handleRequest(TcpSocket& socket)
	{
		std::string connection;

		try {
			const tcp::endpoint endpoint = socket.remote_endpoint();
			connection = endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
			Logger::trace("Connection established with " + connection);

			const auto request = readRequest(socket);
			if (request != nullptr) {
				Logger::trace("Read request from " + connection);

				writeResponse(socket, *request);
				Logger::trace("Sent response to " + connection);
			}

			socket.shutdown(TcpSocket::shutdown_send);
			Logger::trace("Closed connection with " + connection);
		} catch (const std::exception& e) {
			Logger::error("Server::handleRequest @" + connection + ": " + std::string(e.what()));
		}
	}

	std::unique_ptr<HttpRequest> Server::readRequest(TcpSocket& socket)
	{
		try {
			boost::beast::flat_buffer buffer;
			auto request = std::make_unique<HttpRequest>();
			http::read(socket, buffer, *request);
			return request;
		} catch (const boost::system::system_error& e) {
			const auto what = "Server::writeResponse: " + std::string(e.what());
			if (e.code() == http::error::end_of_stream) {
				Logger::info(what);
				return nullptr;
			}
			Logger::warning(what);
		}
		return nullptr;
	}

	void Server::writeResponse(TcpSocket& socket, const HttpRequest& request)
	{
		const auto response = ResponseFactory::createResponse(request);
		http::write(socket, response);
	}
}