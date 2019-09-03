#include "Difficulty_Level_Configurations.h"
#include "../../Common_Files/Random.h"
#include "../SMB1_Compliance_Generator/Difficulty.h"
#include <assert.h>

//Wrappers
void Difficulty_Level_Configurations::Very_Easy(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Very_Easy(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Easy(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Easy(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Normal(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Normal(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Hard(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Hard(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Very_Hard(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Very_Hard(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Brutal(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Brutal(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Purist(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Purist(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Purist_And_Auto_Scroll(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Purist_And_Auto_Scroll(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Purist_And_No_Hammer_Bros(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Purist_And_No_Hammer_Bros(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Purist_And_Lakitu_Throw_Arc(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Purist_And_Lakitu_Throw_Arc(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Purist_And_Lakitu_Throw_Arc_And_Auto_Scroll(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Purist_And_Lakitu_Throw_Arc_And_Auto_Scroll(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Purist_And_Lakitu_Throw_Arc_And_No_Hammer_Bros(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Purist_And_Lakitu_Throw_Arc_And_No_Hammer_Bros(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Walk_In_The_Park(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Walk_In_The_Park(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Just_Keep_Scrolling(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Just_Keep_Scrolling(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Riddled_With_Bullets(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Riddled_With_Bullets(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Lakitus_Challenge(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Lakitus_Challenge(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Cheep_Cheep_Frenzy(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Cheep_Cheep_Frenzy(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Hammer_Time(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Hammer_Time(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Extreme_Hammer_Time(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Extreme_Hammer_Time(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Turn_Up_The_Heat(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Turn_Up_The_Heat(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Too_Hot_To_Handle(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Too_Hot_To_Handle(), pluginSettings, disableASM);}
void Difficulty_Level_Configurations::Random(Plugin_Settings *pluginSettings, bool disableASM) {this->Apply_Difficulty_Settings_To_Plugin_Settings(this->Random(), pluginSettings, disableASM);}

void Difficulty_Level_Configurations::Apply_Difficulty_Settings_To_Plugin_Settings(const Difficulty_Level_Settings &difficultySettings, Plugin_Settings *pluginSettings, bool disableASM) {
    assert(pluginSettings);
    pluginSettings->difficultyAutoScroll = difficultySettings.autoScroll;
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
    pluginSettings->difficultyMinimumEnemyDistance = difficultySettings.minimumEnemyDistance;
    pluginSettings->difficultyMinimumUnderwaterEnemyDistance = difficultySettings.minimumUnderwaterEnemyDistance;
    pluginSettings->difficultyNoEnemies = difficultySettings.noEnemies;
    pluginSettings->difficultyUnlimitedTime = difficultySettings.unlimitedTime;
    pluginSettings->difficultyHammerTimeIntensity = difficultySettings.hammerTimeIntensity;
    pluginSettings->difficultyReplaceCastleLoops = difficultySettings.replaceCastleLoops;
    pluginSettings->difficultyReplaceCastleLoopsCurrent = difficultySettings.replaceCastleLoops;
    pluginSettings->difficultyMaxLevelLength = difficultySettings.maxLevelLength;
    pluginSettings->difficultyMaxLevelLengthBlocks = difficultySettings.maxLevelLength;
    pluginSettings->difficultyAutoScrollChancePerLevel = difficultySettings.autoScrollChancePerLevel;
    pluginSettings->difficultyLakituSpawnChancePerLevel = difficultySettings.lakituSpawnChancePerLevel;
    pluginSettings->difficultyLakituRespawnSpeed = difficultySettings.lakituRespawnSpeed;
    pluginSettings->lakituThrowArc = difficultySettings.lakituThrowArc;
    pluginSettings->difficultySpinyEggBehavior = difficultySettings.spinyEggBehavior;
    pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns = difficultySettings.disableAllOtherEnemiesWhenALakituSpawns;
    pluginSettings->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = difficultySettings.disableAllOtherEnemiesWhenFlyingCheepCheepsSpawn;
    pluginSettings->difficultySpawnerPriority = difficultySettings.spawnerPriority;
    pluginSettings->superMarioOnDamage = difficultySettings.superMarioOnDamage;
    pluginSettings->difficultySurfingLiftSpeed = difficultySettings.surfingLiftSpeed;
    pluginSettings->difficultyPiranhaPlantType = difficultySettings.piranhaPlantType;
    pluginSettings->difficultyMaxNumberOfPiranhaPlants = difficultySettings.maxNumberOfPiranhaPlants;
    pluginSettings->difficultyBasicEnemySpeed = difficultySettings.basicEnemySpeed;
    pluginSettings->difficultyBowserFlameFrequency = difficultySettings.bowserFlameFrequency;
    pluginSettings->difficultyBulletBillFiringRate = difficultySettings.bulletBillFiringRate;
    pluginSettings->difficultyBulletBillSpeed = difficultySettings.bulletBillSpeed;
    pluginSettings->difficultyEnemyGroupSize = difficultySettings.enemyGroupSize;
    pluginSettings->difficultyEnemyRevivalSpeed = difficultySettings.enemyRevivalSpeed;
    pluginSettings->difficultyFlyingCheepCheepJumpHeight = difficultySettings.flyingCheepCheepJumpHeight;
    pluginSettings->difficultyHammerBrosThrowRate = difficultySettings.hammerBrosThrowRate;
    pluginSettings->difficultySpeedyObjectsAndEnemies = difficultySettings.speedyObjectsAndEnemies;
    pluginSettings->difficultyPiranhaPlantsOnFirstLevel = difficultySettings.piranhaPlantsOnFirstLevel;
    if (disableASM) this->Disable_All_ASM_Hacks(pluginSettings);
}

bool Difficulty_Level_Configurations::Update_Plugin_Settings_For_Difficulty_Present(int difficulty, Plugin_Settings *pluginSettings, bool partialSupport) {
    switch (difficulty) {
    default: return false; //unknown difficulty present
    case 0:  return true; //nothing to do for custom difficulties here
    case 1:  return true; //nothing to do for random difficulties here
    case 2:  Difficulty_Level_Configurations().Very_Easy(pluginSettings, partialSupport); return true;
    case 3:  Difficulty_Level_Configurations().Easy(pluginSettings, partialSupport); return true;
    case 4:  Difficulty_Level_Configurations().Normal(pluginSettings, partialSupport); return true;
    case 5:  Difficulty_Level_Configurations().Hard(pluginSettings, partialSupport); return true;
    case 6:  Difficulty_Level_Configurations().Very_Hard(pluginSettings, partialSupport); return true;
    case 7:  Difficulty_Level_Configurations().Brutal(pluginSettings, partialSupport); return true;
    case 8:  Difficulty_Level_Configurations().Purist(pluginSettings, partialSupport); return true;
    case 9:  Difficulty_Level_Configurations().Purist_And_Auto_Scroll(pluginSettings, partialSupport); return true;
    case 10: Difficulty_Level_Configurations().Purist_And_No_Hammer_Bros(pluginSettings, partialSupport); return true;
    case 11: Difficulty_Level_Configurations().Purist_And_Lakitu_Throw_Arc(pluginSettings, partialSupport); return true;
    case 12: Difficulty_Level_Configurations().Purist_And_Lakitu_Throw_Arc_And_Auto_Scroll(pluginSettings, partialSupport); return true;
    case 13: Difficulty_Level_Configurations().Purist_And_Lakitu_Throw_Arc_And_No_Hammer_Bros(pluginSettings, partialSupport); return true;
    case 14: Difficulty_Level_Configurations().Walk_In_The_Park(pluginSettings, partialSupport); return true;
    case 15: Difficulty_Level_Configurations().Just_Keep_Scrolling(pluginSettings, partialSupport); return true;
    case 16: Difficulty_Level_Configurations().Riddled_With_Bullets(pluginSettings, partialSupport); return true;
    case 17: Difficulty_Level_Configurations().Lakitus_Challenge(pluginSettings, partialSupport); return true;
    case 18: Difficulty_Level_Configurations().Cheep_Cheep_Frenzy(pluginSettings, partialSupport); return true;
    case 19: Difficulty_Level_Configurations().Hammer_Time(pluginSettings, partialSupport); return true;
    case 20: Difficulty_Level_Configurations().Extreme_Hammer_Time(pluginSettings, partialSupport); return true;
    case 21: Difficulty_Level_Configurations().Turn_Up_The_Heat(pluginSettings, partialSupport); return true;
    case 22: Difficulty_Level_Configurations().Too_Hot_To_Handle(pluginSettings, partialSupport); return true;
    }
}

void Difficulty_Level_Configurations::Disable_All_ASM_Hacks(Plugin_Settings *pluginSettings) {
    pluginSettings->difficultyAutoScroll = 11;
    pluginSettings->difficultyWalkingHammerBros = 11;
    pluginSettings->difficultyAutoScrollChancePerLevel = 0;
    pluginSettings->difficultyBasicEnemySpeed = 1;
    pluginSettings->difficultyBowserFlameFrequency = 2;
    pluginSettings->difficultyBulletBillFiringRate = 3;
    pluginSettings->difficultyBulletBillSpeed = 2;
    pluginSettings->difficultyEnemyGroupSize = 2; //Normal
    pluginSettings->difficultyEnemyRevivalSpeed = 2; //Normal
    pluginSettings->difficultyFlyingCheepCheepJumpHeight = 2;
    pluginSettings->difficultyHammerBrosThrowRate = 4; //Normal
    pluginSettings->difficultyLakituRespawnSpeed = 3;
    pluginSettings->lakituThrowArc = false;
    pluginSettings->difficultySurfingLiftSpeed = 3; //Normal
    pluginSettings->difficultyPiranhaPlantType = 1;
    pluginSettings->difficultyMaxNumberOfPiranhaPlants = 5; //Normal
    pluginSettings->difficultySpinyEggBehavior = 1;
    pluginSettings->difficultyReplaceCastleLoops = 2;
    pluginSettings->difficultyReplaceCastleLoopsCurrent = 2; //none
    pluginSettings->difficultySpeedyObjectsAndEnemies = false;
    pluginSettings->difficultyPiranhaPlantsOnFirstLevel = false;
    pluginSettings->superMarioOnDamage = false;
    pluginSettings->difficultyUnlimitedTime = false;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Normal() {
    Difficulty_Level_Settings difficultySettings;
    difficultySettings.autoScroll = 4;
    difficultySettings.bulletTime = 7;
    difficultySettings.hammerTime = 9;
    difficultySettings.walkingHammerBros = 11;
    difficultySettings.minimumEnemyDistance = 6;
    difficultySettings.minimumUnderwaterEnemyDistance = 8;
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
    difficultySettings.unlimitedTime = false;
    difficultySettings.hammerTimeIntensity = 10;
    difficultySettings.replaceCastleLoops = 3; //autoscroll
    difficultySettings.maxLevelLength = 4; //Normal
    difficultySettings.autoScrollChancePerLevel = 25;
    difficultySettings.lakituSpawnChancePerLevel = 35;
    difficultySettings.lakituRespawnSpeed = 4;
    difficultySettings.lakituThrowArc = true;
    difficultySettings.spinyEggBehavior = 2; //Random Easy
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = true;
    difficultySettings.disableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = false;
    difficultySettings.spawnerPriority = 1;
    difficultySettings.superMarioOnDamage = true;
    difficultySettings.surfingLiftSpeed = 3; //Normal
    difficultySettings.piranhaPlantType = 1;
    difficultySettings.maxNumberOfPiranhaPlants = 5; //Normal
    difficultySettings.basicEnemySpeed = 3; //Normal
    difficultySettings.bowserFlameFrequency = 2; //Normal
    difficultySettings.bulletBillFiringRate = 3; //Normal
    difficultySettings.bulletBillSpeed = 2;
    difficultySettings.enemyGroupSize = 2; //Normal
    difficultySettings.enemyRevivalSpeed = 2; //Normal
    difficultySettings.flyingCheepCheepJumpHeight = 1; //Low
    difficultySettings.hammerBrosThrowRate = 3; //Infrequent
    difficultySettings.speedyObjectsAndEnemies = false;
    difficultySettings.piranhaPlantsOnFirstLevel = false;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Very_Easy() {
    //Turn Everything Off for Very Easy
    Difficulty_Level_Settings difficultySettings = this->Easy();
    difficultySettings.autoScroll = 11;
    difficultySettings.autoScrollChancePerLevel = 0;
    difficultySettings.bulletTime = 11;
    difficultySettings.minimumEnemyDistance = 10;
    difficultySettings.minimumUnderwaterEnemyDistance = 11;
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
    difficultySettings.hammerBrosThrowRate = 1; //Extremely Infrequent
    difficultySettings.surfingLiftSpeed = 1; //Very Slow
    difficultySettings.maxNumberOfPiranhaPlants = 1;
    difficultySettings.lakituRespawnSpeed = 2;
    difficultySettings.basicEnemySpeed = 1; //Very Slow
    difficultySettings.bowserFlameFrequency = 1; //infrequent
    difficultySettings.bulletBillFiringRate = 2; //infrequent
    difficultySettings.bulletBillSpeed = 1;
    difficultySettings.enemyGroupSize = 1; //Small
    difficultySettings.unlimitedTime = true;
    difficultySettings.replaceCastleLoops = 7; //Top of Flagpole gives 1-UP + Start with Fire Flower
    difficultySettings.maxLevelLength = 2; //Very Short
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Easy() {
    Difficulty_Level_Settings difficultySettings = this->Normal();
    difficultySettings.autoScroll = 7;
    difficultySettings.bulletTime = 9;
    difficultySettings.hammerTime = 11; //disable hammer time
    difficultySettings.minimumEnemyDistance = 8;
    difficultySettings.minimumUnderwaterEnemyDistance = 9;
    difficultySettings.bridgeFlyingCheepCheeps = 5;
    difficultySettings.hammerBrosThrowRate = 2; //Very Infrequent
    difficultySettings.surfingLiftSpeed = 2; //Slow
    difficultySettings.maxNumberOfPiranhaPlants = 3;
    difficultySettings.basicEnemySpeed = 2; //Slow
    difficultySettings.lakituThrowArc = false;
    difficultySettings.disableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = true;
    difficultySettings.spinyEggBehavior = 2; //Random Easy
    difficultySettings.lakituRespawnSpeed = 3;
    difficultySettings.spawnerPriority = 2;
    difficultySettings.replaceCastleLoops = 5; //Top of Flagpole gives 1-UP
    difficultySettings.maxLevelLength = 3; //Short
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Hard() {
    Difficulty_Level_Settings difficultySettings = this->Normal();
    difficultySettings.autoScroll = 3;
    difficultySettings.bulletTime = 5;
    difficultySettings.hammerTime = 7;
    difficultySettings.buzzyBeetlesReplaceLoneGoombas = 4;
    difficultySettings.walkingHammerBros = 11;
    difficultySettings.minimumEnemyDistance = 5;
    difficultySettings.minimumUnderwaterEnemyDistance = 6;
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
    difficultySettings.spinyEggBehavior = 1; //Random (No Bounce)
    difficultySettings.replaceCastleLoops = 1; //random hard
    difficultySettings.maxLevelLength = 5; //Long
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = false;
    difficultySettings.disableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = false;
    difficultySettings.superMarioOnDamage = false;
    difficultySettings.surfingLiftSpeed = 4; //Fast
    difficultySettings.piranhaPlantType = 2; //red
    difficultySettings.basicEnemySpeed = 4; //Fast
    difficultySettings.bulletBillSpeed = 3; //fast
    difficultySettings.enemyRevivalSpeed = 2; //Normal
    difficultySettings.flyingCheepCheepJumpHeight = 2; //Normal
    difficultySettings.hammerBrosThrowRate = 4; //Normal
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Very_Hard() {
    Difficulty_Level_Settings difficultySettings = this->Hard();
    difficultySettings.bulletTime = 2;
    difficultySettings.hammerTime = 3;
    difficultySettings.hammerTimeIntensity = 20;
    difficultySettings.walkingHammerBros = 8;
    difficultySettings.minimumEnemyDistance = 4;
    difficultySettings.minimumUnderwaterEnemyDistance = 5;
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = false;
    difficultySettings.disableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = false;
    difficultySettings.autoScrollChancePerLevel = 35;
    difficultySettings.lakituSpawnChancePerLevel = 50;
    difficultySettings.castleFlyingCheepCheeps = 7;
    difficultySettings.castleLakitus = 5;
    difficultySettings.castleOffscreenBulletBills = 2;
    difficultySettings.standardOverworldLakitus = 3;
    difficultySettings.undergroundLakitus = 3;
    difficultySettings.underwaterLakitus = 3;
    difficultySettings.surfingLiftSpeed = 5; //Very Fast
    difficultySettings.piranhaPlantType = 3; //black
    difficultySettings.spinyEggBehavior = 3; //bounce
    difficultySettings.maxLevelLength = 6; //Very Long
    difficultySettings.lakituRespawnSpeed = 5;
    difficultySettings.speedyObjectsAndEnemies = true;
    difficultySettings.basicEnemySpeed = 6; //Speedy
    difficultySettings.bulletBillFiringRate = 4; //frequent
    difficultySettings.bulletBillSpeed = 4; //speedy
    difficultySettings.hammerBrosThrowRate = 5; //Frequent
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Brutal() {
    Difficulty_Level_Settings difficultySettings = this->Very_Hard();
    difficultySettings.hammerTimeIntensity = 40;
    difficultySettings.walkingHammerBros = 6;
    difficultySettings.autoScroll = 2;
    difficultySettings.minimumEnemyDistance = 1;
    difficultySettings.minimumUnderwaterEnemyDistance = 1;
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
    difficultySettings.surfingLiftSpeed = 6; //ludicrous
    difficultySettings.piranhaPlantType = 3; //black
    difficultySettings.maxLevelLength = 7; //Maximum
    difficultySettings.hammerBrosThrowRate = 8; //ludicrous
    difficultySettings.lakituRespawnSpeed = 6;
    difficultySettings.basicEnemySpeed = 7; //ludicrous
    difficultySettings.bowserFlameFrequency = 2; //frequent
    difficultySettings.bulletBillFiringRate = 7; //ludicrous
    difficultySettings.bulletBillSpeed = 5; //ludicrous
    difficultySettings.enemyGroupSize = 3; //Big
    difficultySettings.enemyRevivalSpeed = 4; //Goombas respawn + Quick
    difficultySettings.spawnerPriority = 0; //random
    difficultySettings.piranhaPlantsOnFirstLevel = true;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Purist() {
    Difficulty_Level_Settings difficultySettings = this->Purist_And_Auto_Scroll();
    difficultySettings.replaceCastleLoops = 2; //nothing
    difficultySettings.autoScroll = 11;
    difficultySettings.autoScrollChancePerLevel = 0;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Purist_And_Auto_Scroll() {
    Difficulty_Level_Settings difficultySettings = this->Normal();
    difficultySettings.buzzyBeetlesReplaceLoneGoombas = 7;
    difficultySettings.walkingHammerBros = 11;
    difficultySettings.replaceCastleLoops = 3; //auto scroll
    difficultySettings.piranhaPlantType = 1; //green
    difficultySettings.lakituRespawnSpeed = 3; //normal
    difficultySettings.lakituThrowArc = false;
    difficultySettings.superMarioOnDamage = false;
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = true;
    difficultySettings.disableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = true;
    difficultySettings.spinyEggBehavior = 8; //normal
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Purist_And_No_Hammer_Bros() {
    Difficulty_Level_Settings difficultySettings = this->Purist();
    difficultySettings.hammerTime = 11;
    difficultySettings.walkingHammerBros = 11;
    difficultySettings.hammerTimeIntensity = 0;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Purist_And_Lakitu_Throw_Arc() {
    Difficulty_Level_Settings difficultySettings = this->Purist();
    difficultySettings.lakituThrowArc = true;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Purist_And_Lakitu_Throw_Arc_And_Auto_Scroll() {
    Difficulty_Level_Settings difficultySettings = this->Purist_And_Auto_Scroll();
    difficultySettings.lakituThrowArc = true;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Purist_And_Lakitu_Throw_Arc_And_No_Hammer_Bros() {
    Difficulty_Level_Settings difficultySettings = this->Purist_And_No_Hammer_Bros();
    difficultySettings.lakituThrowArc = true;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Walk_In_The_Park() {
    Difficulty_Level_Settings difficultySettings = this->Very_Easy();
    difficultySettings.noEnemies = true;
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

Difficulty_Level_Settings Difficulty_Level_Configurations::Just_Keep_Scrolling() {
    Difficulty_Level_Settings difficultySettings = this->Normal();
    difficultySettings.autoScroll = 1;
    difficultySettings.autoScrollChancePerLevel = 100;
    difficultySettings.replaceCastleLoops = 3;
    difficultySettings.unlimitedTime = true;
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
    difficultySettings.bulletBillFiringRate = 5; //very frequent
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
    difficultySettings.disableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = false;
    difficultySettings.spinyEggBehavior = 0;
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
    difficultySettings.replaceCastleLoops = 2;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Extreme_Hammer_Time() {
    Difficulty_Level_Settings difficultySettings = this->Very_Hard();
    difficultySettings.hammerTime = 1;
    difficultySettings.hammerTimeIntensity = 100;
    difficultySettings.replaceCastleLoops = 2;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Turn_Up_The_Heat() {
    Difficulty_Level_Settings difficultySettings = this->Hammer_Time();
    difficultySettings.replaceCastleLoops = 4;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Too_Hot_To_Handle() {
    Difficulty_Level_Settings difficultySettings = this->Extreme_Hammer_Time();
    difficultySettings.replaceCastleLoops = 4;
    return difficultySettings;
}

Difficulty_Level_Settings Difficulty_Level_Configurations::Random() {
    Difficulty_Level_Settings difficultySettings = this->Normal();
    int randRange = Difficulty::DIFFICULTY_MAX-Difficulty::DIFFICULTY_MIN;
    difficultySettings.bulletTime = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.hammerTime = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.walkingHammerBros = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.buzzyBeetlesReplaceLoneGoombas = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.bridgeFlyingCheepCheeps = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.bridgeLakitus = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.bridgeOffscreenBulletBills = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.castleFireBars = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.castleFlyingCheepCheeps = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.castleLakitus = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.castleOffscreenBulletBills = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.islandFlyingCheepCheeps = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.islandLakitus = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.islandOffscreenBulletBills = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.undergroundFlyingCheepCheeps = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.undergroundLakitus = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.undergroundOffscreenBulletBills = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.underwaterBloopers = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.underwaterFlyingCheepCheeps = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.underwaterLakitus = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.underwaterSwimmingCheepCheeps = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.standardOverworldFlyingCheepCheeps = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.standardOverworldLakitus = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.standardOverworldOffscreenBulletBills = Random::Get_Instance().Get_Num(randRange)+Difficulty::DIFFICULTY_MIN+1;
    difficultySettings.minimumEnemyDistance = Random::Get_Instance().Get_Num(1, 11);
    difficultySettings.minimumUnderwaterEnemyDistance = Random::Get_Instance().Get_Num(1, 11);
    difficultySettings.hammerTimeIntensity = Random::Get_Instance().Get_Num(50); //limit to 50%
    difficultySettings.replaceCastleLoops = 0; //0 is random here
    difficultySettings.maxLevelLength = 0; //0 is random here
    difficultySettings.autoScrollChancePerLevel = Random::Get_Instance().Get_Num(100);
    difficultySettings.lakituSpawnChancePerLevel = Random::Get_Instance().Get_Num(100);
    difficultySettings.lakituThrowArc = static_cast<bool>(Random::Get_Instance().Get_Num(1));
    difficultySettings.spinyEggBehavior = 0; //0 is random here
    difficultySettings.lakituRespawnSpeed = 0; //0 is random here
    difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = static_cast<bool>(Random::Get_Instance().Get_Num(1));
    difficultySettings.disableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = static_cast<bool>(Random::Get_Instance().Get_Num(1));
    difficultySettings.spawnerPriority = 0; //0 is random here
    difficultySettings.superMarioOnDamage = static_cast<bool>(Random::Get_Instance().Get_Num(1));
    difficultySettings.surfingLiftSpeed = 0; //0 is random here
    difficultySettings.piranhaPlantType = 0; //0 is random here
    difficultySettings.maxNumberOfPiranhaPlants = 0; //0 is random here
    difficultySettings.basicEnemySpeed = 0; //0 is random here
    difficultySettings.bowserFlameFrequency = 0; //0 is random here
    difficultySettings.bulletBillFiringRate = 0; //0 is random here
    difficultySettings.bulletBillSpeed = 0; //0 is random here
    difficultySettings.enemyGroupSize = 0; //0 is random here
    difficultySettings.enemyRevivalSpeed = 0; //0 is random here
    difficultySettings.flyingCheepCheepJumpHeight = 0; //0 is random here
    difficultySettings.hammerBrosThrowRate = 0; //0 is random here
    difficultySettings.speedyObjectsAndEnemies = static_cast<bool>(Random::Get_Instance().Get_Num(1));
    difficultySettings.piranhaPlantsOnFirstLevel = static_cast<bool>(Random::Get_Instance().Get_Num(1));
    return difficultySettings;
}
