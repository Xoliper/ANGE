workspace "Ange"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Vendor/GLFW"
include "Vendor/freetype"
include "Vendor/libjpeg"
include "Vendor/libpng"
include "Vendor/glad"

project "Ange"
	location "Ange"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	linkgroups "on"

	targetdir ("Ange/Build/" .. outputdir .. "/%{prj.name}")
	objdir ("Ange/Build-obj/" .. outputdir .. "/%{prj.name}")

	pchheader "Apch.h"
	pchsource "Ange/Source/Apch.cpp"

	files 
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/Source",
		"Vendor/GLM",
		"Vendor/GLFW/include",
		"Vendor/libjpeg",
		"Vendor/libpng",
		"Vendor/glad/include",
		"Vendor/boost/",
		"Vendor/freetype/include",
	}

	links 
	{
		"GLFW",
		"glad",
		"libjpeg",
		"libpng",
		"FreeType",
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"ANGE_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_DEPRECATE"
		}
		removefiles { "%{prj.name}/Source/%{prj.name}/Platform/Linux/**" }

	filter "system:linux"
		
		defines
		{
			"ANGE_PLATFORM_LINUX",
			"GLFW_INCLUDE_NONE"		
		}

		removefiles { "%{prj.name}/Source/%{prj.name}/Platform/Win32/**" }

		links 
		{
			"pthread",
			"GLU",
			"GL",
			"rt",
			"Xrandr",
			"Xxf86vm",
			"Xi",
			"Xinerama",
			"X11",
			"dl",
			"Xcursor"
		}

		toolset "clang"

		postbuildmessage
		(
			"Creating packed version of Ange library... "	
		)		

		postbuildcommands
		{
			"./Pack.script"
		}

	filter "configurations:Debug"
		defines "ANGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ANGE_RELEASE"
		runtime "Release"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	linkgroups "on"

	targetdir ("Sandbox/Build/" .. outputdir .. "/%{prj.name}")
	objdir ("Sandbox/Build-obj/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs 
	{
		"Sandbox",
		"Ange/Source",
		"Vendor/GLM",
		"Vendor/GLFW/include",
		"Vendor/libjpeg",
		"Vendor/libpng",
		"Vendor/glad/include",
		"Vendor/boost",
		"Vendor/freetype/include",
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"ANGE_PLATFORM_WINDOWS",
		}

		links
		{
			"opengl32",
			"Ange",
			"GLFW",
			"libjpeg",
			"libpng",
			"FreeType",
			"glad"
		}

	filter "system:linux"
		
		defines
		{
			"ANGE_PLATFORM_LINUX",
		}

		libdirs
		{
			"Ange/Build/" .. outputdir .. "/Ange"
		}

		links 
		{
			"AngePacked",
			"pthread",
			"GLU",
			"GL",
			"rt",
			"Xrandr",
			"Xxf86vm",
			"Xi",
			"Xinerama",
			"X11",
			"dl",
			"Xcursor",
			"libjpeg",
			"FreeType",
		}

		toolset "clang"

	filter "configurations:Debug"
		defines "ANGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ANGE_RELEASE"
		runtime "Release"
		optimize "on"



project "CustomWidgetsExample"
	location "CustomWidget"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	linkgroups "on"

	targetdir ("CustomWidget/Build/" .. outputdir .. "/%{prj.name}")
	objdir ("CustomWidget/Build-obj/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"CustomWidget/**.h",
		"CustomWidget/**.cpp"
	}

	includedirs 
	{
		"CustomWidget",
		"Ange/Source",
		"Vendor/GLM",
		"Vendor/GLFW/include",
		"Vendor/libjpeg",
		"Vendor/libpng",
		"Vendor/glad/include",
		"Vendor/boost",
		"Vendor/freetype/include",
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"ANGE_PLATFORM_WINDOWS",
		}

		links
		{
			"opengl32",
			"Ange",
			"GLFW",
			"libjpeg",
			"libpng",
			"FreeType",
			"glad"
		}

	filter "system:linux"
		
		defines
		{
			"ANGE_PLATFORM_LINUX",
		}

		libdirs
		{
			"Ange/Build/" .. outputdir .. "/Ange"
		}

		links 
		{
			"AngePacked",
			"pthread",
			"GLU",
			"GL",
			"rt",
			"Xrandr",
			"Xxf86vm",
			"Xi",
			"Xinerama",
			"X11",
			"dl",
			"Xcursor",
			"libjpeg",
			"FreeType",
		}

		toolset "clang"

	filter "configurations:Debug"
		defines "ANGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ANGE_RELEASE"
		runtime "Release"
		optimize "on"


project "AngeBuilder"
	location "AngeBuilder"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	linkgroups "on"

	targetdir ("AngeBuilder/Build/" .. outputdir .. "/%{prj.name}")
	objdir ("AngeBuilder/Build-obj/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"AngeBuilder/**.h",
		"AngeBuilder/**.cpp"
	}

	includedirs 
	{
		"AngeBuilder",
		"Ange/Source",
		"Vendor/GLM",
		"Vendor/GLFW/include",
		"Vendor/libjpeg",
		"Vendor/libpng",
		"Vendor/glad/include",
		"Vendor/boost",
		"Vendor/freetype/include",
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"ANGE_PLATFORM_WINDOWS",
		}

		links
		{
			"opengl32",
			"Ange",
			"GLFW",
			"libjpeg",
			"libpng",
			"FreeType",
			"glad"
		}

	filter "system:linux"
		
		defines
		{
			"ANGE_PLATFORM_LINUX",
		}

		libdirs
		{
			"Ange/Build/" .. outputdir .. "/Ange"
		}

		links 
		{
			"AngePacked",
			"pthread",
			"GLU",
			"GL",
			"rt",
			"Xrandr",
			"Xxf86vm",
			"Xi",
			"Xinerama",
			"X11",
			"dl",
			"Xcursor",
			"libjpeg",
			"FreeType",
		}

		toolset "clang"

	filter "configurations:Debug"
		defines "ANGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ANGE_RELEASE"
		runtime "Release"
		optimize "on"
