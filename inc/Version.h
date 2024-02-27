#pragma once

#include "aliases.h"

namespace Jarvis
{
	constexpr auto _VERSION = "0.1.0";

	namespace
	{
		constexpr auto _REVISION = "SNAPSHOT";
	}

	const String VERSION()
	{
		static String value;

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