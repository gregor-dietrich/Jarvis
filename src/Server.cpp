#include <iostream>
#include <string>

#ifdef _DEBUG
	#include <chrono>
#endif

#include "Logger.h"
#include "ResponseFactory.h"
#include "Router.h"
#include "util.h"

#include "Server.h"

namespace Jarvis
{
	Server::Server(const port_t port/* = 80 */) : m_port(port), m_alive(true), m_threads(),
		m_ioContext(std::make_shared<asio::io_context>()), 
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
		auto socket = std::make_shared<tcp::socket>(*m_ioContext);
		m_acceptor->async_accept(*socket, [this, socket](const boost::system::error_code error) {
			if (m_alive && !error) {
				Thread handlerThread(&Server::handleRequest, this, socket);
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

	void Server::handleRequest(std::shared_ptr<tcp::socket> socket)
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

			socket->shutdown(tcp::socket::shutdown_send);
			Logger::trace("Closed connection with " + connection);
		} catch (const std::exception& e) {
			Logger::error("Server::handleRequest @" + connection + ": " + String(e.what()));
		}

	#ifdef _DEBUG
		const auto stop = std::chrono::high_resolution_clock::now();
		const auto duration = std::to_string(duration_cast<std::chrono::microseconds>(stop - start).count());
		Logger::trace("Handled request from " + connection + " in " + duration + " microseconds.");
	#endif
	}

	std::unique_ptr<HttpRequest> Server::readRequest(tcp::socket& socket)
	{
		try {
			beast::flat_buffer buffer;
			auto request = std::make_unique<HttpRequest>();
			http::read(socket, buffer, *request);
			return request;
		} catch (const boost::system::system_error& e) {
			if (e.code() == http::error::end_of_stream) {
				Logger::debug("end of stream");
				return nullptr;
			}
			Logger::error("Server::readRequest(): " + String(e.what()));
		} catch (const std::exception& e) {
			Logger::error("Server::readRequest(): " + String(e.what()));
		}
		return nullptr;
	}

	i16 Server::writeResponse(tcp::socket& socket, const HttpRequest& request)
	{
		try {
			const auto statusCode = ResponseFactory::createResponse(socket, request);
			return static_cast<i16>(statusCode);
		} catch (const std::exception& e) {
			Logger::error("Server::writeResponse(): " + String(e.what()));
			return -1;
		}
	}
}