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
    int tab;
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
    bool combineMusicWithOtherPacks;
    int toneColor;
    int graphics;
    bool infiniteLives;
    int numLives;
    bool godMode;
    int timerSpeed;
    bool addLuigiGame;
    bool superMarioOnDamage;
    bool lakituThrowArc;
    int powerup;
    int secondaryMushroom;
    int difficultyComboIndex;
    int difficultyAutoScroll;
    int difficultyBulletTime;
    int difficultyHammerTime;
    int difficultyWalkingHammerBros;
    int difficultyBuzzyBeetlesReplaceLoneGoombas;
    int difficultyBridgeFlyingCheepCheeps;
    int difficultyBridgeLakitus;
    int difficultyBridgeOffscreenBulletBills;
    int difficultyCastleFireBars;
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
    bool difficultyStartWithFireFlowerOnRoomChange;
    int difficultyHammerTimeIntensity;
    int difficultyReplaceCastleLoops;
    int difficultyReplaceCastleLoopsCurrent;
    int difficultyAutoScrollChancePerLevel;
    int difficultyLakituSpawnChancePerLevel;
    int difficultyLakituRespawnSpeed;
    bool difficultyDisableAllOtherEnemiesWhenALakituSpawns;
    int difficultySpinyEggBehavior;
    int difficultySpawnerPriority;
    int difficultyPiranhaPlantType;
    int difficultyBasicEnemySpeed;
    int difficultyBulletBillSpeed;
    bool difficultySpeedyObjectsAndEnemies;
};

#endif // PLUGIN_SETTINGS_H
