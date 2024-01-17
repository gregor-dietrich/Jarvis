#pragma once

#include <array>
#include <regex>
#include <string>
#include <unordered_map>

#include "Types.h"

namespace Jarvis
{
	class ResponseFactory
	{
		static std::array<std::string, 2> searchStrings;
		static std::string serverAlias;
		static std::regex pattern;

		static void sanitize(std::string& data);

		static http::status createGETResponse(TcpSocket& socket, const HttpRequest& request);

		static HttpStringResponse buildErrorResponse(const http::status statusCode, const unsigned int version);
		static HttpFileResponse buildFileResponse(const std::string& target, const unsigned int version);
	public:
		static http::status createResponse(TcpSocket& socket, const HttpRequest& request);

		static std::string setServerAlias();
	};
}