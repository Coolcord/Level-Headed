#ifndef DIFFICULTY_CONFIGURATIONS_H
#define DIFFICULTY_CONFIGURATIONS_H

#include "Difficulty_Settings.h"
#include "Plugin_Settings.h"

class Difficulty_Configurations {
public:
    Difficulty_Configurations() {}
    ~Difficulty_Configurations() {}
    void Apply_Difficulty_Settings_To_Plugin_Settings(const Difficulty_Settings &difficultySettings, Plugin_Settings *pluginSettings);
    Difficulty_Settings Normal();
    Difficulty_Settings Very_Easy();
    Difficulty_Settings Easy();
    Difficulty_Settings Hard();
    Difficulty_Settings Very_Hard();
    Difficulty_Settings Brutal();
    Difficulty_Settings Walk_In_The_Park();
    Difficulty_Settings Riddled_With_Bullets();
    Difficulty_Settings Lakitus_Challenge();
    Difficulty_Settings Cheep_Cheep_Frenzy();
    Difficulty_Settings Hammer_Time();
    Difficulty_Settings Extreme_Hammer_Time();
    Difficulty_Settings Random();

    //Alternative methods
    void Very_Easy(Plugin_Settings *pluginSettings);
    void Easy(Plugin_Settings *pluginSettings);
    void Normal(Plugin_Settings *pluginSettings);
    void Hard(Plugin_Settings *pluginSettings);
    void Very_Hard(Plugin_Settings *pluginSettings);
    void Brutal(Plugin_Settings *pluginSettings);
    void Walk_In_The_Park(Plugin_Settings *pluginSettings);
    void Riddled_With_Bullets(Plugin_Settings *pluginSettings);
    void Lakitus_Challenge(Plugin_Settings *pluginSettings);
    void Cheep_Cheep_Frenzy(Plugin_Settings *pluginSettings);
    void Hammer_Time(Plugin_Settings *pluginSettings);
    void Extreme_Hammer_Time(Plugin_Settings *pluginSettings);
    void Random(Plugin_Settings *pluginSettings);
};

#endif // DIFFICULTY_CONFIGURATIONS_H
