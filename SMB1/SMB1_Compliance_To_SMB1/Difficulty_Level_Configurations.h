#ifndef DIFFICULTY_LEVEL_CONFIGURATIONS_H
#define DIFFICULTY_LEVEL_CONFIGURATIONS_H

#include "Difficulty_Level_Settings.h"
#include "Plugin_Settings.h"

class Difficulty_Level_Configurations {
public:
    Difficulty_Level_Configurations() {}
    ~Difficulty_Level_Configurations() {}
    void Apply_Difficulty_Settings_To_Plugin_Settings(const Difficulty_Level_Settings &difficultySettings, Plugin_Settings *pluginSettings);
    Difficulty_Level_Settings Normal();
    Difficulty_Level_Settings Very_Easy();
    Difficulty_Level_Settings Easy();
    Difficulty_Level_Settings Hard();
    Difficulty_Level_Settings Very_Hard();
    Difficulty_Level_Settings Brutal();
    Difficulty_Level_Settings Walk_In_The_Park();
    Difficulty_Level_Settings Just_Keep_Scrolling();
    Difficulty_Level_Settings Riddled_With_Bullets();
    Difficulty_Level_Settings Lakitus_Challenge();
    Difficulty_Level_Settings Cheep_Cheep_Frenzy();
    Difficulty_Level_Settings Hammer_Time();
    Difficulty_Level_Settings Extreme_Hammer_Time();
    Difficulty_Level_Settings Turn_Up_The_Heat();
    Difficulty_Level_Settings Too_Hot_To_Handle();
    Difficulty_Level_Settings Random();

    //Alternative methods
    void Very_Easy(Plugin_Settings *pluginSettings);
    void Easy(Plugin_Settings *pluginSettings);
    void Normal(Plugin_Settings *pluginSettings);
    void Hard(Plugin_Settings *pluginSettings);
    void Very_Hard(Plugin_Settings *pluginSettings);
    void Brutal(Plugin_Settings *pluginSettings);
    void Walk_In_The_Park(Plugin_Settings *pluginSettings);
    void Just_Keep_Scrolling(Plugin_Settings *pluginSettings);
    void Riddled_With_Bullets(Plugin_Settings *pluginSettings);
    void Lakitus_Challenge(Plugin_Settings *pluginSettings);
    void Cheep_Cheep_Frenzy(Plugin_Settings *pluginSettings);
    void Hammer_Time(Plugin_Settings *pluginSettings);
    void Extreme_Hammer_Time(Plugin_Settings *pluginSettings);
    void Turn_Up_The_Heat(Plugin_Settings *pluginSettings);
    void Too_Hot_To_Handle(Plugin_Settings *pluginSettings);
    void Random(Plugin_Settings *pluginSettings);
};

#endif // DIFFICULTY_LEVEL_CONFIGURATIONS_H
