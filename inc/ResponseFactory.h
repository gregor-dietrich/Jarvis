#pragma once

#include <array>
#include <regex>
#include <unordered_map>

#include "aliases.h"

namespace Jarvis
{
	using HttpStringResponse = http::response<http::string_body>;
	using HttpFileResponse = http::response<http::file_body>;

	class ResponseFactory
	{
		static String serverAlias;

		static void sanitize(String& data);

		static http::status createGETResponse(tcp::socket& socket, const HttpRequest& request);

		static HttpStringResponse buildErrorResponse(const http::status statusCode, const u32 version);
		static HttpFileResponse buildFileResponse(const String& target, const unsigned int version);
	public:
		static http::status createResponse(tcp::socket& socket, const HttpRequest& request);

		static String setServerAlias();
	};
}