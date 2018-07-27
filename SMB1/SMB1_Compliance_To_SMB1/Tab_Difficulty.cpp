#include "Tab_Difficulty.h"
#include "Configure_Settings_Form.h"
#include "Difficulty_Level_Configurations.h"
#include "ui_Configure_Settings_Form.h"

void Tab_Difficulty::Load_Settings() {
    if (this->pluginSettings->infiniteLives) this->ui->radioInfiniteLives->setChecked(true);
    else if (this->pluginSettings->permadeath) this->ui->radioPermadeath->setChecked(true);
    else this->ui->radioStartingLives->setChecked(true);
    this->ui->sbLives->setValue(this->pluginSettings->numLives);
    this->ui->cbGodMode->setChecked(this->pluginSettings->godMode);
    this->ui->comboTimerSpeed->setCurrentIndex(this->pluginSettings->timerSpeed);
    this->ui->cbRevertToSuperMario->setChecked(this->pluginSettings->superMarioOnDamage);
    this->ui->cbLakituThrowArc->setChecked(this->pluginSettings->lakituThrowArc);
    this->ui->comboBasicEnemySpeed->setCurrentIndex(this->pluginSettings->difficultyBasicEnemySpeed);
    this->ui->comboBulletBillSpeed->setCurrentIndex(this->pluginSettings->difficultyBulletBillSpeed);
    this->ui->cbSpeedyObjectsAndEnemies->setChecked(this->pluginSettings->difficultySpeedyObjectsAndEnemies);
    this->ui->comboDifficulty->setCurrentIndex(this->pluginSettings->difficultyComboIndex);
    this->ui->sbAutoScroll->setValue(this->pluginSettings->difficultyAutoScroll);
    this->ui->sbBulletTime->setValue(this->pluginSettings->difficultyBulletTime);
    this->ui->sbHammerTime->setValue(this->pluginSettings->difficultyHammerTime);
    this->ui->sbWalkingHammerBros->setValue(this->pluginSettings->difficultyWalkingHammerBros);
    this->ui->sbBuzzyBeetleReplaceLoneGoombas->setValue(this->pluginSettings->difficultyBuzzyBeetlesReplaceLoneGoombas);
    this->ui->sbBridgeFlyingCheepCheeps->setValue(this->pluginSettings->difficultyBridgeFlyingCheepCheeps);
    this->ui->sbBridgeLakitus->setValue(this->pluginSettings->difficultyBridgeLakitus);
    this->ui->sbBridgeOffscreenBulletBills->setValue(this->pluginSettings->difficultyBridgeOffscreenBulletBills);
    this->ui->sbCastleFireBars->setValue(this->pluginSettings->difficultyCastleFireBars);
    this->ui->sbCastleFlyingCheepCheeps->setValue(this->pluginSettings->difficultyCastleFlyingCheepCheeps);
    this->ui->sbCastleLakitus->setValue(this->pluginSettings->difficultyCastleLakitus);
    this->ui->sbCastleOffscreenBulletBills->setValue(this->pluginSettings->difficultyCastleOffscreenBulletBills);
    this->ui->sbIslandFlyingCheepCheeps->setValue(this->pluginSettings->difficultyIslandFlyingCheepCheeps);
    this->ui->sbIslandLakitus->setValue(this->pluginSettings->difficultyIslandLakitus);
    this->ui->sbIslandOffscreenBulletBills->setValue(this->pluginSettings->difficultyIslandOffscreenBulletBills);
    this->ui->sbUndergroundFlyingCheepCheeps->setValue(this->pluginSettings->difficultyUndergroundFlyingCheepCheeps);
    this->ui->sbUndergroundLakitus->setValue(this->pluginSettings->difficultyUndergroundLakitus);
    this->ui->sbUndergroundOffscreenBulletBills->setValue(this->pluginSettings->difficultyUndergroundOffscreenBulletBills);
    this->ui->sbUnderwaterBloopers->setValue(this->pluginSettings->difficultyUnderwaterBloopers);
    this->ui->sbUnderwaterFlyingCheepCheeps->setValue(this->pluginSettings->difficultyUnderwaterFlyingCheepCheeps);
    this->ui->sbUnderwaterLakitus->setValue(this->pluginSettings->difficultyUnderwaterLakitus);
    this->ui->sbUnderwaterSwimmingCheepCheeps->setValue(this->pluginSettings->difficultyUnderwaterSwimmingCheepCheeps);
    this->ui->sbStandardOverworldFlyingCheepCheeps->setValue(this->pluginSettings->difficultyStandardOverworldFlyingCheepCheeps);
    this->ui->sbStandardOverworldLakitus->setValue(this->pluginSettings->difficultyStandardOverworldLakitus);
    this->ui->sbStandardOverworldOffscreenBulletBills->setValue(this->pluginSettings->difficultyStandardOverworldOffscreenBulletBills);
    this->ui->cbNoEnemies->setChecked(this->pluginSettings->difficultyNoEnemies);
    this->ui->cbStartWithFireFlowerOnRoomChange->setChecked(this->pluginSettings->difficultyStartWithFireFlowerOnRoomChange);
    this->ui->sbHammerTimeIntensity->setValue(this->pluginSettings->difficultyHammerTimeIntensity);
    this->ui->comboReplaceCastleLoops->setCurrentIndex(this->pluginSettings->difficultyReplaceCastleLoops);
    this->ui->sbAutoScrollChancePerLevel->setValue(this->pluginSettings->difficultyAutoScrollChancePerLevel);
    this->ui->sbLakituSpawnChancePerLevel->setValue(this->pluginSettings->difficultyLakituSpawnChancePerLevel);
    this->ui->comboLakituRespawnSpeed->setCurrentIndex(this->pluginSettings->difficultyLakituRespawnSpeed);
    this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->setChecked(this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns);
    this->ui->comboSpinyEggBehavior->setCurrentIndex(this->pluginSettings->difficultySpinyEggBehavior);
    this->ui->comboSpawnerPriority->setCurrentIndex(this->pluginSettings->difficultySpawnerPriority);
    this->ui->comboPiranhaPlantType->setCurrentIndex(this->pluginSettings->difficultyPiranhaPlantType);
}

