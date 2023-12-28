#include <algorithm>
#include <iostream>
#include <string>

#include "Logger.h"
#include "Server.h"

using namespace gcd;

struct CLArgs
{
	std::string logDir;
	int logLevel = static_cast<int>(LogLevel::MINLEVEL);
	int port = 80;
};


CLArgs parseArgs(int argc, char** argv)
{
	CLArgs result;

	try {
		for (int i = 1; i < argc; i++) {
			if (i + 1 >= argc) {
				continue;
			}
			const std::string arg = argv[i];

			if (arg == "-logDir") {
				result.logDir = argv[i + 1];
				continue;
			}

			try {
				if (arg == "-logLevel") {
					const int parsed = std::stoi(argv[i + 1]);
					result.logLevel = std::max(result.logLevel, std::min((int)LogLevel::MAXLEVEL, parsed));
					continue;
				}

				if (arg == "-port") {
					result.port = std::stoi(argv[i + 1]);
					continue;
				}
			}
			catch (const std::exception& e) {
				Logger::warning("Exception in parseArgs(): " + std::string(e.what()));
				continue;
			}
		}
	}
	catch (...) {
		Logger::error("Critical Error in parseArgs(), using defaults.");
		return {};
	}
	return result;
}

int main(int argc, char** argv)
{
	const auto args = parseArgs(argc, argv);
	Logger::init(args.logDir, args.logLevel);

	try {
		Logger::trace("Hello World!");
		Logger::info("Hello Earth!");
		Logger::warning("Hello Venus!");
		Logger::error("Hello Mars!\n");

		Server server(args.port);
		server.run();
		Logger::trace("Shutting down...");
	}
	catch (const std::exception& e) {
		Logger::error("Exception in main(): " + std::string(e.what()));
		return EXIT_PROCESS_DEBUG_EVENT;
	}
	catch (...) {
		Logger::error("Something went horribly wrong in main()...");
		return EXIT_FAILURE;
	}

	Logger::info("Shut down gracefully.");
	return EXIT_SUCCESS;
}