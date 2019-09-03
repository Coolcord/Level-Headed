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
    QString randomSeed;
    int music;
    bool combineMusicWithOtherPacks;
    int toneColor;
    bool randomSounds;
    bool randomizeSomeAnimations;
    int graphics;
    bool infiniteLives;
    bool permadeath;
    int numLives;
    bool godMode;
    bool addLuigiGame;
    bool superMarioOnDamage;
    bool lakituThrowArc;
    int powerup;
    int secondaryMushroom;
    QString marioName;
    QString luigiName;
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
    int difficultyMinimumEnemyDistance;
    int difficultyMinimumUnderwaterEnemyDistance;
    bool difficultyNoEnemies;
    bool difficultyUnlimitedTime;
    int difficultyHammerTimeIntensity;
    int difficultyReplaceCastleLoops;
    int difficultyReplaceCastleLoopsCurrent;
    int difficultyMaxLevelLength;
    int difficultyMaxLevelLengthBlocks;
    int difficultyAutoScrollChancePerLevel;
    int difficultyLakituSpawnChancePerLevel;
    int difficultyLakituRespawnSpeed;
    bool difficultyDisableAllOtherEnemiesWhenALakituSpawns;
    bool difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn;
    int difficultySpinyEggBehavior;
    int difficultySpawnerPriority;
    int difficultyPiranhaPlantType;
    int difficultyBasicEnemySpeed;
    int difficultyBowserFlameFrequency;
    int difficultyBulletBillFiringRate;
    int difficultyBulletBillSpeed;
    int difficultyEnemyRevivalSpeed;
    int difficultyFlyingCheepCheepJumpHeight;
    int difficultyHammerBrosThrowRate;
    bool difficultySpeedyObjectsAndEnemies;
};

#endif // PLUGIN_SETTINGS_H
