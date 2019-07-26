#include "Apch.h"

#ifdef ANGE_PLATFORM_LINUX
#include "Ange/Platform/Linux/X11Logger.h"

namespace Ange {

	//-----------------------------------------------------------------------
	//Logger
	//-----------------------------------------------------------------------

	void Logger::ProcessType(LoggerMsgType lmt)
	{
		switch (lmt)
		{
		case LoggerMsgType::Log:
			SetConsoleColor(ConsoleColor::Default);
			printf("[APP] ");
			break;

		case LoggerMsgType::Info:
			SetConsoleColor(ConsoleColor::Info);
			printf("[Info] ");
			break;

		case LoggerMsgType::Success:
			SetConsoleColor(ConsoleColor::Success);
			printf("[Success] ");
			break;

		case LoggerMsgType::Warning:
			SetConsoleColor(ConsoleColor::Warning);
			printf("[Warning] ");
			break;

		case LoggerMsgType::Error:
			SetConsoleColor(ConsoleColor::Error);
			printf("[Error] ");
			break;

		case LoggerMsgType::Fatal:
			SetConsoleColor(ConsoleColor::Fatal);
			printf("[Fatal] ");
			break;
		
		case LoggerMsgType::Event:
			SetConsoleColor(ConsoleColor::Event);
			printf("[Event] ");
			break;
		}
	}

	void Logger::SetConsoleColor(ConsoleColor color)
	{
		printf("%s%d%c", "\033[", static_cast<int>(color), 'm');
	}

	void Logger::UnsetConsoleColor()
	{
		printf("%s%d%c", "\033[", static_cast<int>(ConsoleColor::Default), 'm');
	}

	void Logger::Log(LoggerMsgType lmt, const char* fromFile, int line, const char* fromFunction, const char* message, ...)
	{
		//Vars
		size_t detailsLen = 0;

		//Process Type
		ProcessType(lmt);

		//Print date
		char atTime[11];
		GetTimeStamp(atTime, 11);
		printf("%s", atTime);

		//Print standard info
		va_list arguments;
		va_start(arguments, message);
		vprintf(message, arguments);
		va_end(arguments);


		//Print additional debug info
		if (fromFunction != nullptr && fromFile != nullptr) {
			char format[] = "\n\t\t  Function: %s\n\t\t  File: %s\n\t\t  Line:%u";
			detailsLen = snprintf(nullptr, 0, format, fromFunction, fromFile, line);
			char* buffer = new char[detailsLen+1];
			snprintf(buffer, detailsLen+1, format, fromFunction, fromFile, line);
			printf("%s", buffer);
			delete[] buffer;
		}

		//Color console to default
		UnsetConsoleColor();
		printf("\n");
	}

	void Logger::GetTimeStamp(char* buffer, size_t len)
	{
		struct tm * localtimeBuf;
		std::stringstream ss;
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t inTimeT = std::chrono::system_clock::to_time_t(now);
		localtimeBuf = ::localtime(&inTimeT);
		strftime(buffer, len, "[%H:%M:%S]", localtimeBuf);
	}

}

#endif
