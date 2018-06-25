#ifndef PLUGIN_SETTINGS_H
#define PLUGIN_SETTINGS_H

#include <QString>

const static QString STRING_NO_ROMS_INSTALLED = "No ROMs installed!";
const static QString STRING_NO_LEVEL_SCRIPTS_FOUND = "No Level Scripts found!";
const static QString STRING_VERY_COMMON = "Very Common";
const static QString STRING_COMMON = "Common";
const static QString STRING_UNCOMMON = "Uncommon";
const static QString STRING_RARE = "Rare";
const static QString STRING_NONE = "None";

enum Chance {
    VERY_COMMON,
    COMMON,
    UNCOMMON,
    RARE,
    NONE
};

struct Plugin_Settings {
    QString baseROM;
    QString outputROMLocation;
    bool overwriteOuputROM;
    bool randomNumWorlds;
    int numWorlds;
    int numLevelsPerWorld;
    bool generateNewLevels;
    QString levelScripts;
    QString standardOverworldChance;
    QString undergroundChance;
    QString underwaterChance;
    QString bridgeChance;
    QString islandChance;
    uint randomSeed;
    int music;
    int toneColor;
    int graphics;
    bool infiniteLives;
    int numLives;
    bool godMode;
    bool addLuigiGame;
    Qt::CheckState superMarioOnDamage;
    Qt::CheckState lakituThrowArc;
    Qt::CheckState autoscroll;
    Qt::CheckState replaceFireFlowerWithHammerSuit;
    int enemySpeed;
    int difficultyComboIndex;
    int difficultyBulletTime;
    int difficultyHammerTime;
    int difficultyBuzzyBeetlesReplaceLoneGoombas;
    int difficultyBridgeFlyingCheepCheeps;
    int difficultyBridgeLakitus;
    int difficultyBridgeOffscreenBulletBills;
    int difficultyCastleFlyingCheepCheeps;
    int difficultyCastleLakitus;
    int difficultyCastleOffscreenBulletBills;
    int difficultyIslandFlyingCheepCheeps;
    int difficultyIslandLakitus;
    int difficultyIslandOffscreenBulletBills;
    int difficultyUndergroundFlyingCheepCheeps;
    int difficultyUndergroundLakitus;
    int difficultyUndergroundOffscreenBulletBills;
    int difficultyUnderwaterBloopers;
    int difficultyUnderwaterFlyingCheepCheeps;
    int difficultyUnderwaterLakitus;
    int difficultyUnderwaterSwimmingCheepCheeps;
    int difficultyStandardOverworldFlyingCheepCheeps;
    int difficultyStandardOverworldLakitus;
    int difficultyStandardOverworldOffscreenBulletBills;
    bool difficultyNoEnemies;
    int difficultyHammerTimeIntensity;
    int difficultyLakituSpawnChancePerLevel;
    bool difficultyDisableAllOtherEnemiesWhenALakituSpawns;
    int difficultySpawnerPriority;
};

#endif // PLUGIN_SETTINGS_H
