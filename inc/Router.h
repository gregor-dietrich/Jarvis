#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Logger.h"

namespace pt = boost::property_tree;

namespace Jarvis
{
	class Router
	{
		static std::unordered_map<std::string, std::unordered_set<std::string>> m_fileRoutes;

		static pt::ptree parseFile(const std::string& filename);
	public:
		static bool init();
		static bool routeExists(std::string route);
	};
}