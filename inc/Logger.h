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
			light_gray = 7,
			light_red = 12,
			light_green = 10,
			light_yellow = 14,
			light_blue = 9,
#else
			red = 31,
			green = 32,
			yellow = 33,
			blue = 34,
			light_gray = 0,
			light_red = 91,
			light_green = 92,
			light_yellow = 93,
			light_blue = 94,
#endif
			DEFAULT_COLOR = light_gray
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