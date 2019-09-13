#pragma once

#define MAJOR_VERSION 0
#define MINOR_VERSION 2

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



#if _WIN32 || _WIN64
#if _WIN64
#define ENV_ARCH 64
#else
#define ENV_ARCH 86
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENV_ARCH 64
#else
#define ENV_ARCH 86
#endif
#endif

#ifndef ENV_ARCH
#define ENV_ARCH 0
#endif

void InfoAboutVersion()
{
	//--------------------------------------------------------------
	//Compiler & Arch info
	//--------------------------------------------------------------
#if defined(__clang__)
	ANGE_INFO("Compiled with: Clang [%i.%i.%i] / Arch: x%i", __clang_major__, __clang_minor__, __clang_patchlevel__, ENV_ARCH);
#elif defined(__GNUC__) || defined(__GNUG__)
	ANGE_INFO("Compiled with: GCC [%i.%i.%i] / Arch: x%i", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__, ENV_ARCH);
#elif defined(_MSC_VER)
	ANGE_INFO("Compiled with: Microsoft Visual C++ [%i] / Arch: x%i", _MSC_VER, ENV_ARCH);
#endif

	//--------------------------------------------------------------
	//Version & Debug
	//--------------------------------------------------------------
#if defined(ANGE_DEBUG) && !defined(ANGE_RELEASE) 
	ANGE_INFO("ANGE version: %i.%i [%s]", MAJOR_VERSION, MINOR_VERSION, "Debug");
#elif !defined(ANGE_DEBUG) && defined(ANGE_RELEASE) 
	ANGE_INFO("ANGE version: %i.%i [%s]", MAJOR_VERSION, MINOR_VERSION, "Release");
#endif
}


/* TODO list:
-Add "only render when something was changed" feature			|LAST
*/