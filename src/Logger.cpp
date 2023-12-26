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
	std::string Logger::getTime()
	{
		const auto now = std::chrono::system_clock::now();
		const auto now_time = std::chrono::system_clock::to_time_t(now);

		char buffer[26];
		ctime_s(buffer, sizeof buffer, &now_time);
		std::string full = buffer;
		
		std::stringstream result;
		result << full.substr(4, 3) << " " << full.substr(8, 2) << " " << full.substr(20, 4) << ", " << full.substr(11, 8);
		return result.str();
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
			get()->warning("Failed to change text color for STDERR!", false);
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
		std::ofstream logfile("log.txt", std::ios::app);
		if (!logfile.is_open()) {
			get()->error("Unable to create/open the logfile.");
		}
		logfile << message;
	}

	std::shared_ptr<Logger> Logger::get()
	{
		static const auto instance = std::shared_ptr<Logger>(new Logger);
		return instance;
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