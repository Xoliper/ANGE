#pragma once

#ifdef ANGE_PLATFORM_WINDOWS

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
		Warning,Error, Fatal,
		Event
	};

	/*!
	Defines colors for message types.
	*/
	enum class ConsoleColor {
		Default = 7, Info = 240, Success = 48,
		Warning = 96, Error = 78, Fatal = 95,
		Event = 10
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
		static void ProcessType(LoggerMsgType lmt, ConsoleColor & cc, size_t & len);	
		
		/*!
		Sets the text color in the console.
		*/
		static void SetConsoleColor(ConsoleColor color, size_t rows);						
		
		/*!
		Creates a time stamp.
		*/
		static void GetTimeStamp(char* buffer, size_t len);								
		
		/*!
		Calculates the number of lines needed to display the message.
		*/
		static size_t CalcRowsAmount(const char* message, size_t len, size_t offset);		
		
		/*!
		Returns the width of the console.
		*/
		static size_t GetConsoleWidth();													
	};

}

#endif
