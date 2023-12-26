#pragma once

#include <fstream>
#include <memory>
#include <string>

namespace gcd
{
	class Logger
	{
		enum class LogLevel {
			ERRORMSG,
			WARNINGMSG,
			INFOMSG
		};

		enum class color {
#ifdef _WIN32
			green = 2,
			red = 4,
			yellow = 6,
			light_green = 10,
			light_red = 12,
			light_yellow = 14,
#else
			red,
			green,
			yellow,
#endif
			DEFAULT_COLOR =
#ifdef _WIN32
			7
#else
			6
#endif
		};

		Logger() = default;

		static std::string getTime();
		static std::string format(const LogLevel messageType, const std::string& message);
		static void setColor(const bool isError, const color c = color::DEFAULT_COLOR);

		void print(const LogLevel messageType, const std::string& message);
		void log(const std::string& message);
	public:
		static std::shared_ptr<Logger>	get();
		static const LogLevel logLevel = LogLevel::INFOMSG;

		void info(const std::string& message);
		void warning(const std::string& message, const bool colored = true);
		void error(const std::string& message, const bool colored = true);
	};
}