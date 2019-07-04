#pragma once

#ifdef ANGE_PLATFORM_LINUX

#include <cstddef>
#include <string.h>

namespace Ange {

	//-----------------------------------------------------------------------
	//Enums
	//-----------------------------------------------------------------------

	/*!
	Specifies the logger message type.
	*/
	enum class LoggerMsgType
	{
		Log, Info, Success,
		Warning, Error, Fatal,
		Event
	};

	/*!
	Defines colors for message types.
	*/
	enum class ConsoleColor {
		Default = 49, Info = 100, Success = 44,
		Warning = 43, Error = 101, Fatal = 45,
		Event = 42
	};
	
	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------

	/*!
	Allows displaying detailed messages on the console. It supports text coloring, time stamps and gives
	information about where the error occurred. This is a purely static class and can be used without
	creating an object.
	*/
	class Logger
	{
	public:

		/*!
		The default and the only function that displays the message by the logger.
		*/
		static void Log(LoggerMsgType lmt, const char* fromFile, int line, const char* fromFunction, const char* message, ...);
	
	private:	
		
		/*!
		Selects the message color and adds information about the message type.
		*/
		static void ProcessType(LoggerMsgType lmt);	
		
		/*!
		Sets the text color in the console.
		*/
		static void SetConsoleColor	(ConsoleColor color);						
		
		/*!
		Sets the text color in the console to the default one.
		*/
		static void UnsetConsoleColor();													
		
		/*!
		Creates a time stamp.
		*/
		static void GetTimeStamp	(char* buffer, size_t len);																	
	};

}

#endif
