#pragma once

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0601
#endif

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <cstdint>
#include <string>
#include <thread>

namespace Jarvis
{
	namespace asio = boost::asio;
	namespace beast = boost::beast;
	namespace http = beast::http;
	namespace ip = asio::ip;
	namespace ssl = asio::ssl;

	using f32 = float;
	using f64 = double;
	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	using wchar = wchar_t;

	using HttpRequest = http::request<http::string_body>;
	using ip::tcp;

	using String = std::string;
	using Thread = std::thread;
	using WString = std::wstring;
}