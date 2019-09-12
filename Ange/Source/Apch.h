#pragma once

#ifdef ANGE_PLATFORM_WINDOWS
#include <windows.h>
//#include <ConsoleApi.h>
#elif ANGLE_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <cstddef>
#endif

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
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
