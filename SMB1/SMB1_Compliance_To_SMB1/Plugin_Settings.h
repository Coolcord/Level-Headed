#ifndef PLUGIN_SETTINGS_H
#define PLUGIN_SETTINGS_H

#include <QString>

const static QString STRING_NO_ROMS_INSTALLED = "No ROMs installed!";
const static QString STRING_NO_LEVEL_SCRIPTS_FOUND = "No Level Scripts found!";
const static QString STRING_VERY_COMMON = "Very Common";
const static QString STRING_COMMON = "Common";
const static QString STRING_UNCOMMON = "Uncommon";
const static QString STRING_RARE = "Rare";
const static QString STRING_VERY_RARE = "Very Rare (might not be included in the game)";
const static QString STRING_MYTHIC = "Mythic (might not be included in the game)";
const static QString STRING_NONE = "None";

enum Chance {
    VERY_COMMON,
    COMMON,
    UNCOMMON,
    RARE,
    VERY_RARE,
    MYTHIC,
    NONE
};

struct Plugin_Settings {
    int tab;
    QString baseROM;
    QString outputROMLocation;
    QString lastExternalConfigLocation;
    bool overwriteOuputROM;
    bool randomNumWorlds;
    int numWorlds;
    int numLevelsPerWorld;
    bool generateNewLevels;
    bool redistributePowerups;
    bool randomizeWarpZones;
    bool redistributeEnemies;
    bool performChaoticSwapOnEnemies;
    int levelScriptEnemies;
    bool noAutoScrollingLevels;
    bool noPipeIntroCutscenes;
    bool standardLevelDistribution;
    bool includeStandardOverworldLevelsInRandomDistribution;
    bool includeUndergroundLevelsInRandomDistribution;
    bool includeUnderwaterLevelsInRandomDistribution;
    bool includeBridgeLevelsInRandomDistribution;
    bool includeIslandLevelsInRandomDistribution;
    bool smbUtilityCompatibility;
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
    bool randomText;
    bool randomizeSomeAnimations;
    int graphics;
    bool combineGraphicsWithOtherPacks;
    int palette;
    int marioSprite;
    int marioSpritePalette;
    bool infiniteLives;
    bool permadeath;
    int numLives;
    bool alwaysRestartFromCurrentWorldAfterGameOver;
    bool godMode;
    bool addLuigiGame;
    bool superMarioOnDamage;
    bool difficultyEuropeanBlooperSwimHeight;
    bool lakituThrowArc;
    int powerup;
    int difficultySecondaryMushroom;
    bool fireFlowerBouncesLikeAStar;
    QString marioName;
    QString luigiName;
    int difficultyComboIndex;
    int difficultyAutoScroll;
    int difficultyAutoScrollWithFlyingCheepCheeps;
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
    int difficultyUnderwaterHammerBros;
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
    int difficultySurfingLiftSpeed;
    int difficultyPiranhaPlantType;
    int difficultyMaxNumberOfPiranhaPlants;
    int difficultyBasicEnemySpeed;
    int difficultyBowserFlameFrequency;
    int difficultyBulletBillFiringRate;
    int difficultyBulletBillSpeed;
    int difficultyLeapingParatroopaSpeed;
    int difficultyEnemyRevivalSpeed;
    int difficultyFireBarLength;
    int difficultyLongFireBarLength;
    int difficultyFlyingCheepCheepJumpHeight;
    int difficultyHammerBrosThrowRate;
    bool difficultyHammerBrosNeverJump;
    bool difficultyFireBowserThrowsALudicrousAmountOfFireballs;
    bool difficultySpeedyObjectsAndEnemies;
    bool difficultyPiranhaPlantsOnFirstLevel;
    bool difficultyPreventTheFirstLevelFromBeingUnderwater;
    bool difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow;
    bool difficultyAllowHammerBrosWhenRandomizingEnemiesInLevelScripts;
    bool difficultyAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts;
    bool difficultyAllowLakitusWhenRandomizingEnemiesInLevelScripts;
    bool difficultyAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts;
    int difficultyMaxPowerups;
    int difficultyMaxHiddenPowerups;
    int difficultyMaxOneUps;
    int difficultyMaxTenCoinBlocks;
    int difficultyMaxStars;
    int difficultyHiddenPowerupChance;
    int difficultyOneUpChance;
    int difficultyTenCoinBlockChance;
    int difficultyStarChance;
};

#endif // PLUGIN_SETTINGS_H
