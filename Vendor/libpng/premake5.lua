include "../zlib"

project "libpng"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("Build/" .. outputdir .. "/%{prj.name}")
    objdir ("Build-obj/" .. outputdir .. "/%{prj.name}")


	includedirs
	{
		"../zlib",
	}

	links
	{
		"zlib"
	}

	files
	{
		"png.h",
		"pngconf.h",
		"pngpriv.h",
		"pngdebug.h",
		"pnginfo.h",
		"pngstruct.h",
		"png.c",
		"pngerror.c",
		"pngget.c",
		"pngmem.c",
		"pngpread.c",
		"pngread.c",
		"pngrio.c",
		"pngrtran.c",
		"pngrutil.c",
		"pngset.c",
		"pngtrans.c",
		"pngwio.c",
		"pngwrite.c",
		"pngwtran.c",
		"pngwutil.c",
	}

	defines
	{
		"NDEBUG",
		"RELEASE",
		"_CRT_SECURE_NO_WARNINGS",	
		"_CRT_SECURE_NO_DEPRECATE",
		"MD",	
	}

	optimize "Speed"
	omitframepointer "On"
	editandcontinue "Off"

	
	filter "system:windows"
	    systemversion "latest"
		
		defines 
		{
			"WIN32"	
		}
	
	filter "system:linux"
		
		defines 
		{
			"X11"
		}

	filter "configurations:Debug"
		defines "ANGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ANGE_RELEASE"
		runtime "Release"
		optimize "on"
