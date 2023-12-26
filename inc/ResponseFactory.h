#pragma once

#include "Types.h"

namespace gcd
{
	class ResponseFactory
	{
	public:
		static HttpResponse createResponse(const HttpRequest& request);
	};
}