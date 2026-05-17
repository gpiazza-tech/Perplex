workspace "Perplex"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glew"] = "PerplexRenderer/vendor/glew/include"
IncludeDir["spdlog"] = "PerplexCore/vendor/spdlog/include"
IncludeDir["GLFW"] = "PerplexCore/vendor/GLFW/include"
IncludeDir["ImGui"] = "PerplexCore/vendor/imgui"
IncludeDir["glm"] = "PerplexCore/vendor/glm"
IncludeDir["stb_image"] = "PerplexCore/vendor/stb_image"
IncludeDir["entt"] = "PerplexCore/vendor/entt/include"
IncludeDir["nlohmann_json"] = "PerplexCore/vendor/nlohmann_json/include"
IncludeDir["efsw"] = "PerplexCore/vendor/efsw/include"

group "Dependencies"
    include "PerplexRenderer/vendor/glew"
    include "PerplexCore/vendor/GLFW"
    include "PerplexCore/vendor/imgui"
    include "PerplexCore/vendor/efsw"
    include "PerplexCore/vendor/tcc"
group ""

project "PerplexRenderer"
    location "PerplexRenderer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    toolset "msc-v145"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{prj.name}/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/include",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glew}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.glm}/glm",
        "%{IncludeDir.stb_image}",
    }

    defines
    {
        "GLEW_STATIC",
    }

    links
    {
        "glew",
        "opengl32.lib"
    }

    buildoptions
    {
        "/utf-8"
    }

    filter "system:windows"
        systemversion "latest"
        defines "PXR_PLATFORM_WINDOWS"
    
    filter "configurations:Debug"
        defines "PXR_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "PXR_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "PXR_DIST"
        runtime "Release"
        optimize "on"

project "PerplexCore"
    location "PerplexCore"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    toolset "msc-v145"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "PerplexCore/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/res/scripting/include/**.h",
        "%{prj.name}/res/scripting/include/**.cpp",
    }

    includedirs
    {
        "PerplexRenderer/include",
        "PerplexRenderer/src",
        "%{prj.name}/src",
        "%{prj.name}/res/scripting/include",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.glm}/glm",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.nlohmann_json}",
        "%{IncludeDir.efsw}",
    }

    links
    {
        "PerplexRenderer",
        "GLFW",
        "ImGui",
        "opengl32.lib",
        "efsw",
        "tcc"
    }

    buildoptions
    {
        "/utf-8"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HW_PLATFORM_WINDOWS",
            "HW_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
    filter "configurations:Debug"
        defines "HW_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "HW_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "HW_DIST"
        runtime "Release"
        optimize "on"

project "PerplexEditor"
    location "PerplexEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    toolset "msc-v145"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "PerplexRenderer/include",
        "PerplexRenderer/src",
        "PerplexCore/vendor/spdlog/include",
        "PerplexCore/src",
        "PerplexCore/res/scripting/include",
        "PerplexCore/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.nlohmann_json}",
    }

    links
    {
        "PerplexCore"
    }

    buildoptions
    {
        "/utf-8"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HW_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "HW_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "HW_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "HW_DIST"
        runtime "Release"
        optimize "on"

project "PerplexRuntime"
    location "PerplexRuntime"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    toolset "msc-v145"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "PerplexRenderer/include",
        "PerplexRenderer/src",
        "PerplexCore/vendor/spdlog/include",
        "PerplexCore/src",
        "PerplexCore/res/scripting/include",
        "PerplexCore/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.nlohmann_json}",
    }
    
    links
    {
        "PerplexCore"
    }

    buildoptions
    {
        "/utf-8"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HW_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "HW_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "HW_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "HW_DIST"
        runtime "Release"
        optimize "on"
