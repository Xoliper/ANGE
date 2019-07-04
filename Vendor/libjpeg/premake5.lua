project "libjpeg"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	architecture "x64"

	targetdir ("Build/" .. outputdir .. "/%{prj.name}")
    objdir ("Build-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"cderror.h",
		"cdjpeg.h",
		"jconfig.h",
		"jerror.h",
		"jinclude.h",
		"jmorecfg.h",
		"jpeglib.h",
		"jversion.h",
		"*.c",

	}

	defines
	{
		"NDEBUG",
		"RELEASE",
		"_CRT_SECURE_NO_WARNINGS",		
		"_CONSOLE"
	}

	optimize "Speed"
	omitframepointer "On"
	editandcontinue "Off"
	
	filter "system:windows"
	    systemversion "latest"
		
	filter "system:linux"
		toolset "clang"
		defines {"LINUX"}
		undefines { "USE_SETMODE" }

		disablewarnings
		{
			"implicit-function-declaration",
			"shift-negative-value",
		}


	filter "configurations:Debug"
		defines "ANGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ANGE_RELEASE"
		runtime "Release"
		optimize "on"
