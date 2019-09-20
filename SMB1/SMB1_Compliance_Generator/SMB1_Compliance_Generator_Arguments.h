#ifndef SMB1_COMPLIANCE_GENERATOR_ARGUMENTS_H
#define SMB1_COMPLIANCE_GENERATOR_ARGUMENTS_H

#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Scenery.h"
#include "../Common_SMB1_Files/Level_Compliment.h"
#include "../Common_SMB1_Files/Castle.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include <QString>

struct SMB1_Compliance_Generator_Arguments {
    QString fileName;
    int numObjectBytes;
    int numEnemyBytes;
    Background::Background headerBackground;
    Scenery::Scenery headerScenery;
    Level_Compliment::Level_Compliment levelCompliment;
    Castle::Castle startCastle;
    Castle::Castle endCastle;
    Level_Type::Level_Type levelType;
    bool useAutoScroll;
    bool useMidpoints;
    int difficulty;
    int difficultyBulletTime;
    int difficultyHammerTime;
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
    int difficultyUnderwaterFlyingCheepCheeps;
    int difficultyUnderwaterHammerBros;
    int difficultyUnderwaterLakitus;
    int difficultyUnderwaterSwimmingCheepCheeps;
    int difficultyUnderwaterBloopers;
    int difficultyStandardOverworldFlyingCheepCheeps;
    int difficultyStandardOverworldLakitus;
    int difficultyStandardOverworldOffscreenBulletBills;
    int difficultyMinimumEnemyDistance;
    int difficultyMinimumUnderwaterEnemyDistance;
    bool difficultyNoEnemies;
    int difficultyHammerTimeIntensity;
    int difficultyLakituSpawnChancePerLevel;
    bool difficultyDisableAllOtherEnemiesWhenALakituSpawns;
    bool difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn;
    int difficultySpawnerPriority;
    int maxLevelLength;
};

#endif // SMB1_COMPLIANCE_GENERATOR_ARGUMENTS_H
