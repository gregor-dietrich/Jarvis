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
		TRACEMSG,

		MINLEVEL = 0,
		MAXLEVEL = 3
	};

	class Logger
	{
		enum class color {
#ifdef _WIN32
			red = 4,
			green = 2,
			yellow = 6,
			blue = 1,
			light_red = 12,
			light_green = 10,
			light_yellow = 14,
			light_blue = 9,
#else
			red,
			green,
			yellow,
			blue,
			light_red = red,
			light_green = green,
			light_yellow = yellow,
			light_blue = blue,
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
		static bool initialized;

		Logger() = default;

		static void createDirectory(const std::string& path);
		static std::string getTime();
		static std::string format(const std::string& message);
		static std::string format(const LogLevel messageType, const std::string& message);
		static void setColor(const bool isError, const color c = color::DEFAULT_COLOR);

		static void print(const LogLevel messageType, const std::string& message);
		static void log(const std::string& message);
	public:
		static bool init(const std::string& path, const int level);

		static void trace(const std::string& message);
		static void info(const std::string& message);
		static void warning(const std::string& message, const bool colored = true);
		static void error(const std::string& message);
		static void print(const std::string& message);
	};
}