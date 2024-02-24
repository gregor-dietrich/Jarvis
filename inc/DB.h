#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Logger.h"
#include "Types.h"

namespace Jarvis
{
	class DB
	{
		std::string m_username;
		std::string m_password;
		std::string m_database;
		std::string m_hostname;
		std::string m_port;

		std::unique_ptr<mysql::tcp_ssl_connection> connect() const;
	public:
		DB(const std::string& username, const std::string& password, const std::string& database, const std::string& hostname, const std::string& port = mysql::default_port_string);
		mysql::results query(const std::string& query, const std::vector<mysql::field>& params) const;
		static std::string to_string(mysql::results result);
	};
}