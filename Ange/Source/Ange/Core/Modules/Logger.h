#pragma once

#ifdef ANGE_PLATFORM_WINDOWS
	#include "Ange/Core/Platform/Win32/W32Logger.h"
#elif ANGE_PLATFORM_LINUX
	#include "Ange/Core/Platform/Linux/X11Logger.h"

#endif

namespace Ange {
	#undef Success
	//-----------------------------------------------------------------------
	//Defines
	//-----------------------------------------------------------------------
	#define LOG(TYPE, MESSAGE, ...)				::Ange::Logger::Log(TYPE, nullptr, 0, nullptr, MESSAGE, ##__VA_ARGS__);
	#define LOG_DETAILED(TYPE, MESSAGE, ...)	::Ange::Logger::Log(TYPE, __FILE__, __LINE__, __FUNCTION__, MESSAGE, ##__VA_ARGS__);

	#define APP_LOG(MESSAGE, ...)				LOG(::Ange::LoggerMsgType::Log, MESSAGE, ##__VA_ARGS__)
	#define ANGE_INFO(MESSAGE, ...)				LOG(::Ange::LoggerMsgType::Info, MESSAGE, ##__VA_ARGS__)
	#define ANGE_EVENT(MESSAGE, ...)//			LOG(::Ange::LoggerMsgType::Event, MESSAGE, ##__VA_ARGS__)
	#define ANGE_SUCCESS(MESSAGE, ...)			LOG(::Ange::LoggerMsgType::Success, MESSAGE, ##__VA_ARGS__)
	#define ANGE_WARNING(MESSAGE, ...)			LOG(::Ange::LoggerMsgType::Warning, MESSAGE, ##__VA_ARGS__)
	#define ANGE_ERROR(MESSAGE, ...)			LOG_DETAILED(::Ange::LoggerMsgType::Error, MESSAGE, ##__VA_ARGS__)
	#define ANGE_FATAL(MESSAGE, ...)			LOG_DETAILED(::Ange::LoggerMsgType::Fatal, MESSAGE, ##__VA_ARGS__)
	#define ANGE_ASSERT(CHECK, MESSAGE, ...)	if(!CHECK) {ANGE_FATAL(MESSAGE, ##__VA_ARGS__)}

	#ifdef ANGE_DEBUG
		#define ANGE_NULLPTR_TEST(x, str); if(x == nullptr){ANGE_FATAL(str);}
		#define ANGE_ZERO_TEST(x, str); if(x == 0){ANGE_FATAL(str);}
		#define ANGE_M1_TEST(x, str); if(x == -1){ANGE_FATAL(str);}
	#elif ANGE_RELEASE
		#define ANGE_NULLPTR_TEST(x, str); 
		#define ANGE_ZERO_TEST(x, str); 
		#define ANGE_M1_TEST(x, str);
	#endif
}

