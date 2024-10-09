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
        "VoltexEngine/GLFW/src/*.h",
        "VoltexEngine/GLFW/src/*.c",
        "VoltexEngine/GLFW/include/GLFW/*.h",
    }

    includedirs
    {
        "VoltexEngine/GLFW/include"
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
        "VoltexEngine/GLAD/src/gl.c",
        "VoltexEngine/GLAD/include/glad/gl.h",
        "VoltexEngine/GLAD/include/KHR/khrplatform.h"
    }

    includedirs
    {
        "VoltexEngine/GLAD/include"
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
        "VoltexEngine/GLFW/include",
        "VoltexEngine/GLAD/include"
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
        "VoltexEngine/GLFW/include",
        "VoltexEngine/GLAD/include"
    }

    links
    {
        "VoltexEngine"
    }