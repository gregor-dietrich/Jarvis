#pragma once

#include <array>
#include <string>

#include "Types.h"

namespace Jarvis
{
	class ResponseFactory
	{
		static std::array<std::string, 2> searchStrings;
		static std::string serverAlias;

		static std::string sanitize(std::string data);

		static void build404(HttpResponse& response);
	public:
		static HttpResponse createResponse(const HttpRequest& request);

		static std::string setServerAlias();
	};
}