#pragma once

#define MAJOR_VERSION 0
#define MINOR_VERSION 1

#ifdef ANGE_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#elif ANGE_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif

#include "Ange/Core/Logger.h"
#include "Ange/Core/Functions.h"
#include "Ange/Core/Shader.h"
#include "Ange/Core/Window.h"	
#include "Ange/Core/Event.h"
#include "Ange/Core/Primitives.h"
#include "Ange/Core/Texture.h"
#include "Ange/Core/Task.h"
#include "Ange/Core/Font.h"
#include "Ange/Core/BasicWidgets.h"
#include "Ange/Core/ComplexWidgets.h"

/* TODO list:
-Fix double rendering											|FIRST
-Add "only render when something was changed" feature			|LAST
*/