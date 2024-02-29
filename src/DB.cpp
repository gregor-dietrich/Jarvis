#include <sstream>

#include "DB.h"
#include "Logger.h"

#include <boost/mysql/src.hpp>

namespace Jarvis
{
	DB::DB(const String& username, const String& password, const String& database, 
		const String& hostname, const String& port /*= mysql::default_port_string*/)
		: m_username(username), m_password(password), m_database(database), m_hostname(hostname), m_port(port)
	{
	}

	std::unique_ptr<mysql::tcp_ssl_connection> DB::connect() const
	{
		// Handshake Params
		mysql::handshake_params dbParams(m_username, m_password, m_database);

		// TO DO: Use SSL!
		// Disable SSL
		dbParams.set_ssl(mysql::ssl_mode::disable);

		// The execution context, required to run I/O operations.
		asio::io_context ctx;

		// The SSL context, required to establish TLS connections.
		// The default SSL options are good enough for us at this point.
		ssl::context ssl_ctx(ssl::context::tls_client);

		// Represents a connection to the MySQL server.
		auto conn = std::make_unique<mysql::tcp_ssl_connection>(ctx.get_executor(), ssl_ctx);

		// Resolve the hostname to get a collection of endpoints
		tcp::resolver resolver(ctx.get_executor());
		auto endpoints = resolver.resolve(m_hostname, m_port);

		// Connect to the server using the first endpoint returned by the resolver
		conn->connect(*endpoints.begin(), dbParams);

		return conn;
	}

	mysql::results DB::query(const String& query, const std::vector<mysql::field>& params) const
	{
		mysql::results result;
		try {
			auto conn = connect();

			 // Prepare the statement
        	mysql::statement stmt = conn->prepare_statement(query);

			// Issue the SQL query to the server
			conn->execute(stmt.bind(params.begin(), params.end()), result);

			// Close the connection
			conn->close();
		}
		catch (const std::exception& err) {
			Logger::error(err.what());
		}
		return result;
	}

	String DB::to_string(mysql::results result)
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