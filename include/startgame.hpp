#pragma once
#include <iostream>
#include <raylib.h>
#include "raygui.h"
#include "theme.hpp"
#include<string.h>

//below we are defining macros as per the platform
#if defined(__WIN32)
    #define PLATFORM_WINDOWS 1
    #define WIN32_LEAN_AND_MEAN// Minimize included headers
#define NOGDI                 // Exclude GDI (conflicts with Raylib's Rectangle/DrawText)
#define NOUSER                // Exclude USER (conflicts with CloseWindow)

#include <windows.h>     
#undef near
#undef far
#undef Rectangle  
#elif defined(__linux__)
    #define PLATFORM_LINUX 1
    #include <cstdlib>
#elif defined(__APPLE__)
    #define PLATFORM_MACOS 1
    #include <cstdlib>
#endif

class Gamelauncher{
    public:
#if PLATFORM_WINDOWS 
    //lpcwstr is a pointer to wide string specially used for the path string that uses 2 bytes rather than 1 byte where path is used for the link of the file
    std::wstring filepath;

    //this is a structure that tells windows what to do when it is launched we keep it default for now and only include its size
    STARTUPINFOW si = { sizeof(STARTUPINFO) };

    //this tells windows about newer process and is a structure
    PROCESS_INFORMATION pi;
    //we use it later to see if the process has been created
    bool success;
    ThemeType themearg;
#elif PLATFORM_LINUX 
    const char* filepath;
    std::string cmdl;
#elif PLATFORM_MACOS 
 const char* filepath;
 std::string cmdl;
 #endif
    void dir(const char* path);
    void loadgame(ThemeType theme);

};