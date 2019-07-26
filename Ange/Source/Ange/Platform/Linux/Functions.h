#pragma once

#ifdef ANGE_PLATFORM_LINUX

#include <string>

namespace Ange {

	std::string utf8_encode(const std::wstring &wstr);
	std::wstring utf8_decode(const std::string &str);

	int GetScreenDPI();
}

#endif