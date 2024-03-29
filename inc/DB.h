#pragma once

#include <memory>
#include <vector>

#include "aliases.h"

#include <boost/mysql.hpp>

namespace Jarvis
{
	namespace mysql = boost::mysql;
	
	class DB
	{
		String m_username;
		String m_password;
		String m_database;
		String m_hostname;
		String m_port;
	public:
		DB(const String& username, const String& password, const String& database, const String& hostname, const String& port = mysql::default_port_string);
		mysql::results query(const String& query, const std::vector<mysql::field>& params) const;
		static String to_string(mysql::results result);
	};
}