project "box2d"
	kind "StaticLib"
	language "C"
    staticruntime "on"
	toolset "msc-v145"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/**.h",
        "src/**.c",
	}

	includedirs
	{
		"include",
	}

	filter "system:windows"
        systemversion "latest"
		defines { "_Static_assert static_assert" }
		
	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"