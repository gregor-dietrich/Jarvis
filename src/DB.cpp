#include "DB.h"

#include <boost/mysql/src.hpp>
#include <iostream>

namespace Jarvis
{
	constexpr auto _HOST = "localhost";
	constexpr auto _PORT = mysql::default_port_string;
	constexpr auto _USER = "root";
	constexpr auto _PASS = "";
	constexpr auto _DB = "test";

	mysql::results DB::query(const std::string& query)
	{
		mysql::results result;
		try {
			// The execution context, required to run I/O operations.
			asio::io_context ctx;

			// The SSL context, required to establish TLS connections.
			// The default SSL options are good enough for us at this point.
			ssl::context ssl_ctx(ssl::context::tls_client);

			// Represents a connection to the MySQL server.			
			mysql::tcp_ssl_connection m_connection(ctx.get_executor(), ssl_ctx);

			// Resolve the hostname to get a collection of endpoints
			tcp::resolver resolver(ctx.get_executor());
			auto endpoints = resolver.resolve(_HOST, _PORT);

			// The username, password and database to use
			mysql::handshake_params params(_USER, _PASS, _DB);

			// TO DO: Use SSL!
			// Disable SSL
			params.set_ssl(mysql::ssl_mode::disable);

			// Connect to the server using the first endpoint returned by the resolver
			m_connection.connect(*endpoints.begin(), params);

			// Issue the SQL query to the server
			m_connection.execute(query, result);
		}
		catch (const std::exception& err) {
			Logger::error(err.what());
		}
		return result;
	}

	DB::DB()
	{
	}
}