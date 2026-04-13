project "tcc"
	kind "StaticLib"
	language "C"
    staticruntime "on"
	toolset "msc-v145"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"libtcc.h",
        "libtcc.c",
	}

	filter "system:windows"
        systemversion "latest"
		
	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"