void Tab_Difficulty::Save_Settings() {
    this->pluginSettings->infiniteLives = this->ui->radioInfiniteLives->isChecked();
    this->pluginSettings->permadeath = this->ui->radioPermadeath->isChecked();
    this->pluginSettings->numLives = this->ui->sbLives->value();
    this->pluginSettings->godMode = this->ui->cbGodMode->isChecked();
    this->pluginSettings->timerSpeed = this->ui->comboTimerSpeed->currentIndex();

    //Save the Difficulty Settings
    this->pluginSettings->difficultyComboIndex = this->ui->comboDifficulty->currentIndex();
    switch (this->pluginSettings->difficultyComboIndex) {
    default: assert(false); break;
    case 0: //Custom
        this->pluginSettings->difficultyAutoScroll = this->ui->sbAutoScroll->value();
        this->pluginSettings->difficultyBulletTime = this->ui->sbBulletTime->value();
        this->pluginSettings->difficultyHammerTime = this->ui->sbHammerTime->value();
        this->pluginSettings->difficultyWalkingHammerBros = this->ui->sbWalkingHammerBros->value();
        this->pluginSettings->difficultyBuzzyBeetlesReplaceLoneGoombas = this->ui->sbBuzzyBeetleReplaceLoneGoombas->value();
        this->pluginSettings->difficultyBridgeFlyingCheepCheeps = this->ui->sbBridgeFlyingCheepCheeps->value();
        this->pluginSettings->difficultyBridgeLakitus = this->ui->sbBridgeLakitus->value();
        this->pluginSettings->difficultyBridgeOffscreenBulletBills = this->ui->sbBridgeOffscreenBulletBills->value();
        this->pluginSettings->difficultyCastleFireBars = this->ui->sbCastleFireBars->value();
        this->pluginSettings->difficultyCastleFlyingCheepCheeps = this->ui->sbCastleFlyingCheepCheeps->value();
        this->pluginSettings->difficultyCastleLakitus = this->ui->sbCastleLakitus->value();
        this->pluginSettings->difficultyCastleOffscreenBulletBills = this->ui->sbCastleOffscreenBulletBills->value();
        this->pluginSettings->difficultyIslandFlyingCheepCheeps = this->ui->sbIslandFlyingCheepCheeps->value();
        this->pluginSettings->difficultyIslandLakitus = this->ui->sbIslandLakitus->value();
        this->pluginSettings->difficultyIslandOffscreenBulletBills = this->ui->sbIslandOffscreenBulletBills->value();
        this->pluginSettings->difficultyUndergroundFlyingCheepCheeps = this->ui->sbUndergroundFlyingCheepCheeps->value();
        this->pluginSettings->difficultyUndergroundLakitus = this->ui->sbUndergroundLakitus->value();
        this->pluginSettings->difficultyUndergroundOffscreenBulletBills = this->ui->sbUndergroundOffscreenBulletBills->value();
        this->pluginSettings->difficultyUnderwaterBloopers = this->ui->sbUnderwaterBloopers->value();
        this->pluginSettings->difficultyUnderwaterFlyingCheepCheeps = this->ui->sbUnderwaterFlyingCheepCheeps->value();
        this->pluginSettings->difficultyUnderwaterLakitus = this->ui->sbUnderwaterLakitus->value();
        this->pluginSettings->difficultyUnderwaterSwimmingCheepCheeps = this->ui->sbUnderwaterSwimmingCheepCheeps->value();
        this->pluginSettings->difficultyStandardOverworldFlyingCheepCheeps = this->ui->sbStandardOverworldFlyingCheepCheeps->value();
        this->pluginSettings->difficultyStandardOverworldLakitus = this->ui->sbStandardOverworldLakitus->value();
        this->pluginSettings->difficultyStandardOverworldOffscreenBulletBills = this->ui->sbStandardOverworldOffscreenBulletBills->value();
        this->pluginSettings->difficultyNoEnemies = this->ui->cbNoEnemies->isChecked();
        this->pluginSettings->difficultyStartWithFireFlowerOnRoomChange = this->ui->cbStartWithFireFlowerOnRoomChange->isChecked();
        this->pluginSettings->difficultyHammerTimeIntensity = this->ui->sbHammerTimeIntensity->value();
        this->pluginSettings->difficultyReplaceCastleLoops = this->ui->comboReplaceCastleLoops->currentIndex();
        this->pluginSettings->difficultyAutoScrollChancePerLevel = this->ui->sbAutoScrollChancePerLevel->value();
        this->pluginSettings->difficultyLakituSpawnChancePerLevel = this->ui->sbLakituSpawnChancePerLevel->value();
        this->pluginSettings->difficultyLakituRespawnSpeed = this->ui->comboLakituRespawnSpeed->currentIndex();
        this->pluginSettings->lakituThrowArc = this->ui->cbLakituThrowArc->isChecked();
        this->pluginSettings->difficultySpinyEggBehavior = this->ui->comboSpinyEggBehavior->currentIndex();
        this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns = this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->isChecked();
        this->pluginSettings->difficultySpawnerPriority = this->ui->comboSpawnerPriority->currentIndex();
        this->pluginSettings->difficultyPiranhaPlantType = this->ui->comboPiranhaPlantType->currentIndex();
        this->pluginSettings->superMarioOnDamage = this->ui->cbRevertToSuperMario->checkState();
        this->pluginSettings->lakituThrowArc = this->ui->cbLakituThrowArc->checkState();
        this->pluginSettings->difficultyBasicEnemySpeed = this->ui->comboBasicEnemySpeed->currentIndex();
        this->pluginSettings->difficultyBulletBillSpeed = this->ui->comboBulletBillSpeed->currentIndex();
        this->pluginSettings->difficultySpeedyObjectsAndEnemies = this->ui->cbSpeedyObjectsAndEnemies->isChecked();
        break;
    case 1: //Random
        break; //do nothing for now
    case 2: //Very Easy
        Difficulty_Level_Configurations().Very_Easy(this->pluginSettings);
        break;
    case 3: //Easy
        Difficulty_Level_Configurations().Easy(this->pluginSettings);
        break;
    case 4: //Normal
        Difficulty_Level_Configurations().Normal(this->pluginSettings);
        break;
    case 5: //Hard
        Difficulty_Level_Configurations().Hard(this->pluginSettings);
        break;
    case 6: //Very Hard
        Difficulty_Level_Configurations().Very_Hard(this->pluginSettings);
        break;
    case 7: //Brutal
        Difficulty_Level_Configurations().Brutal(this->pluginSettings);
        break;
    case 8: //Purist
        Difficulty_Level_Configurations().Purist(this->pluginSettings);
        break;
    case 9: //Purist + Auto Scroll
        Difficulty_Level_Configurations().Purist_And_Auto_Scroll(this->pluginSettings);
        break;
    case 10: //Purist + No Hammer Bros.
        Difficulty_Level_Configurations().Purist_And_No_Hammer_Bros(this->pluginSettings);
        break;
    case 11: //Purist + Lakitu Throw Arc
        Difficulty_Level_Configurations().Purist_And_Lakitu_Throw_Arc(this->pluginSettings);
        break;
    case 12: //Purist + Lakitu Throw Arc + Auto Scrolling
        Difficulty_Level_Configurations().Purist_And_Lakitu_Throw_Arc_And_Auto_Scroll(this->pluginSettings);
        break;
    case 13: //Purist + Lakitu Throw Arc + No Hammer Bros.
        Difficulty_Level_Configurations().Purist_And_Lakitu_Throw_Arc_And_No_Hammer_Bros(this->pluginSettings);
        break;
    case 14: //Walk in the Park
        Difficulty_Level_Configurations().Walk_In_The_Park(this->pluginSettings);
        break;
    case 15: //Just Keep Scrolling
        Difficulty_Level_Configurations().Just_Keep_Scrolling(this->pluginSettings);
        break;
    case 16: //Riddled with Bullets
        Difficulty_Level_Configurations().Riddled_With_Bullets(this->pluginSettings);
        break;
    case 17: //Lakitu's Challenge
        Difficulty_Level_Configurations().Lakitus_Challenge(this->pluginSettings);
        break;
    case 18: //Cheep-Cheep Frenzy
        Difficulty_Level_Configurations().Cheep_Cheep_Frenzy(this->pluginSettings);
        break;
    case 19: //Hammer Time
        Difficulty_Level_Configurations().Hammer_Time(this->pluginSettings);
        break;
    case 20: //Extreme Hammer Time
        Difficulty_Level_Configurations().Extreme_Hammer_Time(this->pluginSettings);
        break;
    case 21: //Turn up the Heat
        Difficulty_Level_Configurations().Turn_Up_The_Heat(this->pluginSettings);
        break;
    case 22: //Too Hot to Handle
        Difficulty_Level_Configurations().Too_Hot_To_Handle(this->pluginSettings);
        break;
    }
}
