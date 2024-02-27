#include <filesystem>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "Logger.h"
#include "util.h"
#include "Router.h"

namespace Jarvis
{
	std::unordered_map<String, std::pair<String, std::unordered_map<String, String>>> Router::m_fileRoutes;

	pt::ptree Router::parseFile(const String& filename)
	{
		pt::ptree root;
		pt::read_json(filename, root);
		return root;
	}

	bool Router::init()
	{
		try {
			for (const auto& routePath : parseFile("cfg/fileRoutes.json")) {
				const auto& localPath = routePath.second.get<String>("localPath");
				for (const auto& fileExtension : routePath.second.get_child("fileExtensions")) {
					for (const auto& kv : fileExtension.second)	{
						if (m_fileRoutes.find(routePath.first) == m_fileRoutes.end()) {
							m_fileRoutes[routePath.first] = std::make_pair(localPath, std::unordered_map<String, String>());
						}
						m_fileRoutes[routePath.first].second[kv.first] = kv.second.data();
					}
				}
			}
		}
		catch (const std::exception& e) {
			Logger::error("Router::init(): " + String(e.what()));
			return false;
		}

		return true;
	}

	bool Router::fileRouteExists(String route)
	{
		try {
			size_t pos = route.find("?");
			if (pos != String::npos) {
				route = route.substr(0, pos);
			}
			std::vector<String> path;
			boost::split(path, route, boost::is_any_of("/"));
			
			if (path.empty() || m_fileRoutes.count(path[0]) == 0) {
				return false;
			}

			const auto& fileRoute = m_fileRoutes[path[0]];
			for (const auto& fileExtension : fileRoute.second) {
				const auto& fileName = path[path.size() - 1];
				
				auto offset = String::npos;
				for (int64_t i = fileName.length() - 1; i >= 0; --i) {
					if (fileName[i] != '.') {
						continue;
					}
					offset = i;
					break;
				}

				if (offset == String::npos) {
					continue;
				}
				
				if (fileName.substr(offset + 1) == fileExtension.first) {
					std::filesystem::path filePath = fileRoute.first + "/" + fileName;
					return std::filesystem::exists(filePath);
				}
			}
		} catch (...) {

		}
		return false;
	}

	String Router::getMimeType(const String& target)
	{
		std::vector<String> result;
		boost::split(result, target, boost::is_any_of("/"));
		const auto& fileName = result[result.size() - 1];
		const auto fileExtension = fileName.substr(fileName.find('.') + 1);
		return m_fileRoutes[result[0]].second[fileExtension];
	}

	String Router::getLocalPath(const String& target)
	{
		std::vector<String> result;
		boost::split(result, target, boost::is_any_of("/"));

		String filePath = target;
		replaceSubString(filePath, result[0] + "/", m_fileRoutes[result[0]].first + "/");
		return filePath;
	}
}