#include<iostream>
#include"theme.hpp"
#include<raylib.h>
#include "raygui.h"


bool dropdownEditMode = false;
//using default theme 
 ThemeType currentTheme = THEME_DEFAULT;
//for dropdown it accepts int so we are recasting the enum into int type here
 int dropdownvalue=static_cast<int>(currentTheme);


void check_systemvar_theme(int argc, char** argv){
     if (argc > 1) {
        currentTheme = ParseThemeArg(argv[1]);
        
        
    }
    else{
        currentTheme=THEME_DEFAULT;
    }
}
ThemeType ParseThemeArg(const char* arg)
 {
    if (strcmp(arg, "default") == 0) return THEME_DEFAULT;
    if (strcmp(arg, "jungle") == 0) return THEME_JUNGLE;
    if (strcmp(arg, "candy") == 0) return THEME_CANDY;
    if (strcmp(arg, "lavanda") == 0) return THEME_LAVANDA;
    if (strcmp(arg, "cyber") == 0) return THEME_CYBER;
    if (strcmp(arg, "terminal") == 0) return THEME_TERMINAL;
    if (strcmp(arg, "ashes") == 0) return THEME_ASHES;
    if (strcmp(arg, "dark") == 0) return THEME_DARK;
    if (strcmp(arg, "cherry") == 0) return THEME_CHERRY;
    return THEME_DEFAULT; 
}

void themeswitch(ThemeType& currenttheme){
     switch(currenttheme) {
            case THEME_DEFAULT:  GuiLoadStyleDefault(); break;
            case THEME_JUNGLE:   GuiLoadStyleJungle(); break;
            case THEME_CANDY:    GuiLoadStyleCandy(); break;
            case THEME_LAVANDA:  GuiLoadStyleLavanda(); break;
            case THEME_CYBER:    GuiLoadStyleCyber(); break;
            case THEME_TERMINAL: GuiLoadStyleTerminal(); break;
            case THEME_ASHES:    GuiLoadStyleAshes(); break;
            case THEME_DARK:     GuiLoadStyleDarker(); break;
            case THEME_CHERRY:   GuiLoadStyleCherry(); break;
     }
}

void GuiLoadStyleJungle() {
    GuiLoadStyleDefault();
    // Stylish Jungle
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,    0x003F2FFF); // deep jungle green
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,   0xA8FF60FF); // neon lime
    GuiSetStyle(BUTTON,  BASE_COLOR_NORMAL,   0xFF6F91FF); // coral pink
}

void GuiLoadStyleCandy() {
    GuiLoadStyleDefault();
    // Stylish Candy
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,    0xFFF1E6FF); // blush peach
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,   0xFF6B6BFF); // dusty rose
    GuiSetStyle(BUTTON,  BASE_COLOR_NORMAL,   0xCBFFEEFF); // mint cream
}

void GuiLoadStyleDarker() {
    GuiLoadStyleDefault();
    
    // Complete Dark Theme Overhaul - Guaranteed Fix
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,    0x181C24FF);  // True dark background
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,   0xFFFFFFFF);  // Force pure white
    
    // Button - Explicitly set ALL color states
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,    0x394D6FFF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED,   0x4A5D7FFF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED,    0x2D3D56FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,     0xFFFFFFFF);  // White
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED,    0xFFFFFFFF);  // White
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED,     0xFFFFFFFF);  // White
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL,   0x283345FF);
    
    // Reset potential gradient artifacts
  
    GuiSetStyle(SLIDER, BASE_COLOR_NORMAL,     0x4A7BAAFF);
    
    // Critical: Force text rendering mode
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetFont(GetFontDefault());  // Reset to default font
}

void GuiLoadStyleCherry() {
    GuiLoadStyleDefault();
    
    // Cherry Theme with Forced Text Colors
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,    0x2E0B1EFF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,   0xFFFFFFFF);  // Pure white
    
    // Button - All states explicitly set
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,    0xD93F4DFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,     0xFFFFFFFF);  // White
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED,    0xFFFFFFFF);  // White
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED,     0xFFFFFFFF);  // White
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL,  0xA12F3BFF);
    
    // Slider - Solid color
   
    GuiSetStyle(SLIDER, BASE_COLOR_NORMAL,     0xC04050FF);
    
    // Force text rendering
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetFont(GetFontDefault());  // Reset to default font
}
void GuiLoadStyleLavanda() {
    GuiLoadStyleDefault();
    // Stylish Lavanda
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,    0x4C3575FF); // deep lavender
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,   0xF0E5E1FF); // soft ivory
    GuiSetStyle(BUTTON,  BASE_COLOR_NORMAL,   0xB984EFFF); // light mauve
}

void GuiLoadStyleCyber() {
    GuiLoadStyleDefault();
    // Stylish Cyber
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,    0x010A2FFF); // midnight blue
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,   0x00FFF7FF); // neon cyan
    GuiSetStyle(BUTTON,  BASE_COLOR_NORMAL,   0xFF00CBFF); // neon magenta
}

void GuiLoadStyleTerminal() {
    GuiLoadStyleDefault();
    // Stylish Terminal
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,    0x000100FF); // pure black
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,   0x39FF14FF); // classic green
    GuiSetStyle(BUTTON,  BASE_COLOR_NORMAL,   0x14FFECFF); // neon aqua
}

void GuiLoadStyleAshes() {
    GuiLoadStyleDefault();
    // Stylish Ashes
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,    0x2E2E2EFF); // charcoal gray
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,   0xC7F9CCFF); // pastel mint
    GuiSetStyle(BUTTON,  BASE_COLOR_NORMAL,   0xFFC300FF); // vivid gold
}

