workspace "Marble"
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
IncludeDir["GLFW"] = "Marble/thirdParty/GLFW/include"
IncludeDir["Glad"] = "Marble/thirdParty/Glad/include"
IncludeDir["ImGui"] = "Marble/thirdParty/imgui"
IncludeDir["glm"] = "Marble/thirdParty/glm"
IncludeDir["stb_image"] = "Marble/thirdParty/stb_image"

PostBuildCmd = {}
PostBuildCmd["SandboxAssets"] = {
    ["EchoMessage"] = "{ECHO} Adding assets from \"%{prj.location}\".",
    ["DeleteOld"] = "{RMDIR} \"%{cfg.buildtarget.directory}/assets/\"",
    ["AddNew"] = "{COPY} \"%{prj.location}/assets\" \"%{cfg.buildtarget.directory}/assets/\"",
}

group "Dependencies"
    include "Marble/thirdParty/GLFW"
    include "Marble/thirdParty/Glad"
    include "Marble/thirdParty/imgui"
group ""

project "Marble"
    location "Marble"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mblpch.h"
    pchsource "Marble/src/mblpch.cpp"

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
            "GLFW_INCLUDE_NONE"
        }

    filter "system:windows"
        systemversion "latest"

        links {
            "opengl32.lib"
        }

        defines {
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "MBL_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "MBL_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "MBL_DIST"
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
        "Marble/thirdParty/spdlog/include",
        "Marble/src",
        "Marble/thirdParty",
        "%{IncludeDir.glm}"
    }

    links {
        "Marble"
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
            "MBL_PLATFORM_LINUX"
        }

    filter "system:windows"
        systemversion "latest"

        defines {
            "MBL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "MBL_DEBUG"
        runtime "Debug"
        symbols "on"

        postbuildcommands {
            PostBuildCmd["SandboxAssets"]["EchoMessage"],
            PostBuildCmd["SandboxAssets"]["DeleteOld"],
            PostBuildCmd["SandboxAssets"]["AddNew"],
        }

    filter "configurations:Release"
        defines "MBL_RELEASE"
        runtime "Release"
        optimize "on"

        postbuildcommands {
            PostBuildCmd["SandboxAssets"]["EchoMessage"],
            PostBuildCmd["SandboxAssets"]["DeleteOld"],
            PostBuildCmd["SandboxAssets"]["AddNew"],
        }

    filter "configurations:Dist"
        defines "MBL_DIST"
        runtime "Release"
        optimize "on"

        postbuildcommands {
            PostBuildCmd["SandboxAssets"]["EchoMessage"],
            PostBuildCmd["SandboxAssets"]["DeleteOld"],
            PostBuildCmd["SandboxAssets"]["AddNew"],
        }