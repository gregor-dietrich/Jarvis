#pragma once

#include <random>

#include "aliases.h"

namespace Jarvis
{
	inline size_t randInt(const size_t min, const size_t max)
	{
		if (min > max - 1) {
			return min;
		}
		thread_local auto rng = std::mt19937(std::random_device{}());
		std::uniform_int_distribution<size_t> distribution(min, max - 1);
		return distribution(rng);
	}

	inline size_t randInt(const size_t max)
	{
		return randInt(0, max);
	}

	inline String htmlEntities(const String& data)
	{
		String buffer;
		buffer.reserve(data.size());

		for (const char& pos : data) {
			switch (pos) {
			case '&':  buffer.append("&amp;");		break;
			case '\"': buffer.append("&quot;");     break;
			case '\'': buffer.append("&apos;");     break;
			case '<':  buffer.append("&lt;");       break;
			case '>':  buffer.append("&gt;");       break;

			default:   buffer.append(&pos, 1);		break;
			}
		}

		return buffer;
	}

	inline void replaceSubString(String& sourceString, const String& oldString, const String& newString)
	{
		size_t pos = 0;
		while ((pos = sourceString.find(oldString, pos)) != String::npos) {
			sourceString.replace(pos, oldString.length(), newString);
			pos += newString.length();
		}
	}

	inline String toString(const tcp::socket& socket)
	{
		const tcp::endpoint& endpoint = socket.remote_endpoint();
		return endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
	}
}