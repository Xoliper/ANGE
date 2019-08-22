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

project "INSTALL"
	location "INSTALL"
	kind "Utility"
	
	targetdir ("INSTALL")
	objdir ("INSTALL/LOG")
	
	-- Compatibility with Visual Studio
	filter "system:windows"
		systemversion "latest"
	
	-- Copy files to the target directory
	filter "system:windows"
		postbuildcommands {
			'mkdir include',
			'mkdir lib',
			'xcopy /F "../Ange/Build/'.. outputdir ..'/Ange/Ange.lib" "../Install/lib"',
			'xcopy /F "../Vendor/freetype/Build/'.. outputdir ..'/FreeType/FreeType.lib" "../Install/lib"',
			'xcopy /F "../Vendor/glad/Build/'.. outputdir ..'/glad/glad.lib" "../Install/lib"',
			'xcopy /F "../Vendor/GLFW/Build/'.. outputdir ..'/GLFW/GLFW.lib" "../Install/lib"',
			'xcopy /F "../Vendor/libjpeg/Build/'.. outputdir ..'/libjpeg/libjpeg.lib" "../Install/lib"',
			'xcopy /F "../Vendor/libpng/Build/'.. outputdir ..'/libpng/libpng.lib" "../Install/lib"',
			'xcopy /F "../Vendor/zlib/Build/'.. outputdir ..'/zlib/zlib.lib" "../Install/lib"',
			'xcopy /E /Y /F "../Ange/Source" "../Install/include"',
			'xcopy /E /I /F"../Vendor/boost/boost" "../Install/include/boost/boost"',
			'xcopy /E /F "../Vendor/GLM" "../Install/include"',
			'xcopy /E /F "../Vendor/glad/include" "../Install/include"',
			'xcopy /E /F "../Vendor/freetype/include" "../Install/include"',
			'xcopy /E /F"../Vendor/GLFW/include" "../Install/include"',
			'xcopy /I /F "../Vendor/libjpeg/*.h" "../Install/include/libjpeg"',
			'xcopy /I /F "../Vendor/libpng/*.h" "../Install/include/libpng"',
			'xcopy /I /F "../Vendor/zlib/*.h" "../Install/include/zlib"',

		}
	filter "system:linux"
		postbuildcommands {
			"{COPY} %{cfg.targetdir}/Ange.lib ../Install/lib",
			'cp -R "Source" "../Install/include"'
		}

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
