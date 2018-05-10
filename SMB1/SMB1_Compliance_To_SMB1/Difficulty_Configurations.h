#ifndef DIFFICULTY_CONFIGURATIONS_H
#define DIFFICULTY_CONFIGURATIONS_H

#include "Difficulty_Settings.h"
#include "Plugin_Settings.h"

class Difficulty_Configurations {
public:
    Difficulty_Configurations() {}
    ~Difficulty_Configurations() {}
    void Apply_Difficulty_Settings_To_Plugin_Settings(const Difficulty_Settings &difficultySettings, Plugin_Settings *pluginSettings);
    Difficulty_Settings Easy();
    void Easy(Plugin_Settings *pluginSettings);
    Difficulty_Settings Normal();
    void Normal(Plugin_Settings *pluginSettings);
    Difficulty_Settings Hard();
    void Hard(Plugin_Settings *pluginSettings);
};

#endif // DIFFICULTY_CONFIGURATIONS_H
