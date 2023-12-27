#pragma once

#include <random>
#include <string>

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

inline std::string htmlEntities(const std::string& data)
{
	std::string buffer;
	buffer.reserve(data.size());

	for (const char& pos : data) {
		if (pos == '\0') continue;

		switch (pos) {
		case '&':  buffer.append("&amp;");       break;
		case '\"': buffer.append("&quot;");      break;
		case '\'': buffer.append("&apos;");      break;
		case '<':  buffer.append("&lt;");        break;
		case '>':  buffer.append("&gt;");        break;

		default:   buffer.append(&pos, 1); break;
		}
	}

	return buffer;
}