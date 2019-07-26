#pragma once

#ifdef ANGE_PLATFORM_WINDOWS

#include <string>

namespace Ange {

	std::string utf8_encode(const std::wstring &wstr);
	std::wstring utf8_decode(const std::string &str);

	int GetScreenDPI();

}

#endif