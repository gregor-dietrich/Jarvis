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

		static HttpStringResponse build404Response(const HttpRequest& request);
		static HttpFileResponse buildFileResponse(const std::string& target, const unsigned int version);
	public:
		static http::status createResponse(TcpSocket& socket, const HttpRequest& request);

		static std::string setServerAlias();
	};
}