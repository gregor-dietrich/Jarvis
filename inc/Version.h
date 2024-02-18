#pragma once

#include <string>

namespace Jarvis
{
	namespace
	{
		constexpr auto _PROJECT = "Jarvis";
		constexpr auto _VERSION = "0.1.0";
		constexpr auto _REVISION = "Development Build";

#ifdef _DEBUG
		constexpr auto _BUILDCFG = "DEBUG";
#else
		constexpr auto _BUILDCFG = "RELEASE";
#endif
	}

	constexpr const std::string VERSION()
	{
		std::string value = _PROJECT;

		value.append(" v");
		value.append(_VERSION);
		value.append("-");
		value.append(_BUILDCFG);
		value.append(" (");
		value.append(_REVISION);
		value.append(")");

		return value;
	}
}