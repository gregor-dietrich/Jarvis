#include <algorithm>
#include <iostream>
#include <string>
#include <thread>

#include "aliases.h"
#include "DB.h"
#include "Logger.h"
#include "Server.h"
#include "util.h"
#include "version.h"

using namespace Jarvis;

struct CLArgs
{
	String logDir;
	port_t port = 80;
	LogLevel logLevel = LogLevel::MINLEVEL;
};

static CLArgs parseArgs(int argc, char** argv)
{
	CLArgs result;

	for (int i = 1; i < argc; i++) {
		if (i + 1 >= argc) {
			continue;
		}
		const String arg = argv[i];

		if (arg == "-logDir") {
			result.logDir = argv[++i];
			continue;
		}
		
		try {
			if (arg == "-logLevel") {
				result.logLevel = static_cast<LogLevel>(std::max(static_cast<int>(LogLevel::MINLEVEL), 
																std::min(static_cast<int>(LogLevel::MAXLEVEL), std::stoi(argv[++i]))));
				continue;
			}

			if (arg == "-port") {
				result.port = static_cast<port_t>(std::stoi(argv[++i]));
				continue;
			}
		}
		catch (const std::exception& e) {
			Logger::warning("Exception in parseArgs(): " + String(e.what()));
			continue;
		}
	}

	return result;
}

int main(int argc, char** argv)
{
	try {
		Logger::print(VERSION());

		const auto args = parseArgs(argc, argv);
		Logger::init(args.logDir, args.logLevel);

		DB db("root", "", "company", "localhost");

		const auto tables = db.query("SELECT table_name FROM information_schema.tables WHERE table_schema = ?", { mysql::field("company") });

		for (const auto& row : tables.rows()) {
			Logger::info(row[0].as_string());
			const auto result = db.query("SELECT COLUMN_NAME, DATA_TYPE, IS_NULLABLE, COLUMN_DEFAULT FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = ?", { mysql::field(row[0]) });
			Logger::info(DB::to_string(result));
		}
		
		Server server(args.port);
		std::thread serverThread(&Server::run, &server);

		while (true) {
			String input;
			std::getline(std::cin, input);
			if (input == "quit") {
				server.quit();
				serverThread.join();
				break;
			}
			Logger::error("Command not recognized: " + input + ". Please try again.");
		}
	}
	catch (const std::exception& e) {
		Logger::error("Exception in main(): " + String(e.what()));
		return 1;
	}
	catch (...) {
		Logger::error("Something went horribly wrong in main()...");
		return 2;
	}

	Logger::info("Shut down gracefully.");
	return 0;
}