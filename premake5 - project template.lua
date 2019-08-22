projName = "ProjectName"
angeDir = "<Path\\To\\ANGE>"

workspace "Solution"
	architecture "x64"
	startproject (projName)

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project (projName)
	architecture "x64"
	location (projName)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	linkgroups "on"

	targetdir (projName .. "/Build/" .. outputdir .. "/%{prj.name}")
	objdir (projName .. "/Build-obj/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs 
	{
		angeDir .. "\\include",
		angeDir .. "\\include\\**",
	}

	libdirs
	{
		angeDir .. "\\lib",
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
			"libpng",
			"libjpeg",
			"zlib",
			"FreeType",
			"glad"
		}

	filter "system:linux"
		
		defines
		{
			"ANGE_PLATFORM_LINUX",
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