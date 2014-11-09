#ifndef PLUGIN_SETTINGS_H
#define PLUGIN_SETTINGS_H

#include <QString>

const static QString STRING_NO_ROMS_INSTALLED = "No ROMs installed!";
const static QString STRING_VERY_COMMON = "Very Common";
const static QString STRING_COMMON = "Common";
const static QString STRING_UNCOMMON = "Uncommon";
const static QString STRING_RARE = "Rare";
const static QString STRING_NONE = "None";

struct Plugin_Settings {
    QString baseROM;
    int numWorlds;
    bool noDuplicates;
    bool generateNewLevels;
    QString levelScripts;
    QString standardOverworldChance;
    QString undergroundChance;
    QString underwaterChance;
    QString bridgeChance;
    QString islandChance;
};

#endif // PLUGIN_SETTINGS_H
