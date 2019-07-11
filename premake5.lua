workspace "Nucleus"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Nucleus/thirdParty/GLFW/include"
IncludeDir["Glad"] = "Nucleus/thirdParty/Glad/include"
IncludeDir["ImGui"] = "Nucleus/thirdParty/imgui"
IncludeDir["glm"] = "Nucleus/thirdParty/glm"

group "Dependencies"
    include "Nucleus/thirdParty/GLFW"
    include "Nucleus/thirdParty/Glad"
    include "Nucleus/thirdParty/ImGui"
group ""

project "Nucleus"
    location "Nucleus"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ncpch.h"
    pchsource "Nucleus/src/ncpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/thirdParty/glm/glm/**.hpp",
        "%{prj.name}/thirdParty/glm/glm/**.inl",
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/thirdParty/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links {
        "Glad",
        "GLFW",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines {
            "NC_PLATFORM_WINDOWS",
            "NC_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
        
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "NC_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "NC_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "NC_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Nucleus/thirdParty/spdlog/include",
        "Nucleus/src",
        "Nucleus/thirdParty",
        "%{IncludeDir.glm}"
    }

    links {
        "Nucleus"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines {
            "NC_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "NC_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "NC_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "NC_DIST"
        runtime "Release"
        optimize "On"