#ifndef BASE_GAME_SETTINGS_H
#define BASE_GAME_SETTINGS_H

#include <QString>

const static QString STRING_NO_ROMS_INSTALLED = "No ROMs installed!";

struct Base_Game_Settings {
    QString baseROM;
    int numWorlds;
    bool noDuplicates;
};

#endif // BASE_GAME_SETTINGS_H
