#include "Apch.h"

#ifdef ANGE_PLATFORM_WINDOWS
#include "Ange/Platform/Win32/W32Logger.h"

namespace Ange {

	//-----------------------------------------------------------------------
	//Logger
	//-----------------------------------------------------------------------

	void Logger::ProcessType(LoggerMsgType lmt, ConsoleColor & cc, size_t & size)
	{
		switch (lmt)
		{
		case LoggerMsgType::Log:
			printf("[APP] ");
			cc = ConsoleColor::Default;
			size = 6;
			break;

		case LoggerMsgType::Info:
			printf("[Info] ");
			cc = ConsoleColor::Info;
			size = 7;
			break;

		case LoggerMsgType::Success:
			printf("[Success] ");
			cc = ConsoleColor::Success;
			size = 10;
			break;

		case LoggerMsgType::Warning:
			printf("[Warning] ");
			cc = ConsoleColor::Warning;
			size = 10;
			break;

		case LoggerMsgType::Error:
			printf("[Error] ");
			cc = ConsoleColor::Error;
			size = 8;
			break;

		case LoggerMsgType::Fatal:
			printf("[Fatal] ");
			cc = ConsoleColor::Fatal;
			size = 8;
			break;
		
		case LoggerMsgType::Event:
			printf("[Event] ");
			cc = ConsoleColor::Event;
			size = 8;
			break;
		}
	}

	void Logger::SetConsoleColor(ConsoleColor color, size_t rows)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD written = 0;
		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		if (GetConsoleScreenBufferInfo(hConsole, &cbsi)) {
			int width = cbsi.srWindow.Right - cbsi.srWindow.Left + 1;
			cbsi.dwCursorPosition.Y -= static_cast<short>(rows);
			cbsi.dwCursorPosition.X = 0;
			FillConsoleOutputAttribute(hConsole, static_cast<int>(color), static_cast<DWORD>(width*rows), cbsi.dwCursorPosition, &written);
		}
	}

	size_t Logger::GetConsoleWidth()
	{

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		if (GetConsoleScreenBufferInfo(hConsole, &cbsi)){
			return (cbsi.srWindow.Right - cbsi.srWindow.Left);
		}
		//ANGE_WARNING("Cannot get console witdh, this can cause malformated output.");
		return 0;
	}


	size_t Logger::CalcRowsAmount(const char * message, size_t len, size_t offset)
	{
		size_t noEnterSum = offset;
		size_t rows = 1;
		size_t width = GetConsoleWidth();
		for (size_t i = 0; i<len-1; i++){
			if (noEnterSum > width) {
				noEnterSum = 0;
				++rows;
			}
			
			if (message[i] == '\n') {
				noEnterSum = 0;
				++rows;
			} else {
				if (message[i] != '\t') {
					++noEnterSum;
				} else {
					noEnterSum += 8;
				}
			}
		}
		return rows;
	}


	void Logger::Log(LoggerMsgType lmt, const char* fromFile, int line, const char* fromFunction, const char* message, ...)
	{
		//Vars
		size_t msgLen = 0;
		size_t detailsLen = 0;
		size_t enters = 0;
		size_t headerSize = 0;
		size_t rows = 0;

		//Print date
		char atTime[11];
		GetTimeStamp(atTime, 11);
		printf("%s", atTime);

		//Process Type
		ConsoleColor cc;
		ProcessType(lmt, cc, headerSize);

		//Print standard info
		va_list arguments;
		va_start(arguments, message);
		vprintf(message, arguments);
		msgLen = vsnprintf(nullptr, 0, message, arguments);
		va_end(arguments);
		rows = CalcRowsAmount(message, msgLen+1, 11+headerSize);


		//Print additional debug info
		if (fromFunction != nullptr && fromFile != nullptr) {
			char format[] = "\n\t\t  Function: %s\n\t\t  File: %s\n\t\t  Line:%u\n";
			detailsLen = snprintf(nullptr, 0, format, fromFunction, fromFile, line);
			char* buffer = new char[detailsLen+1];
			snprintf(buffer, detailsLen+1, format, fromFunction, fromFile, line);
			printf("%s", buffer);
			rows += CalcRowsAmount(buffer, detailsLen+1, 0)-2;
			delete[] buffer;
		} else {
			printf("\n");
		}

		//Color console
		SetConsoleColor(cc, rows);
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
