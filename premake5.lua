workspace "Voltex"
    architecture "x64"

    configurations 
    {
        "Debug",
        "Release"
    }

    filter "configurations:Debug"
        targetdir "bin/debug"
        objdir "bin-int/debug"

        symbols "On"

        defines
        {
            VX_DEBUG
        }

    filter "configurations:Release"
        targetdir "bin/release"
        objdir "bin-int/release"

        optimize "On"
        buildoptions "/MT"

        defines
        {
            VX_RELEASE
        }

project "GLFW"
    location "VoltexEngine/GLFW"
    kind "StaticLib"
    language "C"

    files
    {
        "GLFW/src/*.h",
        "GLFW/src/*.c",
        "GLFW/include/GLFW/*.h",
    }

    includedirs
    {
        "GLFW/include"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_GLFW_WIN32"
    }

project "GLAD"
    location "VoltexEngine/GLAD"
    kind "StaticLib"
    language "C"

    files
    {
        "GLAD/src/gl.c",
        "GLAD/include/glad/gl.h",
    }

    includedirs
    {
        "GLAD/include"
    }

project "VoltexEngine"
    location "VoltexEngine"
    kind "StaticLib"
    language "C++"

    pchheader "vxpch.h"
    pchsource "VoltexEngine/src/vxpch.cpp"

    files
    {
        "VoltexEngine/src/**.h",
        "VoltexEngine/src/**.cpp",
    }

    includedirs
    {
        "VoltexEngine/src",
        "GLFW/include",
        "GLAD/include"
    }

    links
    {
        "GLFW",
        "GLAD",
        "opengl32.lib"
    }

    defines
    {
        GLFW_INCLUDE_NONE
    }

project "VoltexGame"
    location "VoltexGame"
    kind "ConsoleApp"
    language "C++"

    files
    {
        "VoltexGame/src/**.h",
        "VoltexGame/src/**.cpp"
    }

    includedirs
    {
        "VoltexEngine/src",
        "GLFW/include",
        "GLAD/include"
    }

    links
    {
        "VoltexEngine"
    }