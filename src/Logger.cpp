#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>

#include "Logger.h"

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace gcd
{
	LogLevel Logger::logLevel = LogLevel::ERRORMSG;
	std::string Logger::logFilePath = {};
	bool Logger::initialized = false;

	void Logger::createDirectory(const std::string& path)
	{
		if (std::filesystem::exists(path)) {
			return;
		}
		std::filesystem::create_directory(path);
		trace("Log Directory created: " + path);
	}

	std::string Logger::getTime()
	{
		const auto now = std::chrono::system_clock::now();
		const auto now_time = std::chrono::system_clock::to_time_t(now);

		std::tm timeinfo{};
		localtime_s(&timeinfo, &now_time);

		std::stringstream ss;
		ss << std::put_time(&timeinfo, "%d.%m.%Y %H:%M:%S");
		return ss.str();
	}

	std::string Logger::format(const std::string& message)
	{
		std::stringstream result;

		result << "[" << getTime() << "] " << message << "\n";

		return result.str();
	}

	std::string Logger::format(const LogLevel messageType, const std::string& message)
	{
		std::stringstream result;

		result << "[" << getTime() << "] (";
		switch (messageType) {
		case LogLevel::TRACEMSG:
			result << "TRACE";
			break;
		case LogLevel::INFOMSG:
			result << "INFO";
			break;
		case LogLevel::WARNINGMSG:
			result << "WARNING";
			break;
		case LogLevel::ERRORMSG:
			result << "ERROR";
			break;
		}
		result << ") " << message << "\n";

		return result.str();
	}

	void Logger::setColor(const bool isError, const color c/* = color::DEFAULT_COLOR */)
	{
#ifdef _WIN32
		static const auto& H_CONSOLE = GetStdHandle(isError ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
		if (!SetConsoleTextAttribute(H_CONSOLE, static_cast<WORD>(c))) {
			std::stringstream ss;
			ss << "Failed to change text color for " << (isError ? "STDERR" : "STDOUT") << "!";
			warning(ss.str(), false);
		}
#else
		if (isError) {
			std::cerr << "\033[" << c << "m";
		}
		else {	
			std::cout << "\033[" << c << "m";
		}
#endif
	}

	void Logger::print(const LogLevel messageType, const std::string& message)
	{
		switch (messageType) {
		case LogLevel::TRACEMSG:
		case LogLevel::INFOMSG:
			std::cout << message;
			break;
		case LogLevel::WARNINGMSG:
		case LogLevel::ERRORMSG:
			std::cerr << message;
			break;
		}
	}

	void Logger::log(const std::string& message)
	{
		if (logFilePath.empty()) {
			return;
		}

		std::ofstream logfile(logFilePath, std::ios::app);
		if (!logfile.is_open()) {
			error("Unable to create/open the logfile.");
		}
		logfile << message;
	}

	bool Logger::init(const std::string& path, const LogLevel level)
	{
		if (initialized) {
			warning("Prevented Logger re-initialization.");
			false;
		}
		initialized = true;
		logLevel = level;

		if (path.empty()) {
			return true;
		}
		createDirectory(path);

		auto time = getTime();
		std::replace(time.begin(), time.end(), ' ', '_');
		std::replace(time.begin(), time.end(), ':', '.');

		logFilePath = path + "/Log_" + time + ".txt";
		return true;
	}

	void Logger::print(const std::string& message)
	{
		std::cout << format(message);
	}

	void Logger::trace(const std::string& message)
	{
		if (logLevel < LogLevel::TRACEMSG) {
			return;
		}
		const auto formatted = format(LogLevel::TRACEMSG, message);

		setColor(false, color::light_blue);
		print(LogLevel::TRACEMSG, formatted);
		setColor(false);

		log(formatted);
	}

	void Logger::info(const std::string& message)
	{
		if (logLevel < LogLevel::INFOMSG) {
			return;
		}
		const auto formatted = format(LogLevel::INFOMSG, message);

		setColor(false, color::light_green);
		print(LogLevel::INFOMSG, formatted);
		setColor(false);

		log(formatted);
	}

	void Logger::warning(const std::string& message, const bool colored/* = true*/)
	{
		if (logLevel < LogLevel::WARNINGMSG) {
			return;
		}
		const auto formatted = format(LogLevel::WARNINGMSG, message);

		if (colored) {
			setColor(true, color::yellow);
		}
		print(LogLevel::WARNINGMSG, formatted);
		if (colored) {
			setColor(true);
		}

		log(formatted);
	}

	void Logger::error(const std::string& message)
	{
		const auto formatted = format(LogLevel::ERRORMSG, message);

		setColor(true, color::red);
		print(LogLevel::ERRORMSG, formatted);
		setColor(true);

		log(formatted);
	}
}