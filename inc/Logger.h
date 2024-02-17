#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

namespace Jarvis
{
	enum class LogLevel : uint16_t {
		ERRORMSG,
		WARNINGMSG,
		INFOMSG,
		TRACEMSG,
		DEBUGMSG,

		MINLEVEL = 0,
		MAXLEVEL = 4
	};

	class Logger
	{
		enum class color : uint16_t {
#ifdef _WIN32
			red = 4,
			green = 2,
			yellow = 6,
			blue = 1,
			light_red = 12,
			light_green = 10,
			light_yellow = 14,
			light_blue = 9,
			white = 15,
#else
			red = 0,
			green = 1,
			yellow = 2,
			blue = 3,
			white = 6,
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
		static bool init(const std::string& path, const LogLevel level);

		static void debug(const std::string& message);
		static void trace(const std::string& message);
		static void info(const std::string& message);
		static void warning(const std::string& message, const bool colored = true);
		static void error(const std::string& message);
		static void print(const std::string& message);
	};
}