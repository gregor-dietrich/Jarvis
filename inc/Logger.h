#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

namespace gcd
{
	enum class LogLevel {
		ERRORMSG,
		WARNINGMSG,
		INFOMSG,

		MINLEVEL = 0,
		MAXLEVEL = 2
	};

	class Logger
	{
		enum class color {
#ifdef _WIN32
			red = 4,
			green = 2,
			yellow = 6,
#else
			red,
			green,
			yellow,
#endif
			DEFAULT_COLOR =
#ifdef _WIN32
			7 // light_gray
#else
			6 // white
#endif
		};

		static LogLevel logLevel;
		static std::string logFilePath;

		Logger() = default;

		static void createDirectory(const std::string& path);
		static std::string getTime();
		static std::string format(const LogLevel messageType, const std::string& message);
		static void setColor(const bool isError, const color c = color::DEFAULT_COLOR);

		static void print(const LogLevel messageType, const std::string& message);
		static void log(const std::string& message);
	public:
		static void init(const std::string& path, const int level);
		static void info(const std::string& message);
		static void warning(const std::string& message, const bool colored = true);
		static void error(const std::string& message, const bool colored = true);
	};
}