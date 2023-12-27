#include <chrono>
#include <ctime>
#include <iostream>

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

	void Logger::createDirectory(const std::string& path)
	{
		if (std::filesystem::exists(path)) {
			return;
		}
		std::filesystem::create_directory(path);
		Logger::info("Log Directory created: " + path);
	}

	std::string Logger::getTime()
	{
		const auto now = std::chrono::system_clock::now();
		const auto now_time = std::chrono::system_clock::to_time_t(now);

		std::tm timeinfo;
		localtime_s(&timeinfo, &now_time);

		std::stringstream ss;
		ss << std::put_time(&timeinfo, "%d.%m.%Y %H:%M:%S");
		return ss.str();
	}

	std::string Logger::format(const LogLevel messageType, const std::string& message)
	{
		std::stringstream result;

		result << "[" << getTime() << "] (";
		switch (messageType) {
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
			warning("Failed to change text color for STDERR!", false);
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
		const auto formatted = format(messageType, message);
		switch (messageType) {
		case LogLevel::INFOMSG:
			std::cout << formatted;
			break;
		case LogLevel::WARNINGMSG:
		case LogLevel::ERRORMSG:
			std::cerr << formatted;
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

	void Logger::init(const std::string& path, const int level)
	{
		if (!logFilePath.empty()) {
			return warning("Prevented Logger re-initialization.");
		}

		logLevel = static_cast<LogLevel>(level);

		if (path.empty()) {
			return;
		}

		auto time = getTime();
		std::replace(time.begin(), time.end(), ' ', '_');
		std::replace(time.begin(), time.end(), ':', '.');

		createDirectory(path);
		logFilePath = path + "/Log_" + time + ".txt";
	}

	void Logger::info(const std::string& message)
	{
		if (logLevel < LogLevel::INFOMSG) {
			return;
		}

		setColor(false, color::green);
		print(LogLevel::INFOMSG, message);
		setColor(false);

		log(format(LogLevel::INFOMSG, message));
	}

	void Logger::warning(const std::string& message, const bool colored/* = true*/)
	{
		if (logLevel < LogLevel::WARNINGMSG) {
			return;
		}

		setColor(true, color::yellow);
		print(LogLevel::WARNINGMSG, message);
		setColor(true);

		log(format(LogLevel::WARNINGMSG, message));
	}

	void Logger::error(const std::string& message, const bool colored/* = true*/)
	{
		setColor(true, color::red);
		print(LogLevel::ERRORMSG, message);
		setColor(true);

		log(format(LogLevel::ERRORMSG, message));
	}
}