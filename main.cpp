#include <algorithm>
#include <iostream>
#include <string>

#include "Logger.h"
#include "Server.h"

using namespace gcd;

bool parseArgs(int argc, char** argv)
{
	std::string logDir;
	int logLevel = (int)LogLevel::MINLEVEL;

	try {
		for (int i = 1; i < argc; i += 2) {
			if (i + 1 >= argc) {
				continue;
			}

			const std::string arg = argv[i];

			if (arg == "-logDir") {
				logDir = argv[i + 1];
				continue;
			}

			try { 
				if (arg == "-logLevel") {
					const int parsed = std::stoi(argv[i + 1]);
					logLevel = std::max(logLevel, std::min((int)LogLevel::MAXLEVEL, parsed));
					continue;
				}
			}
			catch (const std::exception& e) {
				Logger::warning("Exception in parseArgs(): " + std::string(e.what()));
				continue;
			}
		}

		Logger::init(logDir, logLevel);
	} catch (...) {
		return false;
	}
	return true;
}

int main(int argc, char** argv)
{
	if (!parseArgs(argc, argv)) {
		Logger::error("Failed to parse command line arguments! Exiting...");
		return EXIT_PROCESS_DEBUG_EVENT;
	}

	try {
		Server server(1337);
		server.run();
		Logger::info("Shutting down...");
	} catch(const std::exception& e) {
		Logger::error("Exception in main(): " + std::string(e.what()));
		return EXIT_PROCESS_DEBUG_EVENT;
	} catch (...) {
		Logger::error("Something went horribly wrong in main()...");
		return EXIT_FAILURE;
	}

	Logger::info("Shut down gracefully.");
	return EXIT_SUCCESS;
}