workspace "Voltex"
    configurations { "Debug", "Release" }
    platforms { "Win64" }

    filter "configurations:Debug"
        targetdir "bin/debug"
        objdir "bin-int/debug"

    filter "configurations:Release"
        targetdir "bin/release"
        objdir "bin-int/release"

project "VoltexProject"
    location "VoltexProject"
    kind "ConsoleApp"
    language "C++"

    files
    {
        "VoltexProject/src/**.h",
        "VoltexProject/src/**.cpp"
    }

    links
    {
        "VoltexEngine"
    }

    includedirs
    {
        "VoltexEngine/src"
    }

project "VoltexEngine"
    location "VoltexEngine"
    kind "StaticLib"
    language "C++"

    files
    {
        "VoltexEngine/src/**.h",
        "VoltexEngine/src/**.cpp"
    }