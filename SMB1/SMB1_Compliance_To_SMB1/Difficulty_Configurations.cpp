#include "Difficulty_Configurations.h"
#include <assert.h>

void Difficulty_Configurations::Apply_Difficulty_Settings_To_Plugin_Settings(const Difficulty_Settings &difficultySettings, Plugin_Settings *pluginSettings) {
    assert(pluginSettings);
    pluginSettings->difficultyBulletTime = difficultySettings.bulletTime;
    pluginSettings->difficultyHammerTime = difficultySettings.hammerTime;
    pluginSettings->difficultyBridgeFlyingCheepCheeps = difficultySettings.bridgeFlyingCheepCheeps;
    pluginSettings->difficultyBridgeLakitus = difficultySettings.bridgeLakitus;
    pluginSettings->difficultyBridgeOffscreenBulletBills = difficultySettings.bridgeOffscreenBulletBills;
    pluginSettings->difficultyCastleFlyingCheepCheeps = difficultySettings.castleFlyingCheepCheeps;
    pluginSettings->difficultyCastleLakitus = difficultySettings.castleLakitus;
    pluginSettings->difficultyCastleOffscreenBulletBills = difficultySettings.castleOffscreenBulletBills;
    pluginSettings->difficultyIslandFlyingCheepCheeps = difficultySettings.islandFlyingCheepCheeps;
    pluginSettings->difficultyIslandLakitus = difficultySettings.islandLakitus;
    pluginSettings->difficultyIslandOffscreenBulletBills = difficultySettings.islandOffscreenBulletBills;
    pluginSettings->difficultyUndergroundFlyingCheepCheeps = difficultySettings.undergroundFlyingCheepCheeps;
    pluginSettings->difficultyUndergroundLakitus = difficultySettings.undergroundLakitus;
    pluginSettings->difficultyUndergroundOffscreenBulletBills = difficultySettings.undergroundOffscreenBulletBills;
    pluginSettings->difficultyUnderwaterFlyingCheepCheeps = difficultySettings.underwaterFlyingCheepCheeps;
    pluginSettings->difficultyUnderwaterLakitus = difficultySettings.underwaterLakitus;
    pluginSettings->difficultyUnderwaterSwimmingCheepCheeps = difficultySettings.underwaterSwimmingCheepCheeps;
    pluginSettings->difficultyStandardOverworldFlyingCheepCheeps = difficultySettings.standardOverworldFlyingCheepCheeps;
    pluginSettings->difficultyStandardOverworldLakitus = difficultySettings.standardOverworldLakitus;
    pluginSettings->difficultyStandardOverworldOffscreenBulletBills = difficultySettings.standardOverworldOffscreenBulletBills;
    pluginSettings->difficultyHammerTimeIntensity = difficultySettings.hammerTimeIntensity;
    pluginSettings->difficultyLakituSpawnChancePerLevel = difficultySettings.lakituSpawnChancePerLevel;
    pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns = difficultySettings.disableAllOtherEnemiesWhenALakituSpawns;
    pluginSettings->difficultySpawnerPriority = difficultySettings.spawnerPriority;
}

Difficulty_Settings Difficulty_Configurations::Normal() {
    Difficulty_Settings difficultySettings;
    difficultySettings.bulletTime = 7;
    difficultySettings.hammerTime = 9;
    difficultySettings.bridgeFlyingCheepCheeps = 3;
    difficultySettings.bridgeLakitus = 11;
    difficultySettings.bridgeOffscreenBulletBills = 11;
    difficultySettings.castleFlyingCheepCheeps = 11;
    difficultySettings.castleLakitus = 11;
    difficultySettings.castleOffscreenBulletBills = 11;
    difficultySettings.islandFlyingCheepCheeps = 11;
    difficultySettings.islandLakitus = 11;
    difficultySettings.islandOffscreenBulletBills = 6;
    difficultySettings.undergroundFlyingCheepCheeps = 11;
    difficultySettings.undergroundLakitus = 11;
    difficultySettings.undergroundOffscreenBulletBills = 11;
    difficultySettings.underwaterFlyingCheepCheeps = 11;
    difficultySettings.underwaterLakitus = 11;
    difficultySettings.underwaterSwimmingCheepCheeps = 6;
    difficultySettings.standardOverworldFlyingCheepCheeps = 11;
    difficultySettings.standardOverworldLakitus = 6;
    difficultySettings.standardOverworldOffscreenBulletBills = 11;
    difficultySettings.hammerTimeIntensity = 10;
    difficultySettings.lakituSpawnChancePerLevel = 15;
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = true;
    difficultySettings.spawnerPriority = 1;
    return difficultySettings;
}

Difficulty_Settings Difficulty_Configurations::Very_Easy() {
    //Turn Everything Off for Very Easy
    Difficulty_Settings difficultySettings = this->Normal();
    difficultySettings.bulletTime = 11;
    difficultySettings.hammerTime = 11;
    difficultySettings.bridgeFlyingCheepCheeps = 11;
    difficultySettings.bridgeLakitus = 11;
    difficultySettings.bridgeOffscreenBulletBills = 11;
    difficultySettings.castleFlyingCheepCheeps = 11;
    difficultySettings.castleLakitus = 11;
    difficultySettings.castleOffscreenBulletBills = 11;
    difficultySettings.islandFlyingCheepCheeps = 11;
    difficultySettings.islandLakitus = 11;
    difficultySettings.islandOffscreenBulletBills = 11;
    difficultySettings.undergroundFlyingCheepCheeps = 11;
    difficultySettings.undergroundLakitus = 11;
    difficultySettings.undergroundOffscreenBulletBills = 11;
    difficultySettings.underwaterFlyingCheepCheeps = 11;
    difficultySettings.underwaterLakitus = 11;
    difficultySettings.underwaterSwimmingCheepCheeps = 11;
    difficultySettings.standardOverworldFlyingCheepCheeps = 11;
    difficultySettings.standardOverworldLakitus = 11;
    difficultySettings.standardOverworldOffscreenBulletBills = 11;
    return difficultySettings;
}

Difficulty_Settings Difficulty_Configurations::Easy() {
    Difficulty_Settings difficultySettings = this->Normal();
    difficultySettings.bulletTime = difficultySettings.hammerTime; //activate bullet time where hammer time would be activated
    difficultySettings.hammerTime = 11; //disable hammer time
    return difficultySettings;
}

Difficulty_Settings Difficulty_Configurations::Hard() {
    Difficulty_Settings difficultySettings = this->Normal();
    difficultySettings.bulletTime = 1;
    difficultySettings.hammerTime = 1;
    difficultySettings.bridgeFlyingCheepCheeps = 1;
    difficultySettings.islandOffscreenBulletBills = 1;
    difficultySettings.standardOverworldLakitus = 1;
    difficultySettings.hammerTimeIntensity = 25;
    return difficultySettings;
}

void Difficulty_Configurations::Very_Easy(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Very_Easy(), pluginSettings);
}

void Difficulty_Configurations::Easy(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Easy(), pluginSettings);
}

void Difficulty_Configurations::Normal(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Normal(), pluginSettings);
}

void Difficulty_Configurations::Hard(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Hard(), pluginSettings);
}
