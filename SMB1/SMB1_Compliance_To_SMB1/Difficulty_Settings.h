#ifndef DIFFICULTY_SETTINGS_H
#define DIFFICULTY_SETTINGS_H

struct Difficulty_Settings {
    bool save;
    int bulletTime;
    int hammerTime;
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
    int underwaterFlyingCheepCheeps;
    int underwaterLakitus;
    int underwaterSwimmingCheepCheeps;
    int standardOverworldFlyingCheepCheeps;
    int standardOverworldLakitus;
    int standardOverworldOffscreenBulletBills;
    int hammerTimeIntensity;
    int lakituSpawnChancePerLevel;
    bool disableAllOtherEnemiesWhenALakituSpawns;
    int spawnerPriority;
};

#endif // DIFFICULTY_SETTINGS_H
