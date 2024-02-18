#pragma once

#include <memory>

#include "Logger.h"
#include "Types.h"

namespace Jarvis
{
	class DB
	{
	public:
		DB();
		mysql::results query(const std::string& query);
	};
}