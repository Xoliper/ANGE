#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <cstdarg>
#include <cstdlib>
#include <functional>
#include <vector>
#include <map>
#include <assert.h>
#include <sys/stat.h>

#ifdef ANGE_PLATFORM_WINDOWS
	#include <windows.h>
	#include <ConsoleApi.h>
#elif ANGLE_PLATFORM_LINUX
	#include <cstddef>
#endif
