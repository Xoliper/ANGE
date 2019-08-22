#pragma once

#ifdef ANGE_PLATFORM_WINDOWS

#include <string>

namespace Ange {

	/*!
	Encodes the std::string to std::wstring.
	*/
	std::string utf8_encode(const std::wstring &wstr);
	
	/*!
	Decodes the std::wstring to std::string.
	*/
	std::wstring utf8_decode(const std::string &str);

	/*!
	Returns the DPI of the screen.
	*/
	int GetScreenDPI();

}

#endif