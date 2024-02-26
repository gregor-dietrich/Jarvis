#pragma once

#include <unordered_map>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Jarvis
{
	namespace pt = boost::property_tree;

	class Router
	{
		static std::unordered_map<std::string, std::pair<std::string, std::unordered_map<std::string, std::string>>> m_fileRoutes;

		static pt::ptree parseFile(const std::string& filename);

	public:
		static bool init();
		static bool fileRouteExists(std::string route);
		static std::string getLocalPath(const std::string& target);
		static std::string getMimeType(const std::string& target);
	};
}