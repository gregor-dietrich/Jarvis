#pragma once

#include <string>

namespace Jarvis
{
	namespace
	{
		constexpr auto _VERSION = "0.1.0";
		constexpr auto _REVISION = "SNAPSHOT";
	}

	const std::string VERSION(const bool full = true)
	{
		if (!full) {
			return _VERSION;
		}

		static std::string value;

		if (value.empty()) {
			value.append("Jarvis v");
			value.append(_VERSION);
			value.append("-");
			value.append(_REVISION);
#ifdef _DEBUG
			value.append(" (DEBUG BUILD)");
#endif
		}

		return value;
	}
}