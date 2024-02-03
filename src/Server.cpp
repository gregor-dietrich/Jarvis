#include <iostream>
#include <string>

#ifdef _DEBUG
	#include <chrono>
#endif

#include "Logger.h"
#include "ResponseFactory.h"
#include "Router.h"
#include "Util.h"

#include "Server.h"

namespace Jarvis
{
	Server::Server(const port_t port/* = 80 */) : m_port(port), m_alive(true), m_threads(),
		m_ioContext(std::make_shared<boost::asio::io_context>()), 
		m_acceptor(std::make_unique<tcp::acceptor>(*m_ioContext, tcp::endpoint(tcp::v4(), m_port)))
	{
		Logger::trace("Booting...");
		Logger::info("Using Alias: " + ResponseFactory::setServerAlias());

		Router::init();

		Logger::print("Listening on port " + std::to_string(m_port));
	}

	void Server::run()
	{
		listen();
		m_ioContext->run();
	}

	void Server::listen()
	{
		auto socket = std::make_shared<TcpSocket>(*m_ioContext);
		m_acceptor->async_accept(*socket, [this, socket](const boost::system::error_code error) {
			if (m_alive && !error) {
				std::thread handlerThread(&Server::handleRequest, this, socket);
				m_threads.emplace_back(std::move(handlerThread));
				listen();
			}
		});
	}

	void Server::quit()
	{
		Logger::trace("Shutting down...");
		m_alive = false;
		m_ioContext->stop();
		m_acceptor->close();
		for (auto& t : m_threads) {
			if (t.joinable()) {
				t.join();
			}
		}
	}

	void Server::handleRequest(std::shared_ptr<TcpSocket> socket)
	{
	#ifdef _DEBUG
		const auto start = std::chrono::high_resolution_clock::now();
	#endif

		const auto connection = toString(*socket);

		try {
			Logger::trace("Connection established with " + connection);

			const auto request = readRequest(*socket);
			if (request != nullptr) {
				Logger::trace("Read request from " + connection);

				const auto statusCode = writeResponse(*socket, *request);
				if (statusCode == -1) {
					Logger::error("Failed to send response to " + connection);
				} else {
					Logger::trace("Sent response with status " + std::to_string(statusCode) + " to " + connection);
				}
			}

			socket->shutdown(TcpSocket::shutdown_send);
			Logger::trace("Closed connection with " + connection);
		} catch (const std::exception& e) {
			Logger::error("Server::handleRequest @" + connection + ": " + std::string(e.what()));
		}

	#ifdef _DEBUG
		const auto stop = std::chrono::high_resolution_clock::now();
		const auto duration = std::to_string(duration_cast<std::chrono::microseconds>(stop - start).count());
		Logger::trace("Handled request from " + connection + " in " + duration + " microseconds.");
	#endif
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
			const auto statusCode = ResponseFactory::createResponse(socket, request);
			return static_cast<int16_t>(statusCode);
		} catch (const std::exception& e) {
			Logger::error("Server::writeResponse(): " + std::string(e.what()));
			return -1;
		}
	}
}