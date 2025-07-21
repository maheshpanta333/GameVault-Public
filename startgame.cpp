#include "startgame.hpp"
#include <vector>
#include <string>
using namespace std;


std::wstring themeToString(ThemeType theme) {
    switch(theme) {
        case THEME_DEFAULT:  return L"default";
        case THEME_JUNGLE:   return L"jungle";
        case THEME_CANDY:    return L"candy";
        case THEME_LAVANDA:  return L"lavanda";
        case THEME_CYBER:    return L"cyber";
        case THEME_TERMINAL: return L"terminal";
        case THEME_ASHES:    return L"ashes";
        case THEME_DARK:     return L"dark";
        case THEME_CHERRY:   return L"cherry";
        default:             return L"default";
    }
}
void Gamelauncher::dir(const char* path) {
    #if PLATFORM_WINDOWS
    //windows api needs wide string so cant assign file path to path directly
    //we convert char* to wstring ie wide string
    //windows use utf16 encoding but we have utf 8
    //fn calculates the number of wide character with null character where path gives the type of data for conversion so basically it calculates output buffer size
  int wchars_num = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
    filepath.resize(wchars_num - 1); // -1 to remove null terminator for std::wstring
    MultiByteToWideChar(CP_UTF8, 0, path, -1, &filepath[0], wchars_num);
        
        #elif PLATFORM_LINUX
        filepath=path;
        themearg=theme;
        system();
        #elif PLATFORM_MACOS
        filepath=path;
        themearg=theme;
    #endif
}


void Gamelauncher::loadgame(ThemeType theme){
#if PLATFORM_WINDOWS
// Convert themearg (ThemeType) to a command line argument string
std::wstring themeArgStr = themeToString(theme);
std::wstring cmdLine = L"";
cmdLine += filepath;
cmdLine += L" ";
cmdLine += themeArgStr;

// CreateProcessW requires a modifiable buffer
std::vector<wchar_t> cmdLineVec(cmdLine.begin(), cmdLine.end());
cmdLineVec.push_back(0); // null-terminate

success = CreateProcessW(
    NULL,                   // Application name (NULL means use command line)
    cmdLineVec.data(),      // Command line WITH THEME
    NULL,                   // Default security
    NULL,                   // Default security
    FALSE,                  // Don't inherit handles
    CREATE_NO_WINDOW | DETACHED_PROCESS, // Don't show a console window, and detach
    NULL,                   // Use parent's environment
    NULL,                   // Use parent's directory
    &si,                    // Startup info
    &pi                     // Process info (output)
);

//now we check if the cmdline has been passed
if(success){
    //when we terminate we ought to free resources
    //so we close handles form pi structure that has process information and closes all the process and threads running to free the system
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

}
else{
    cout<<"Error in launching the game!"<<endl;

}
#elif PLATFORM_LINUX
cmdl=filepath
#elif PLATFORM_MACOS


#endif
}