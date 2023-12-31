#include <array>
#include <iostream>
#include <string>

#include "Logger.h"
#include "ResponseFactory.h"
#include "Util.h"

#include "Server.h"

namespace gcd
{
	Server::Server(const port_t port/* = 80 */) : m_port(port), m_alive(true)
	{
		Logger::trace("Booting...");

		m_ioContext = std::make_shared<boost::asio::io_context>();
		m_acceptor = std::make_shared<tcp::acceptor>(*m_ioContext, tcp::endpoint(tcp::v4(), m_port));

		Logger::info("Using Alias: " + ResponseFactory::setServerAlias());
		Logger::print("Listening on port " + std::to_string(m_port));
	}

	void Server::run()
	{
		while (m_alive) {
			try {
				TcpSocket socket(*m_ioContext);
				m_acceptor->accept(socket);
				handleRequest(socket);
			}
			catch (const boost::system::system_error& e) {
				if (m_alive) {
					Logger::error("Server::run(): " + std::string(e.what()));
				}
			}
			catch (const std::exception& e) {
				Logger::error("Server::run(): " + std::string(e.what()));
			}
		}
	}

	void Server::quit()
	{
		Logger::trace("Shutting down...");
		m_alive = false;
		m_ioContext->stop();
		m_acceptor->close();
	}

	void Server::handleRequest(TcpSocket& socket)
	{
		const auto connection = toString(socket);

		try {
			Logger::trace("Connection established with " + connection);

			const auto request = readRequest(socket);
			if (request != nullptr) {
				Logger::trace("Read request from " + connection);

				const auto statusCode = writeResponse(socket, *request);
				if (statusCode == -1) {
					Logger::error("Failed to send response to " + connection);
				} else {
					Logger::trace("Sent response with status " + std::to_string(statusCode) + " to " + connection);
				}
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
			if (e.code() == http::error::end_of_stream) {
				Logger::info("end of stream");
				return nullptr;
			}
			Logger::error("Server::readRequest(): " + std::string(e.what()));
		} catch (const std::exception& e) {
			Logger::error("Server::readRequest(): " + std::string(e.what()));
		}
		return nullptr;
	}

	int16_t Server::writeResponse(TcpSocket& socket, const HttpRequest& request)
	{
		try {
			const auto response = ResponseFactory::createResponse(request);
			http::write(socket, response);
			return static_cast<int16_t>(response.result());
		} catch (const std::exception& e) {
			Logger::error("Server::writeResponse(): " + std::string(e.what()));
			return -1;
		}
	}
}