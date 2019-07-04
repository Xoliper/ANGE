project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("Build/" .. outputdir .. "/%{prj.name}")
    objdir ("Build-obj/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"include",
	}

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"RELEASE",
	}

	filter "system:windows"
		systemversion "latest"
	

	optimize "Speed"
	omitframepointer "On"
	editandcontinue "Off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"