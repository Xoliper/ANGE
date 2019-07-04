project "GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "on"
 
	targetdir ("Build/" .. outputdir .. "/%{prj.name}")
    objdir ("Build-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
        "src/glfw_config.h",
        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/vulkan.c",
        "src/window.c"
    }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}    

	filter "system:windows"
        systemversion "latest"
        
        files
        {
            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_monitor.c",
            "src/win32_time.c",
            "src/win32_window.c",
			"src/win32_tls.c",
            "src/wgl_context.c",
            "src/egl_context.c",
			"src/win32_platform.h",
        }

		defines 
		{ 
            "_GLFW_WIN32"
		}

	filter "system:linux"

		files
		{
			"src/x11_init.c",
			"src/x11_monitor.c",
			"src/x11_window.c",
			"src/glx_context.c",
			"src/egl_context.c",
			"src/x11_platform.h",
			"src/xkb_unicode.c",
			"src/posix_time.c",
			"src/posix_tls.c",
			"src/linux_joystick.c",
			"src/x11_platform.h",
			"src/xkb_unicode.h",
			"src/linux_joystick.h",
			"src/posix_time.h",
			"src/posix_tls.h",
			"src/glx_context.h",
			"src/egl_context.h",
		}

		defines
		{
			"_GLFW_X11",
		}

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
