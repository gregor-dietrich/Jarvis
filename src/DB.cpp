#include "DB.h"

#include <boost/mysql/src.hpp>
#include <sstream>

namespace Jarvis
{
	DB::DB(const std::string& username, const std::string& password, const std::string& database, 
		const std::string& hostname, const std::string& port /*= mysql::default_port_string*/)
		: m_username(username), m_password(password), m_database(database), m_hostname(hostname), m_port(port)
	{
	}

	mysql::results DB::query(const std::string& query) const
	{
		mysql::results result;
		try {
			// Handshake Params
			mysql::handshake_params params(m_username, m_password, m_database);

			// TO DO: Use SSL!
			// Disable SSL
			params.set_ssl(mysql::ssl_mode::disable);

			// The execution context, required to run I/O operations.
			asio::io_context ctx;

			// The SSL context, required to establish TLS connections.
			// The default SSL options are good enough for us at this point.
			ssl::context ssl_ctx(ssl::context::tls_client);

			// Represents a connection to the MySQL server.
			mysql::tcp_ssl_connection m_connection(ctx.get_executor(), ssl_ctx);

			// Resolve the hostname to get a collection of endpoints
			tcp::resolver resolver(ctx.get_executor());
			auto endpoints = resolver.resolve(m_hostname, m_port);

			// Connect to the server using the first endpoint returned by the resolver
			m_connection.connect(*endpoints.begin(), params);

			// Issue the SQL query to the server
			m_connection.execute(query, result);

			// Close the connection
			m_connection.close();
		}
		catch (const std::exception& err) {
			Logger::error(err.what());
		}
		return result;
	}

	std::string DB::to_string(mysql::results result)
	{
		std::stringstream ss;
		if (result.has_value()) {
			for (const auto& row : result.rows()) {
				for (const auto& col : row) {
					ss << col << '\t';
				}
				ss << '\n';
			}
		}
		return ss.str();
	}
}