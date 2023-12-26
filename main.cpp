#include <iostream>
#include <iostream>

#include "Logger.h"
#include "Server.h"

using namespace gcd;

int main(int argc, char** argv)
{
	Server server(1337);

	try {
		server.run();
		Logger::get()->info("Shutting down...");
	} catch(std::exception e) {
		Logger::get()->error("Exception: " + std::string(e.what()));
		return EXIT_PROCESS_DEBUG_EVENT;
	} catch (...) {
		Logger::get()->error("Something went horribly wrong...");
		return EXIT_FAILURE;
	}

	Logger::get()->info("Shut down gracefully.");
	return EXIT_SUCCESS;
}