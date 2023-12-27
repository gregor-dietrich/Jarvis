#include <iostream>
#include <sstream>
#include <string>

#include "Logger.h"
#include "ResponseFactory.h"
#include "Util.h"

namespace gcd
{
	std::array<std::string, 2> ResponseFactory::searchStrings = {"..", "%2e%2e"};
	std::array<std::string, 4> ResponseFactory::fakeServers = {
		"Apache/2.4.56",
		"Apache Tomcat/9.0.58",
		"Microsoft-IIS/10.0",
		"Nginx/1.14.2"
	};
	std::string ResponseFactory::fakeServer;

	std::string ResponseFactory::sanitize(std::string data)
	{
		for (const auto& searchString : searchStrings) {
			size_t pos = 0;
			while ((pos = data.find(searchString, pos)) != std::string::npos) {
				data.replace(pos, searchString.length(), "");
				pos += searchString.length();
			}
		}

		return htmlEntities(data);
	}

	HttpResponse ResponseFactory::createResponse(const HttpRequest& request)
	{
		HttpResponse response;
		response.version(request.version());

		if (fakeServer.empty()) {
			fakeServer = fakeServers[randInt(fakeServers.size())];
			Logger::info("Using server alias: " + fakeServer);
		}
		response.set(http::field::server, fakeServer);

		const std::string target = request.target();
		std::stringstream html;

		switch (request.method()) {
		case http::verb::connect:
			response.result(http::status::method_not_allowed);
			break;
		case http::verb::delete_:
			response.result(http::status::method_not_allowed);
			break;
		case http::verb::get:
			Logger::trace("Received a GET Request for resource: " + target);

			html << "<!DOCTYPE html><html lang=\"en\"><head><title>Jarvis</title></head><body>";
			html << "<div><p>Hello Mars!</p></div>";
			html << "</body></html>";

			response.set(http::field::content_type, "text/html");
			response.result(http::status::ok);
			response.body() = html.str();
			break;
		case http::verb::head:
			response.result(http::status::method_not_allowed);
			break;
		case http::verb::options:
			response.result(http::status::method_not_allowed);
			break;
		case http::verb::patch:
			response.result(http::status::method_not_allowed);
			break;
		case http::verb::post:
			response.result(http::status::method_not_allowed);
			break;
		case http::verb::put:
			response.result(http::status::method_not_allowed);
			break;
		case http::verb::trace:
			response.result(http::status::method_not_allowed);
			break;
		}
		
		response.prepare_payload();
		return response;
	}
}
