#include <iostream>
#include <iostream>

#include "Server.h"

using namespace gcd;

int main(int argc, char** argv)
{
	Server server(1337);

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