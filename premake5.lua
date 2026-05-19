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
IncludeDir["entt"] = "PerplexCore/vendor/entt/include"
IncludeDir["nlohmann_json"] = "PerplexCore/vendor/nlohmann_json/include"
IncludeDir["box2d"] = "PerplexCore/vendor/box2d/include"

group "Dependencies"
    include "PerplexRenderer/vendor/glew"
    include "PerplexCore/vendor/GLFW"
    include "PerplexCore/vendor/imgui"
    include "PerplexCore/vendor/efsw"
    include "PerplexCore/vendor/tcc"
    include "PerplexCore/vendor/box2d"
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

    pchheader "pxr/pch.h"
    pchsource "PerplexRenderer/src/pch.cpp"

    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/include",
        "%{prj.name}/vendor/stb_image/include",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glew}",
        "%{IncludeDir.glm}",
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
        "/execution-charset:utf-8"
    }

    filter "system:windows"
        systemversion "latest"
        defines "PXR_PLATFORM_WINDOWS"
        fatalwarnings { "All" }
    
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

    pchheader "Perplex/pch.h"
    pchsource "PerplexCore/src/pch.cpp"

    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/res/scripting/include/**.h",
        "%{prj.name}/res/scripting/include/**.cpp",
    }

    includedirs
    {
        "PerplexRenderer/include",
        "%{prj.name}/include",
        "%{prj.name}/res/scripting/include",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.nlohmann_json}",
        "%{IncludeDir.box2d}",
    }

    links
    {
        "PerplexRenderer",
        "GLFW",
        "ImGui",
        "efsw",
        "tcc",
        "box2d"
    }

    buildoptions
    {
        "/execution-charset:utf-8"
    }

    filter "system:windows"
        systemversion "latest"
        fatalwarnings { "All" }

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
        "%{prj.name}/src",
        "PerplexRenderer/include",
        "PerplexCore/vendor/spdlog/include",
        "PerplexCore/include",
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
        "/execution-charset:utf-8"
    }

    pchheader "pch.h"
    pchsource "PerplexEditor/src/pch.cpp"

    filter "system:windows"
        systemversion "latest"
        fatalwarnings { "All" }

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

    pchheader "pch.h"
    pchsource "PerplexRuntime/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "PerplexRenderer/include",
        "PerplexCore/vendor/spdlog/include",
        "PerplexCore/include",
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

    fatalwarnings { "All" }

    buildoptions
    {
        "/execution-charset:utf-8"
    }

    filter "system:windows"
        systemversion "latest"
        fatalwarnings { "All" }

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
