project "zlib"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("Build/" .. outputdir .. "/%{prj.name}")
    objdir ("Build-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"zlib.h",
		"crc32.h",
		"deflate.h",
		"gzguts.h",
		"inffast.h",
		"inffixed.h",
		"inflate.h",
		"inftrees.h",
		"trees.h",
		"zutil.h",
		"adler32.c",
		"compress.c",
		"crc32.c",
		"deflate.c",
		"gzclose.c",
		"gzlib.c",
		"gzread.c",
		"gzwrite.c",
		"inflate.c",
		"infback.c",
		"inftrees.c",
		"inffast.c",
		"trees.c",
		"uncompr.c",
		"zutil.c",
	}

	defines
	{
		"NDEBUG",
		"RELEASE",
		"_CRT_SECURE_NO_WARNINGS",
		"_CRT_SECURE_NO_DEPRECATE",
		"_CRT_NONSTDC_NO_DEPRECATE",		
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
		
		disablewarnings "implicit-function-declaration"

	filter "configurations:Debug"
		defines "ANGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ANGE_RELEASE"
		runtime "Release"
		optimize "on"
