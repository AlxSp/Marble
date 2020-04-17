workspace "Nucleus"
    architecture "x86_64"
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Nucleus/thirdParty/GLFW/include"
IncludeDir["Glad"] = "Nucleus/thirdParty/Glad/include"
IncludeDir["ImGui"] = "Nucleus/thirdParty/imgui"
IncludeDir["glm"] = "Nucleus/thirdParty/glm"
IncludeDir["stb_image"] = "Nucleus/thirdParty/stb_image"

PostBuildCmd = {}
PostBuildCmd["SandboxAssets"] = {
    ["EchoMessage"] = "{ECHO} Adding assets from \"%{prj.location}\".",
    ["DeleteOld"] = "{RMDIR} \"%{cfg.buildtarget.directory}/assets/\"",
    ["AddNew"] = "{COPY} \"%{prj.location}/assets\" \"%{cfg.buildtarget.directory}/assets/\"",
}

group "Dependencies"
    include "Nucleus/thirdParty/GLFW"
    include "Nucleus/thirdParty/Glad"
    include "Nucleus/thirdParty/imgui"
group ""

project "Nucleus"
    location "Nucleus"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ncpch.h"
    pchsource "Nucleus/src/ncpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/thirdParty/stb_image/**.h",
        "%{prj.name}/thirdParty/stb_image/**.cpp",
        "%{prj.name}/thirdParty/glm/glm/**.hpp",
        "%{prj.name}/thirdParty/glm/glm/**.inl",
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/thirdParty/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
    }

    links {
        "Glad",
        "GLFW",
        "ImGui"
    }

    filter "system:linux"
        pic "on"

        links {
            "GL",
            "X11",
            "Xrandr",
            "Xi",
        }

        defines {
            "NC_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "system:windows"
        systemversion "latest"

        links {
            "opengl32.lib"
        }

        defines {
            "NC_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "NC_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NC_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "NC_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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

    filter "system:linux"
        systemversion "latest"

        links {
            "Glad",
            "GLFW",
            "GL",
            "ImGui",
            "X11",
            "Xrandr",
            "Xi",
            "dl",
            "pthread",
            "stdc++fs"
        }

        defines {
            "NC_PLATFORM_LINUX"
        }

    filter "system:windows"
        systemversion "latest"

        defines {
            "NC_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "NC_DEBUG"
        runtime "Debug"
        symbols "on"

        postbuildcommands {
            PostBuildCmd["SandboxAssets"]["EchoMessage"],
            PostBuildCmd["SandboxAssets"]["DeleteOld"],
            PostBuildCmd["SandboxAssets"]["AddNew"],
        }

    filter "configurations:Release"
        defines "NC_RELEASE"
        runtime "Release"
        optimize "on"

        postbuildcommands {
            PostBuildCmd["SandboxAssets"]["EchoMessage"],
            PostBuildCmd["SandboxAssets"]["DeleteOld"],
            PostBuildCmd["SandboxAssets"]["AddNew"],
        }

    filter "configurations:Dist"
        defines "NC_DIST"
        runtime "Release"
        optimize "on"

        postbuildcommands {
            PostBuildCmd["SandboxAssets"]["EchoMessage"],
            PostBuildCmd["SandboxAssets"]["DeleteOld"],
            PostBuildCmd["SandboxAssets"]["AddNew"],
        }