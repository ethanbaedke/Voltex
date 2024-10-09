workspace "Voltex"
    configurations { "Debug", "Release" }
    platforms { "Win64" }

    filter "configurations:Debug"
        targetdir "bin/debug"
        objdir "bin-int/debug"

    filter "configurations:Release"
        targetdir "bin/release"
        objdir "bin-int/release"

project "VoltexEngine"
        location "VoltexEngine"
        kind "StaticLib"
        language "C++"
    
        pchheader "vxpch.h"
        pchsource "VoltexEngine/src/vxpch.cpp"
    
        files
        {
            "VoltexEngine/src/**.h",
            "VoltexEngine/src/**.cpp"
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

    links
    {
        "VoltexEngine"
    }

    includedirs
    {
        "VoltexEngine/src"
    }