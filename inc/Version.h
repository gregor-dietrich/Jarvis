#pragma once

#include <string>

namespace Jarvis
{
	constexpr auto VERSION = "0.1.0";
	constexpr auto REVISION = "Development Build";

	std::string getVersion()
	{
		static std::string retval = "Jarvis v";

		if (retval.length() <= 8) {
			retval += VERSION;
			retval += " (";
			retval += REVISION;
			retval += ")";
		}

		return retval;
	}
}