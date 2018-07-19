#include "Difficulty_Level_Configurations.h"
#include "../../Common_Files/Random.h"
#include "../SMB1_Compliance_Generator/Difficulty.h"
#include <assert.h>

void Difficulty_Level_Configurations::Apply_Difficulty_Settings_To_Plugin_Settings(const Difficulty_Level_Settings &difficultySettings, Plugin_Settings *pluginSettings) {
    assert(pluginSettings);
    pluginSettings->difficultyBulletTime = difficultySettings.bulletTime;
    pluginSettings->difficultyHammerTime = difficultySettings.hammerTime;
    pluginSettings->difficultyWalkingHammerBros = difficultySettings.walkingHammerBros;
    pluginSettings->difficultyBuzzyBeetlesReplaceLoneGoombas = difficultySettings.buzzyBeetlesReplaceLoneGoombas;
    pluginSettings->difficultyBridgeFlyingCheepCheeps = difficultySettings.bridgeFlyingCheepCheeps;
    pluginSettings->difficultyBridgeLakitus = difficultySettings.bridgeLakitus;
    pluginSettings->difficultyBridgeOffscreenBulletBills = difficultySettings.bridgeOffscreenBulletBills;
    pluginSettings->difficultyCastleFireBars = difficultySettings.castleFireBars;
    pluginSettings->difficultyCastleFlyingCheepCheeps = difficultySettings.castleFlyingCheepCheeps;
    pluginSettings->difficultyCastleLakitus = difficultySettings.castleLakitus;
    pluginSettings->difficultyCastleOffscreenBulletBills = difficultySettings.castleOffscreenBulletBills;
    pluginSettings->difficultyIslandFlyingCheepCheeps = difficultySettings.islandFlyingCheepCheeps;
    pluginSettings->difficultyIslandLakitus = difficultySettings.islandLakitus;
    pluginSettings->difficultyIslandOffscreenBulletBills = difficultySettings.islandOffscreenBulletBills;
    pluginSettings->difficultyUndergroundFlyingCheepCheeps = difficultySettings.undergroundFlyingCheepCheeps;
    pluginSettings->difficultyUndergroundLakitus = difficultySettings.undergroundLakitus;
    pluginSettings->difficultyUndergroundOffscreenBulletBills = difficultySettings.undergroundOffscreenBulletBills;
    pluginSettings->difficultyUnderwaterBloopers = difficultySettings.underwaterBloopers;
    pluginSettings->difficultyUnderwaterFlyingCheepCheeps = difficultySettings.underwaterFlyingCheepCheeps;
    pluginSettings->difficultyUnderwaterLakitus = difficultySettings.underwaterLakitus;
    pluginSettings->difficultyUnderwaterSwimmingCheepCheeps = difficultySettings.underwaterSwimmingCheepCheeps;
    pluginSettings->difficultyStandardOverworldFlyingCheepCheeps = difficultySettings.standardOverworldFlyingCheepCheeps;
    pluginSettings->difficultyStandardOverworldLakitus = difficultySettings.standardOverworldLakitus;
    pluginSettings->difficultyStandardOverworldOffscreenBulletBills = difficultySettings.standardOverworldOffscreenBulletBills;
    pluginSettings->difficultyNoEnemies = difficultySettings.noEnemies;
    pluginSettings->difficultyStartWithFireFlowerOnRoomChange = difficultySettings.startWithFireFlowerOnRoomChange;
    pluginSettings->difficultyHammerTimeIntensity = difficultySettings.hammerTimeIntensity;
    pluginSettings->difficultyLakituSpawnChancePerLevel = difficultySettings.lakituSpawnChancePerLevel;
    pluginSettings->difficultyLakituRespawnSpeed = difficultySettings.lakituRespawnSpeed;
    pluginSettings->lakituThrowArc = difficultySettings.lakituThrowArc;
    pluginSettings->difficultySpinyEggBehavior = difficultySettings.spinyEggBehavior;
    pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns = difficultySettings.disableAllOtherEnemiesWhenALakituSpawns;
    pluginSettings->difficultySpawnerPriority = difficultySettings.spawnerPriority;
    pluginSettings->superMarioOnDamage = difficultySettings.superMarioOnDamage;
    pluginSettings->difficultyPiranhaPlantType = difficultySettings.piranhaPlantType;
    pluginSettings->difficultyBasicEnemySpeed = difficultySettings.basicEnemySpeed;
    pluginSettings->difficultyBulletBillSpeed = difficultySettings.bulletBillSpeed;
    pluginSettings->difficultySpeedyObjectsAndEnemies = difficultySettings.speedyObjectsAndEnemies;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Normal() {
    Difficulty_Level_Settings difficultySettings;
    difficultySettings.bulletTime = 7;
    difficultySettings.hammerTime = 9;
    difficultySettings.walkingHammerBros = 11;
    difficultySettings.buzzyBeetlesReplaceLoneGoombas = 6;
    difficultySettings.bridgeFlyingCheepCheeps = 3;
    difficultySettings.bridgeLakitus = 4;
    difficultySettings.bridgeOffscreenBulletBills = 11;
    difficultySettings.castleFireBars = 3;
    difficultySettings.castleFlyingCheepCheeps = 11;
    difficultySettings.castleLakitus = 11;
    difficultySettings.castleOffscreenBulletBills = 11;
    difficultySettings.islandFlyingCheepCheeps = 11;
    difficultySettings.islandLakitus = 4;
    difficultySettings.islandOffscreenBulletBills = 6;
    difficultySettings.undergroundFlyingCheepCheeps = 11;
    difficultySettings.undergroundLakitus = 11;
    difficultySettings.undergroundOffscreenBulletBills = 11;
    difficultySettings.underwaterBloopers = 4;
    difficultySettings.underwaterFlyingCheepCheeps = 11;
    difficultySettings.underwaterLakitus = 3;
    difficultySettings.underwaterSwimmingCheepCheeps = 1;
    difficultySettings.standardOverworldFlyingCheepCheeps = 11;
    difficultySettings.standardOverworldLakitus = 4;
    difficultySettings.standardOverworldOffscreenBulletBills = 11;
    difficultySettings.noEnemies = false;
    difficultySettings.startWithFireFlowerOnRoomChange = false;
    difficultySettings.hammerTimeIntensity = 10;
    difficultySettings.lakituSpawnChancePerLevel = 35;
    difficultySettings.lakituRespawnSpeed = 4;
    difficultySettings.lakituThrowArc = true;
    difficultySettings.spinyEggBehavior = 0;
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = true;
    difficultySettings.spawnerPriority = 1;
    difficultySettings.superMarioOnDamage = true;
    difficultySettings.piranhaPlantType = 1;
    difficultySettings.basicEnemySpeed = 1;
    difficultySettings.bulletBillSpeed = 2;
    difficultySettings.speedyObjectsAndEnemies = false;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Very_Easy() {
    //Turn Everything Off for Very Easy
    Difficulty_Level_Settings difficultySettings = this->Easy();
    difficultySettings.bulletTime = 11;
    difficultySettings.buzzyBeetlesReplaceLoneGoombas = 10;
    difficultySettings.bridgeFlyingCheepCheeps = 11;
    difficultySettings.bridgeLakitus = 8;
    difficultySettings.bridgeOffscreenBulletBills = 8;
    difficultySettings.castleFireBars = 10;
    difficultySettings.islandLakitus = 11;
    difficultySettings.islandOffscreenBulletBills = 8;
    difficultySettings.underwaterBloopers = 9;
    difficultySettings.underwaterLakitus = 5;
    difficultySettings.underwaterSwimmingCheepCheeps = 2;
    difficultySettings.standardOverworldLakitus = 8;
    difficultySettings.lakituRespawnSpeed = 2;
    difficultySettings.bulletBillSpeed = 1;
    difficultySettings.startWithFireFlowerOnRoomChange = true;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Easy() {
    Difficulty_Level_Settings difficultySettings = this->Normal();
    difficultySettings.bulletTime = 9;
    difficultySettings.hammerTime = 11; //disable hammer time
    difficultySettings.lakituThrowArc = false;
    difficultySettings.lakituRespawnSpeed = 3;
    difficultySettings.spawnerPriority = 2;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Hard() {
    Difficulty_Level_Settings difficultySettings = this->Normal();
    difficultySettings.bulletTime = 5;
    difficultySettings.hammerTime = 7;
    difficultySettings.buzzyBeetlesReplaceLoneGoombas = 4;
    difficultySettings.walkingHammerBros = 11;
    difficultySettings.bridgeFlyingCheepCheeps = 2;
    difficultySettings.bridgeLakitus = 11;
    difficultySettings.bridgeOffscreenBulletBills = 11;
    difficultySettings.castleFireBars = 1;
    difficultySettings.castleFlyingCheepCheeps = 10;
    difficultySettings.castleLakitus = 10;
    difficultySettings.castleOffscreenBulletBills = 11;
    difficultySettings.islandFlyingCheepCheeps = 8;
    difficultySettings.islandLakitus = 6;
    difficultySettings.islandOffscreenBulletBills = 3;
    difficultySettings.undergroundFlyingCheepCheeps = 11;
    difficultySettings.undergroundLakitus = 6;
    difficultySettings.undergroundOffscreenBulletBills = 8;
    difficultySettings.underwaterBloopers = 2;
    difficultySettings.underwaterFlyingCheepCheeps = 11;
    difficultySettings.underwaterLakitus = 6;
    difficultySettings.underwaterSwimmingCheepCheeps = 1;
    difficultySettings.standardOverworldFlyingCheepCheeps = 11;
    difficultySettings.standardOverworldLakitus = 3;
    difficultySettings.standardOverworldOffscreenBulletBills = 11;
    difficultySettings.hammerTimeIntensity = 20;
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = false;
    difficultySettings.superMarioOnDamage = false;
    difficultySettings.piranhaPlantType = 2; //red
    difficultySettings.lakituRespawnSpeed = 5;
    difficultySettings.basicEnemySpeed = 2; //fast
    difficultySettings.bulletBillSpeed = 3; //fast
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Very_Hard() {
    Difficulty_Level_Settings difficultySettings = this->Hard();
    difficultySettings.bulletTime = 2;
    difficultySettings.hammerTime = 3;
    difficultySettings.hammerTimeIntensity = 20;
    difficultySettings.walkingHammerBros = 8;
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = false;
    difficultySettings.lakituSpawnChancePerLevel = 50;
    difficultySettings.castleFlyingCheepCheeps = 7;
    difficultySettings.castleLakitus = 5;
    difficultySettings.castleOffscreenBulletBills = 2;
    difficultySettings.standardOverworldLakitus = 3;
    difficultySettings.undergroundLakitus = 3;
    difficultySettings.underwaterLakitus = 3;
    difficultySettings.piranhaPlantType = 3; //black
    difficultySettings.lakituRespawnSpeed = 6;
    difficultySettings.speedyObjectsAndEnemies = true;
    difficultySettings.basicEnemySpeed = 3; //speedy
    difficultySettings.bulletBillSpeed = 4; //speedy
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Brutal() {
    Difficulty_Level_Settings difficultySettings = this->Very_Hard();
    difficultySettings.hammerTimeIntensity = 40;
    difficultySettings.walkingHammerBros = 6;
    difficultySettings.buzzyBeetlesReplaceLoneGoombas = 1;
    difficultySettings.lakituSpawnChancePerLevel = 100;
    difficultySettings.castleFlyingCheepCheeps = 4;
    difficultySettings.castleLakitus = 3;
    difficultySettings.castleOffscreenBulletBills = 5;
    difficultySettings.bridgeFlyingCheepCheeps = 4;
    difficultySettings.bridgeLakitus = 3;
    difficultySettings.bridgeOffscreenBulletBills = 5;
    difficultySettings.islandFlyingCheepCheeps = 4;
    difficultySettings.islandLakitus = 3;
    difficultySettings.islandOffscreenBulletBills = 5;
    difficultySettings.standardOverworldFlyingCheepCheeps = 4;
    difficultySettings.standardOverworldLakitus = 3;
    difficultySettings.standardOverworldOffscreenBulletBills = 5;
    difficultySettings.undergroundFlyingCheepCheeps = 4;
    difficultySettings.undergroundLakitus = 3;
    difficultySettings.undergroundOffscreenBulletBills = 5;
    difficultySettings.underwaterFlyingCheepCheeps = 5;
    difficultySettings.underwaterLakitus = 3;
    difficultySettings.piranhaPlantType = 3; //black
    difficultySettings.basicEnemySpeed = 4; //ludicrous
    difficultySettings.bulletBillSpeed = 5; //ludicrous
    difficultySettings.spawnerPriority = 0; //random
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Walk_In_The_Park() {
    Difficulty_Level_Settings difficultySettings = this->Very_Easy();
    difficultySettings.noEnemies = true;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Riddled_With_Bullets() {
    Difficulty_Level_Settings difficultySettings = this->Hard();
    difficultySettings.bulletTime = 1;
    difficultySettings.bridgeFlyingCheepCheeps = 11;
    difficultySettings.bridgeLakitus = 11;
    difficultySettings.bridgeOffscreenBulletBills = 1;
    difficultySettings.castleFlyingCheepCheeps = 11;
    difficultySettings.castleLakitus = 11;
    difficultySettings.castleOffscreenBulletBills = 1;
    difficultySettings.islandFlyingCheepCheeps = 11;
    difficultySettings.islandLakitus = 11;
    difficultySettings.islandOffscreenBulletBills = 1;
    difficultySettings.undergroundFlyingCheepCheeps = 11;
    difficultySettings.undergroundLakitus = 11;
    difficultySettings.undergroundOffscreenBulletBills = 1;
    difficultySettings.underwaterFlyingCheepCheeps = 11;
    difficultySettings.underwaterLakitus = 11;
    difficultySettings.underwaterSwimmingCheepCheeps = 1;
    difficultySettings.standardOverworldFlyingCheepCheeps = 11;
    difficultySettings.standardOverworldLakitus = 11;
    difficultySettings.standardOverworldOffscreenBulletBills = 1;
    difficultySettings.bulletBillSpeed = 4; //speedy
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Lakitus_Challenge() {
    Difficulty_Level_Settings difficultySettings = this->Hard();
    difficultySettings.bridgeFlyingCheepCheeps = 11;
    difficultySettings.bridgeLakitus = 1;
    difficultySettings.bridgeOffscreenBulletBills = 11;
    difficultySettings.castleFlyingCheepCheeps = 11;
    difficultySettings.castleLakitus = 1;
    difficultySettings.castleOffscreenBulletBills = 11;
    difficultySettings.islandFlyingCheepCheeps = 11;
    difficultySettings.islandLakitus = 1;
    difficultySettings.islandOffscreenBulletBills = 11;
    difficultySettings.undergroundFlyingCheepCheeps = 11;
    difficultySettings.undergroundLakitus = 1;
    difficultySettings.undergroundOffscreenBulletBills = 11;
    difficultySettings.underwaterFlyingCheepCheeps = 11;
    difficultySettings.underwaterLakitus = 1;
    difficultySettings.underwaterSwimmingCheepCheeps = 11;
    difficultySettings.standardOverworldFlyingCheepCheeps = 11;
    difficultySettings.standardOverworldLakitus = 1;
    difficultySettings.standardOverworldOffscreenBulletBills = 11;
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = false;
    difficultySettings.lakituRespawnSpeed = 6;
    difficultySettings.lakituSpawnChancePerLevel = 100;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Cheep_Cheep_Frenzy() {
    Difficulty_Level_Settings difficultySettings = this->Hard();
    difficultySettings.bridgeFlyingCheepCheeps = 1;
    difficultySettings.bridgeLakitus = 11;
    difficultySettings.bridgeOffscreenBulletBills = 11;
    difficultySettings.castleFlyingCheepCheeps = 1;
    difficultySettings.castleLakitus = 11;
    difficultySettings.castleOffscreenBulletBills = 11;
    difficultySettings.islandFlyingCheepCheeps = 1;
    difficultySettings.islandLakitus = 11;
    difficultySettings.islandOffscreenBulletBills = 11;
    difficultySettings.undergroundFlyingCheepCheeps = 1;
    difficultySettings.undergroundLakitus = 11;
    difficultySettings.undergroundOffscreenBulletBills = 11;
    difficultySettings.underwaterFlyingCheepCheeps = 1;
    difficultySettings.underwaterLakitus = 11;
    difficultySettings.underwaterSwimmingCheepCheeps = 11;
    difficultySettings.standardOverworldFlyingCheepCheeps = 1;
    difficultySettings.standardOverworldLakitus = 11;
    difficultySettings.standardOverworldOffscreenBulletBills = 11;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Hammer_Time() {
    Difficulty_Level_Settings difficultySettings = this->Hard();
    difficultySettings.hammerTime = 1;
    difficultySettings.hammerTimeIntensity = 20;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Extreme_Hammer_Time() {
    Difficulty_Level_Settings difficultySettings = this->Hard();
    difficultySettings.hammerTime = 1;
    difficultySettings.hammerTimeIntensity = 100;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Random() {
    Difficulty_Level_Settings difficultySettings = this->Normal();
    int randRange = Difficulty::DIFFICULTY_MAX-Difficulty::DIFFICULTY_MIN;
    difficultySettings.bulletTime = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.hammerTime = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.walkingHammerBros = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.buzzyBeetlesReplaceLoneGoombas = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.bridgeFlyingCheepCheeps = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.bridgeLakitus = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.bridgeOffscreenBulletBills = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.castleFireBars = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.castleFlyingCheepCheeps = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.castleLakitus = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.castleOffscreenBulletBills = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.islandFlyingCheepCheeps = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.islandLakitus = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.islandOffscreenBulletBills = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.undergroundFlyingCheepCheeps = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.undergroundLakitus = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.undergroundOffscreenBulletBills = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.underwaterBloopers = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.underwaterFlyingCheepCheeps = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.underwaterLakitus = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.underwaterSwimmingCheepCheeps = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.standardOverworldFlyingCheepCheeps = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.standardOverworldLakitus = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.standardOverworldOffscreenBulletBills = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.hammerTimeIntensity = Random::Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.lakituSpawnChancePerLevel = Random::Get_Num(100);
    difficultySettings.lakituThrowArc = static_cast<bool>(Random::Get_Num(1));
    difficultySettings.spinyEggBehavior = 0; //0 is random here
    difficultySettings.lakituRespawnSpeed = 0; //0 is random here
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = static_cast<bool>(Random::Get_Num(1));
    difficultySettings.spawnerPriority = 0; //0 is random here
    difficultySettings.superMarioOnDamage = static_cast<bool>(Random::Get_Num(1));
    difficultySettings.piranhaPlantType = 0; //0 is random here
    difficultySettings.basicEnemySpeed = 0; //0 is random here
    difficultySettings.bulletBillSpeed = 0; //0 is random here
    difficultySettings.speedyObjectsAndEnemies = static_cast<bool>(Random::Get_Num(1));
    return difficultySettings;
}

void Difficulty_Level_Configurations::Very_Easy(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Very_Easy(), pluginSettings);
}

void Difficulty_Level_Configurations::Easy(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Easy(), pluginSettings);
}

void Difficulty_Level_Configurations::Normal(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Normal(), pluginSettings);
}

void Difficulty_Level_Configurations::Hard(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Hard(), pluginSettings);
}

void Difficulty_Level_Configurations::Very_Hard(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Very_Hard(), pluginSettings);
}

void Difficulty_Level_Configurations::Brutal(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Brutal(), pluginSettings);
}

void Difficulty_Level_Configurations::Walk_In_The_Park(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Walk_In_The_Park(), pluginSettings);
}

void Difficulty_Level_Configurations::Riddled_With_Bullets(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Riddled_With_Bullets(), pluginSettings);
}

void Difficulty_Level_Configurations::Lakitus_Challenge(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Lakitus_Challenge(), pluginSettings);
}

void Difficulty_Level_Configurations::Cheep_Cheep_Frenzy(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Cheep_Cheep_Frenzy(), pluginSettings);
}

void Difficulty_Level_Configurations::Hammer_Time(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Hammer_Time(), pluginSettings);
}

void Difficulty_Level_Configurations::Extreme_Hammer_Time(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Extreme_Hammer_Time(), pluginSettings);
}

void Difficulty_Level_Configurations::Random(Plugin_Settings *pluginSettings) {
    this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Random(), pluginSettings);
}
