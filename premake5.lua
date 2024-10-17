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
    location "GLFW"
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
    location "GLAD"
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

project "GLM"
    location "GLM"
    kind "StaticLib"
    language "C"

    files
    {
        "GLM/**.hpp"
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
        "VoltexEngine/src/Shaders/*.vert",
        "VoltexEngine/src/Shaders/*.frag"
    }

    includedirs
    {
        "VoltexEngine/src",
        "GLFW/include",
        "GLAD/include",
        "GLM"
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
        "VoltexGame/src/**.cpp",
        "VoltexGame/textures/*.png"
    }

    includedirs
    {
        "VoltexEngine/src",
        "GLFW/include",
        "GLAD/include",
        "GLM"
    }

    links
    {
        "VoltexEngine"
    }

project "VoltexTool"
    location "VoltexTool"
    kind "ConsoleApp"
    language "C++"

    files
    {
        "VoltexTool/src/**.h",
        "VoltexTool/src/**.cpp",
        "VoltexTool/textures/*.png"
    }

    includedirs
    {
        "VoltexEngine/src",
        "GLFW/include",
        "GLAD/include",
        "GLM"
    }

    links
    {
        "VoltexEngine"
    }