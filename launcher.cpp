#include<iostream>
#include <raylib.h>
#include "raygui.h"
#include "theme.hpp"
#include "core_var.hpp"
#include "startgame.hpp"

enum GameState{Settings, Launcher};
GameState currentstate = Launcher;
int mute_counter=0;
float vol = 0.5f; // Initial volume

//Function declaration for loading and playing music
void randomize_music(Music &music, const string &filename);
//we are assining unique int value to each theme name so it would be easier to use in the upcoming file

/*
====================================================================================================================
this would be central code for the launcher to launch the menu screen that shows other games:
1)acts as a launcher
2)when user chooses a tile or an icon of the game it launches the exe file of the game
3)provide them with an option of theme here for the customisation
4)maybe add an easy option of gui implementation to add game directly through gui rather than through code
5)might need to add more features to give it launcher like feel
=====================================================================================================================

*/
namespace fs = std::filesystem;
//we use char** because we need array of strings from the commandline
int main(int argc, char** argv){

 SetConfigFlags(FLAG_WINDOW_RESIZABLE);
 InitWindow(screen_x,screen_y, "GameVault: A Bundle of Minnigames!");
 Texture2D menuBackground = LoadTexture("assets/settings.png");
 InitAudioDevice();
//Loading Music
for(const auto& entry : fs::directory_iterator("assets/music")){
    if(entry.path().extension() == ".ogg" || entry.path().extension() == ".wav"){
        musicFiles.push_back(entry.path().string());
    }
}

//shuffle the music files
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
shuffle(musicFiles.begin(), musicFiles.end(),default_random_engine(seed));

size_t currentTrackIndex = 0;
Music background_music = LoadMusicStream(musicFiles[currentTrackIndex].c_str());
background_music.looping = false;
PlayMusicStream(background_music);
SetMusicVolume(background_music, 0.05f);

//Loading Font
myfont = LoadFontEx("assets\\space-nova-font\\SpaceNova-6Rpd1.otf", 50, nullptr, 0);
    //this fn is defined in our theme.hpp that checks for sys var and switches up the theme
    if(argc>1){
    check_systemvar_theme(argc, argv);
    //after getting theme changed here we cast that theme into int type to get the styles and everything
    dropdownvalue = static_cast<int>(currentTheme);
    themeswitch(currentTheme);
    }
  
    SetTargetFPS(144);

    bool isMuted = false;
    float originalVolume = 0.05f;
    SetMusicVolume(background_music, originalVolume);
 
 
     //this is actaul gamebundle class that holds all the game and the buttons to be loaded in the landing page
     GameBundle bundle;

     //this will be our gui class that holds everything
    GUI gui;
    //Game Loop
   
    while (!WindowShouldClose()) {
    



        UpdateMusicStream(background_music);
        //Checking if the song is finished
        if (GetMusicTimePlayed(background_music) >= GetMusicTimeLength(background_music) - 0.1f) {
            currentTrackIndex = (currentTrackIndex + 1) % musicFiles.size();
            randomize_music(background_music, musicFiles[currentTrackIndex]);
        }
        // Check for fullscreen toggle
        if(IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)){
        ToggleFullscreen();
        }

        if(currentstate==Launcher){
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        gui.load_up_gui();
        //For the music playing
        DrawText("Music Playing:", 20, (float)GetRenderHeight()-65, 20, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        filesystem::path musicPath(musicFiles[currentTrackIndex]);
        DrawText(musicPath.filename().string().c_str(), 20, (float)GetRenderHeight()-40, 20, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        //string.c_str() is used to conver std::string to const char* for DrawText


        // Draw the background image
        bundle.draw_buttons_of_game();
        //checks for user if they have selected any buttons
        bundle.see_for_the_user_choice();

       
      
        // Go to settings state
        if (GuiButton((Rectangle){10, 10, 200, 40}, "Settings")) {
            currentstate = Settings;
        } 
        // Theme dropdown
        //GuiDropdownBox is a function that creates a dropdown menu that accepts rectangle structure 
        //&currentTheme is to store the theme
        //dropdownEditMode is a boolean that determines if the dropdown is currently open or closed.
       
        //after we recieve integer values for the drop down we recast the data type into enum type
        currentTheme=static_cast<ThemeType>(dropdownvalue);
        // Apply selected theme
       themeswitch(currentTheme);

        if (GuiButton((Rectangle){ (float)GetRenderWidth() - 70, (float)GetRenderHeight()-50, 50, 30 }, isMuted ? "Unmute" : "Mute")) {
        isMuted = !isMuted;
        if(mute_counter%2==0||mute_counter==0){
        SetMasterVolume(0);}
        else if(mute_counter%2!=0){
            SetMasterVolume(vol);
        }
        mute_counter++;
        }
        EndDrawing();
    }
    if(currentstate==Settings){
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawTexturePro(
        menuBackground,
        (Rectangle){0, 0, (float)menuBackground.width, (float)menuBackground.height},  // source
        (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()},         // destination
        (Vector2){0, 0},                                                               // origin
        0.0f,                                                                          // rotation
        WHITE                                                                          // tint
    );
       // DrawText("Choose a Theme", GetRenderWidth()/2 - MeasureText("Choose a Theme", 30)/2, 100, 30, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
           DrawLine(0, 75, GetRenderWidth(), 75, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
    DrawText("Settings", GetRenderWidth()/2 - MeasureText("Settings", 30)/2, 50, 30, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

    // Calculate center positions for form layout
    int centerX   = GetRenderWidth() / 2;
    int formWidth = 400;
    int leftCol   = centerX - formWidth/2;
    int rightCol  = centerX + 50;

    // Volume Section (Y = 120)
    DrawText("Volume:", leftCol, 120, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
    DrawText(TextFormat("%.0f%%", vol*100), rightCol + 210, 120, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
    if (GuiSlider((Rectangle){(float)rightCol, 115, 200, 20}, nullptr, nullptr, &vol, 0.0f, 1.0f)) {
        SetMasterVolume(vol);
    }

    // FPS Limit Section (moved up to Y = 170)
    static float maxFPS = 144.0f;
    DrawText("FPS Limit:", leftCol, 170, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
    DrawText(TextFormat("%.0f", maxFPS), rightCol + 210, 170, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
    if (GuiSlider((Rectangle){(float)rightCol, 165, 200, 20}, nullptr, nullptr, &maxFPS, 30.0f, 144.0f)) {
        SetTargetFPS((int)maxFPS);
    }

    // Theme Section (moved up to Y = 220)
    DrawText("Theme:", leftCol, 220, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
    if (GuiDropdownBox((Rectangle){(float)rightCol, 215, 200, 30},
                       "Default;Jungle;Candy;Lavanda;Ashes;Dark;Cherry",
                       &dropdownvalue, gui.dropdownEditMode)) {
        gui.dropdownEditMode = !gui.dropdownEditMode;
    }
    currentTheme = static_cast<ThemeType>(dropdownvalue);
    themeswitch(currentTheme);

    // Back Button (still at bottom)
    if (GuiButton((Rectangle){(float)centerX - 100, (float)GetRenderHeight() - 80, 200, 40}, "<- Back to Launcher")) {
        currentstate = Launcher; // Go back to launcher
    }

        EndDrawing();
    }






}
    UnloadMusicStream(background_music);
    CloseAudioDevice();
    UnloadFont(myfont);
    CloseWindow();
    return 0;

}

void randomize_music(Music &music, const string &filename){
     UnloadMusicStream(music);
     music = LoadMusicStream(filename.c_str());
     PlayMusicStream(music);
     SetMusicVolume(music, 0.005f);
     music.looping = true;

}
