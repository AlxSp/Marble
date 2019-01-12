workspace "Nucleus"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Nucleus"
    location "Nucleus"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/thirdParty/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "NC_PLATFORM_WINDOWS",
            "NC_BUILD_DLL"
        }
        
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "NC_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NC_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "NC_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Nucleus/thirdParty/spdlog/include",
        "Nucleus/src"
    }

    links {
        "Nucleus"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "NC_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "NC_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NC_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "NC_DIST"
        optimize "On"