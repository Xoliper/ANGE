projName = "ProjectName"
angeDir = "<Path\\To\\ANGE>"

workspace "Solution"
	startproject (projName)

	configurations
	{
		"Debug",
		"Release",
	}

--Add options
	newoption {
	   trigger     = "arch",
	   value       = "VALUE",
	   description = "Choose a particular CPU architecture",
	   allowed = {
		  { "x86",  "32 bit architecture" },
		  { "x64",  "64 bit architecture" }
	   }
	}

	newoption {
	   trigger     = "compiler",
	   value       = "VALUE",
	   description = "Choose a particular compiler to use in build",
	   allowed = {
		  { "clang",    "Clang (clang)" },
		  { "gcc",  "GNU GCC (gcc/g++)" },
		  { "msc",  "Microsoft Visual C++ Compiler" }
	   }
	}

print("----------------------------")

-- Architecture
if not _OPTIONS["arch"] then
   _OPTIONS["arch"] = "x64"
end
architecture (_OPTIONS["arch"])

-- Print message
if _OPTIONS["arch"] == "x64" then
	print("Choosen architecture - x64");
elseif _OPTIONS["arch"] == "x86" then
	print("Choosen architecture - x86");
end

-- Toolset
if not _OPTIONS["compiler"] then
   _OPTIONS["compiler"] = "clang"
end
toolset (_OPTIONS["compiler"])

-- Print message & Fix GCC
if _OPTIONS["compiler"] == "gcc" then
	print("Configuring for      - GCC")
	makesettings [[
	CC = gcc
	]]
elseif _OPTIONS["compiler"] == "clang" then
	print("Configuring for - CLANG")
elseif _OPTIONS["compiler"] == "msc" then
	print("Configuring for - MSVC")
end

print("----------------------------")

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

	filter "configurations:Debug"
		defines "ANGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ANGE_RELEASE"
		runtime "Release"
		optimize "on"