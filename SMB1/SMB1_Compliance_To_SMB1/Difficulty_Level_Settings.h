#ifndef DIFFICULTY_LEVEL_SETTINGS_H
#define DIFFICULTY_LEVEL_SETTINGS_H

struct Difficulty_Level_Settings {
    bool save;
    int bulletTime;
    int hammerTime;
    int buzzyBeetlesReplaceLoneGoombas;
    int bridgeFlyingCheepCheeps;
    int bridgeLakitus;
    int bridgeOffscreenBulletBills;
    int castleFlyingCheepCheeps;
    int castleLakitus;
    int castleOffscreenBulletBills;
    int islandFlyingCheepCheeps;
    int islandLakitus;
    int islandOffscreenBulletBills;
    int undergroundFlyingCheepCheeps;
    int undergroundLakitus;
    int undergroundOffscreenBulletBills;
    int underwaterBloopers;
    int underwaterFlyingCheepCheeps;
    int underwaterLakitus;
    int underwaterSwimmingCheepCheeps;
    int standardOverworldFlyingCheepCheeps;
    int standardOverworldLakitus;
    int standardOverworldOffscreenBulletBills;
    bool noEnemies;
    int hammerTimeIntensity;
    int lakituSpawnChancePerLevel;
    bool lakituThrowArc;
    bool disableAllOtherEnemiesWhenALakituSpawns;
    int spawnerPriority;
    bool superMarioOnDamage;
};

#endif // DIFFICULTY_LEVEL_SETTINGS_H
