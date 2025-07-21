#pragma once
#include <string.h>
#include "raygui.h"

typedef enum {
    THEME_DEFAULT = 0,
    THEME_JUNGLE,
    THEME_CANDY, 
    THEME_LAVANDA,
    THEME_CYBER,
    THEME_TERMINAL,
    THEME_ASHES,
    THEME_DARK,
    THEME_CHERRY
} ThemeType;


extern ThemeType currentTheme;
extern int dropdownvalue;
extern bool dropdownEditMode;

//can be used to check for the system variable for the theme to laod into the games
void check_systemvar_theme(int argc, char** argv);

// as we know themetype is enum data set so when we receive system argument we compare with all the themes and just return the theme 
//only char* is passed we are passing only one string
ThemeType ParseThemeArg(const char* arg);


//acts as a switch for changing themes
void themeswitch(ThemeType& currenttheme);

//Calling the functions
void GuiLoadStyleJungle();

void GuiLoadStyleCandy();


void GuiLoadStyleLavanda();

void GuiLoadStyleCyber();

void GuiLoadStyleTerminal();

void GuiLoadStyleAshes();

void GuiLoadStyleDarker();

void GuiLoadStyleCherry();
