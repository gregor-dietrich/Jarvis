#include <iostream>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>

#include "Logger.h"
#include "Router.h"
#include "Util.h"

#include "ResponseFactory.h"

namespace Jarvis
{
	std::array<std::string, 2> ResponseFactory::searchStrings = {"..", "%2e%2e"};
	std::string ResponseFactory::serverAlias;

	std::string ResponseFactory::setServerAlias()
	{
		const std::array<std::string, 10> fakeOSs = {
			" (Alpine)",
			" (Arch Linux)",
			" (CentOS)",
			" (Debian)",
			" (Fedora)",
			" (Gentoo)",
			" (openSUSE)",
			" (RHEL)",
			" (Ubuntu)",
			" (Win64)"
		};

		const std::array<std::string, 15>fakeServers = {
			"Apache/2.4.41",
			"Apache/2.4.46",
			"Apache/2.4.52",
			"Apache/2.4.54",
			"Apache/2.4.58",
			"Apache Tomcat/6.0.53",
			"Apache Tomcat/7.0.109",
			"Apache Tomcat/8.5.97",
			"Apache Tomcat/9.0.84",
			"Apache Tomcat/10.1.17",
			/* "Microsoft-IIS/7.0",
			"Microsoft-IIS/7.5",
			"Microsoft-IIS/8.0",
			"Microsoft-IIS/8.5",
			"Microsoft-IIS/10.0", */
			"Nginx/1.18.0",
			"Nginx/1.20.2",
			"Nginx/1.22.1",
			"Nginx/1.24.0",
			"Nginx/1.25.3"
		};

		serverAlias = fakeServers[randInt(fakeServers.size())] + fakeOSs[randInt(fakeOSs.size())];
		return serverAlias;
	}

	std::string ResponseFactory::sanitize(std::string data)
	{
		for (const auto& searchString : searchStrings) {
			replaceSubString(data, searchString, "");
		}
		return data;
	}

	http::status ResponseFactory::createResponse(TcpSocket& socket, const HttpRequest& request)
	{
		const std::string target = sanitize(request.target().substr(1));

		if (!Router::routeExists(target)) {
			const auto response = build404Response(request);
			http::write(socket, response);
			return response.result();
		}

		auto response = buildFileResponse(target, request.version());
		boost::beast::error_code ec;
		http::serializer<false, http::file_body, http::fields> sr{ response };
		http::write(socket, sr, ec);
		if (ec) {
			Logger::error("createResponse(): " + ec.message());
			response.result(http::status::internal_server_error);
		}
		return response.result();
		
		/*
		switch (request.method()) {
		case http::verb::connect:
			response.result(http::status::method_not_allowed);
			break;
		case http::verb::delete_:
			response.result(http::status::method_not_allowed);
			break;
		case http::verb::get:
			Logger::trace("Received a GET Request for resource: " + target);
			response.result(http::status::ok);
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
		*/
	}

	HttpStringResponse ResponseFactory::build404Response(const HttpRequest& request)
	{
		HttpStringResponse response;
		response.version(request.version());
		response.set(http::field::server, serverAlias);
		response.set(http::field::content_type, "text/html");
		response.result(http::status::not_found);

		std::stringstream html;
		html << "<!DOCTYPE html><html lang=\"en\"><head><title>Jarvis</title></head><body>";
		html << "<div><h1>Error 404</h1><p>Not Found</p></div>";
		html << "</body></html>";
		response.body() = html.str();

		response.prepare_payload();
		return response;
	}

	HttpFileResponse ResponseFactory::buildFileResponse(const std::string& target, const unsigned int version)
	{
		HttpFileResponse response;
		response.version(version);
		response.set(http::field::server, serverAlias);
		response.set(http::field::content_type, Router::getMimeType(target));
		response.result(http::status::ok);

		boost::beast::error_code ec;
		response.body().open(target.c_str(), boost::beast::file_mode::scan, ec);

		if (ec) {
			Logger::error("buildFileResponse(): " + ec.message());
			response.result(http::status::internal_server_error);
		}
		
		response.prepare_payload();
		return response;
	}
}
