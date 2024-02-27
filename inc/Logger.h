#pragma once

#include <filesystem>
#include <fstream>
#include <memory>

#include "aliases.h"

namespace Jarvis
{
	enum class LogLevel : u16 {
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
		enum class color : u16 {
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
		static String logFilePath;
		static bool initialized;

		Logger() = default;

		static void createDirectory(const String& path);
		static String getTime();
		static String format(const String& message);
		static String format(const LogLevel messageType, const String& message);
		static void setColor(const bool isError, const color c = color::DEFAULT_COLOR);

		static void print(const LogLevel messageType, const String& message);
		static void log(const String& message);
	public:
		static bool init(const String& path, const LogLevel level);

		static void debug(const String& message);
		static void trace(const String& message);
		static void info(const String& message);
		static void warning(const String& message, const bool colored = true);
		static void error(const String& message);
		static void print(const String& message);
	};
}