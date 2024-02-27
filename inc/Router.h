#pragma once

#include <unordered_map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "aliases.h"

namespace Jarvis
{
	namespace pt = boost::property_tree;

	class Router
	{
		static std::unordered_map<String, std::pair<String, std::unordered_map<String, String>>> m_fileRoutes;

		static pt::ptree parseFile(const String& filename);

	public:
		static bool init();
		static bool fileRouteExists(String route);
		static String getLocalPath(const String& target);
		static String getMimeType(const String& target);
	};
}