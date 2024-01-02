#include <filesystem>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "Router.h"

namespace Jarvis
{
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> Router::m_fileRoutes;

	pt::ptree Router::parseFile(const std::string& filename)
	{
		pt::ptree root;
		pt::read_json(filename, root);
		return root;
	}

	bool Router::init()
	{
		try {
			for (const auto& routePath : parseFile("cfg/fileRoutes.json")) {
				for (const auto& fileExtension : routePath.second) {
					for (const auto& kv : fileExtension.second) {
						m_fileRoutes[routePath.first][kv.first] = kv.second.data();
					}
				}
			}
		}
		catch (const std::exception& e) {
			Logger::error("Router::init(): " + std::string(e.what()));
			return false;
		}

		for (const auto& fileRoute : m_fileRoutes) {
			Logger::print(fileRoute.first + ": ");
			for (const auto& item : fileRoute.second) {
				Logger::print(item.first + ", " + item.second);
			}
		}

		return true;
	}

	bool Router::fileRouteExists(std::string route)
	{
		try {
			size_t pos = route.find("?");
			if (pos != std::string::npos) {
				route = route.substr(0, pos);
			}
			std::vector<std::string> path;
			boost::split(path, route, boost::is_any_of("/"));
			if (path.empty()) {
				return false;
			}

			for (const auto& fileRoute : m_fileRoutes) {
				if (fileRoute.first != path[0]) {
					continue;
				}
				for (const auto& fileExtension : fileRoute.second) {
					const auto fileName = path[path.size() - 1];
					const auto offset = fileName.find(".");
					if (offset == std::string::npos) {
						continue;
					}
					if (fileName.substr(offset + 1) == fileExtension.first) {
						return std::filesystem::exists(route);
					}
				}
			}
		} catch (...) {
			
		}
		return false;
	}

	std::string Router::getMimeType(const std::string& target)
	{
		std::vector<std::string> result;
		boost::split(result, target, boost::is_any_of("/"));
		const auto& fileName = result[result.size() - 1];
		const auto& fileExtension = fileName.substr(fileName.find('.') + 1);
		return m_fileRoutes[result[0]][fileExtension];
	}
}