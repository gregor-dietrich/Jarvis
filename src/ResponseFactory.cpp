#include <iostream>
#include <sstream>

#include "Logger.h"
#include "ResponseFactory.h"

namespace gcd
{
	HttpResponse ResponseFactory::createResponse(const HttpRequest& request)
	{
		HttpResponse response;
		response.version(request.version());
		response.set(http::field::server, "Jarvis");
		response.result(http::status::method_not_allowed);

		const http::verb method = request.method();

		std::stringstream html;
		switch (request.method()) {
		case http::verb::connect:
			break;
		case http::verb::delete_:
			break;
		case http::verb::get:
			Logger::info("Received a GET Request...");

			response.set(http::field::content_type, "text/html");
			response.result(http::status::ok);

			html << "<!DOCTYPE html><html lang=\"en\"><head><title>Jarvis</title></head><body>";
			html << "<div><p>Hello Mars!</p></div>";
			html << "</body></html>";
			response.body() = html.str();
			break;
		case http::verb::head:
			break;
		case http::verb::options:
			break;
		case http::verb::patch:
			break;
		case http::verb::post:
			break;
		case http::verb::put:
			break;
		case http::verb::trace:
			break;
		}
		
		response.prepare_payload();
		return response;
	}
}
