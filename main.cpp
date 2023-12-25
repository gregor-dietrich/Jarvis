#include <iostream>

#include "Server.h"

int main()
{
	const gcd::Server server;

	try {
		server.run();
		std::cout << "Shutting down...\n";
	} catch(std::exception e) {
		std::cerr << e.what() << "\n";
		return EXIT_PROCESS_DEBUG_EVENT;
	} catch (...) {
		std::cerr << "Something went horribly wrong...\n";
		return EXIT_FAILURE;
	}

	std::cout << "Shut down gracefully.\n";
	return EXIT_SUCCESS;
}