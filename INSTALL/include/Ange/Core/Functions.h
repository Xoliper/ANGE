#pragma once

#ifdef ANGE_PLATFORM_WINDOWS
#include "Ange/Platform/Win32/Functions.h"
#elif ANGE_PLATFORM_LINUX
#include "Ange/Platform/Linux/Functions.h"
#endif