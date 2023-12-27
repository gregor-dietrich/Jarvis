#pragma once

#include <array>
#include <string>

#include "Types.h"

namespace gcd
{
	class ResponseFactory
	{
		static std::array<std::string, 2> searchStrings;
		static std::array<std::string, 4> fakeServers;
		static std::string fakeServer;

		static std::string sanitize(std::string data);
	public:
		static HttpResponse createResponse(const HttpRequest& request);
	};
}