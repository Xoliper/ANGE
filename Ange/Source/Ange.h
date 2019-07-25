#pragma once

#define MAJOR_VERSION 0
#define MINOR_VERSION 1

#ifdef ANGE_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif

#include "Ange/Core/Modules/Logger.h"
#include "Ange/Core/Modules/Shader.h"
#include "Ange/Core/Modules/Window.h"	
#include "Ange/Core/Event.h"
#include "Ange/Core/Primitives.h"

/* TODO list:
-Do not accumulate events like OnResize per frame.				|FIRST
-Change directories structure									|
-Create global "platform" file with all implementation			|
-Fix double rendering											|THIRD
-Add "only render when something was changed" feature			|
*/