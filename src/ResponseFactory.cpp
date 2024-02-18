#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include "Logger.h"
#include "Router.h"
#include "Util.h"

#include "ResponseFactory.h"

namespace Jarvis
{
	std::string ResponseFactory::serverAlias;

	std::string ResponseFactory::setServerAlias()
	{
		static const std::array<std::string, 10> fakeOSs = {
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

		static const std::array<std::string, 15> fakeServers = {
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

	http::status ResponseFactory::createResponse(tcp::socket& socket, const HttpRequest& request)
	{
		HttpStringResponse response;

		switch (request.method()) {
		case http::verb::get:
			return createGETResponse(socket, request);
		case http::verb::connect:
		case http::verb::delete_:
		case http::verb::head:
		case http::verb::options:
		case http::verb::patch:
		case http::verb::post:
		case http::verb::put:
		case http::verb::trace:
		default:
			response = buildErrorResponse(http::status::method_not_allowed, request.version());
			break;
		}
		
		return response.result();
	}

	void ResponseFactory::sanitize(std::string& data)
	{
		static const std::array<std::string, 2> searchStrings = { "..", "%2e%2e" };
		for (const auto& searchString : searchStrings) {
			replaceSubString(data, searchString, "");
		}

		replaceSubString(data, "%20", " ");

		static const std::regex pattern("[^a-zA-Z0-9-_. ?=/&]");
		data = std::regex_replace(data, pattern, "");
	}

	http::status ResponseFactory::createGETResponse(tcp::socket& socket, const HttpRequest& request)
	{
		std::string target = std::string(request.target().substr(1));
		sanitize(target);
		Logger::trace("Received a GET Request for resource: " + target);

		if (Router::fileRouteExists(target)) {
			try {
				auto fileResponse = buildFileResponse(target, request.version());
				beast::error_code ec;
				http::serializer<false, http::file_body, http::fields> sr{ fileResponse };
				http::write(socket, sr, ec);
				if (ec) {
					throw std::runtime_error(ec.message().c_str());
				}
				return fileResponse.result();
			} catch (const std::exception& e) {
				Logger::error("createGETResponse(): " + std::string(e.what()));
				const auto response = buildErrorResponse(http::status::internal_server_error, request.version());
				http::write(socket, response);
				return response.result();
			}
		}

		const auto response = buildErrorResponse(http::status::not_found, request.version());
		http::write(socket, response);
		return response.result();
	}

	HttpStringResponse ResponseFactory::buildErrorResponse(const http::status statusCode, const unsigned int version)
	{
		HttpStringResponse response;
		response.version(version);
		response.set(http::field::server, serverAlias);
		response.set(http::field::content_type, "text/html");
		response.result(statusCode);

		std::stringstream html;
		html << "<!DOCTYPE html><html lang=\"en\"><head><title>Jarvis</title></head><body>";
		html << "<div><h1>Error " << static_cast<int>(statusCode) << "</h1><p>" << statusCode << "</p></div>";
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

		beast::error_code ec;
		response.body().open(Router::getLocalPath(target).c_str(), beast::file_mode::scan, ec);

		if (ec) {
			Logger::error("buildFileResponse(): " + ec.message());
			response.result(http::status::internal_server_error);
		}
		
		response.prepare_payload();
		return response;
	}
}
