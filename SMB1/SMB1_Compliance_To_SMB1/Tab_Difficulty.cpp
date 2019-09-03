#include "Tab_Difficulty.h"
#include "Configure_Settings_Form.h"
#include "Difficulty_Level_Configurations.h"
#include "ui_Configure_Settings_Form.h"
#include "../SMB1_Writer/ROM_Filename.h"

void Tab_Difficulty::Load_Settings() {
    if (this->pluginSettings->infiniteLives) this->ui->radioInfiniteLives->setChecked(true);
    else if (this->pluginSettings->permadeath) this->ui->radioPermadeath->setChecked(true);
    else this->ui->radioStartingLives->setChecked(true);
    this->ui->sbLives->setValue(this->pluginSettings->numLives);
    this->ui->cbGodMode->setChecked(this->pluginSettings->godMode);
    this->ui->cbRevertToSuperMario->setChecked(this->pluginSettings->superMarioOnDamage);
    this->ui->cbLakituThrowArc->setChecked(this->pluginSettings->lakituThrowArc);
    this->ui->comboBasicEnemySpeed->setCurrentIndex(this->pluginSettings->difficultyBasicEnemySpeed);
    this->ui->comboBowserFlameFrequency->setCurrentIndex(this->pluginSettings->difficultyBowserFlameFrequency);
    this->ui->comboBulletBillFiringRate->setCurrentIndex(this->pluginSettings->difficultyBulletBillFiringRate);
    this->ui->comboBulletBillSpeed->setCurrentIndex(this->pluginSettings->difficultyBulletBillSpeed);
    this->ui->comboEnemyGroupSize->setCurrentIndex(this->pluginSettings->difficultyEnemyGroupSize);
    this->ui->comboEnemyRevivalSpeed->setCurrentIndex(this->pluginSettings->difficultyEnemyRevivalSpeed);
    this->ui->comboFlyingCheepCheepJumpHeight->setCurrentIndex(this->pluginSettings->difficultyFlyingCheepCheepJumpHeight);
    this->ui->comboHammerBrosThrowRate->setCurrentIndex(this->pluginSettings->difficultyHammerBrosThrowRate);
    this->ui->cbSpeedyObjectsAndEnemies->setChecked(this->pluginSettings->difficultySpeedyObjectsAndEnemies);
    this->ui->cbPiranhaPlantsOnFirstLevel->setChecked(this->pluginSettings->difficultyPiranhaPlantsOnFirstLevel);
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
    this->ui->sbMinimumEnemyDistance->setValue(this->pluginSettings->difficultyMinimumEnemyDistance);
    this->ui->sbMinimumUnderwaterEnemyDistance->setValue(this->pluginSettings->difficultyMinimumUnderwaterEnemyDistance);
    this->ui->cbNoEnemies->setChecked(this->pluginSettings->difficultyNoEnemies);
    this->ui->cbUnlimitedTime->setChecked(this->pluginSettings->difficultyUnlimitedTime);
    this->ui->sbHammerTimeIntensity->setValue(this->pluginSettings->difficultyHammerTimeIntensity);
    this->ui->comboReplaceCastleLoops->setCurrentIndex(this->pluginSettings->difficultyReplaceCastleLoops);
    this->ui->comboMaxLevelLength->setCurrentIndex(this->pluginSettings->difficultyMaxLevelLength);
    this->ui->sbAutoScrollChancePerLevel->setValue(this->pluginSettings->difficultyAutoScrollChancePerLevel);
    this->ui->sbLakituSpawnChancePerLevel->setValue(this->pluginSettings->difficultyLakituSpawnChancePerLevel);
    this->ui->comboLakituRespawnSpeed->setCurrentIndex(this->pluginSettings->difficultyLakituRespawnSpeed);
    this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->setChecked(this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns);
    this->ui->cbDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn->setChecked(this->pluginSettings->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn);
    this->ui->comboSpinyEggBehavior->setCurrentIndex(this->pluginSettings->difficultySpinyEggBehavior);
    this->ui->comboSpawnerPriority->setCurrentIndex(this->pluginSettings->difficultySpawnerPriority);
    this->ui->comboSurfingLiftSpeed->setCurrentIndex(this->pluginSettings->difficultySurfingLiftSpeed);
    this->ui->comboPiranhaPlantType->setCurrentIndex(this->pluginSettings->difficultyPiranhaPlantType);
    this->ui->comboMaxNumberOfPiranhaPlants->setCurrentIndex(this->pluginSettings->difficultyMaxNumberOfPiranhaPlants);
}

void Tab_Difficulty::Save_Settings() {
    bool partialSupport = this->ui->comboBaseROM->currentText().startsWith(ROM_Filename::STRING_PARTIAL_SUPPORT);
    this->pluginSettings->infiniteLives = this->ui->radioInfiniteLives->isChecked();
    this->pluginSettings->permadeath = this->ui->radioPermadeath->isChecked();
    this->pluginSettings->numLives = this->ui->sbLives->value();
    this->pluginSettings->godMode = this->ui->cbGodMode->isChecked();

    //Save the Difficulty Settings
    this->pluginSettings->difficultyComboIndex = this->ui->comboDifficulty->currentIndex();
    if (this->pluginSettings->difficultyComboIndex == 0) { //save custom difficulty settings
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
        this->pluginSettings->difficultyMinimumEnemyDistance = this->ui->sbMinimumEnemyDistance->value();
        this->pluginSettings->difficultyMinimumUnderwaterEnemyDistance = this->ui->sbMinimumUnderwaterEnemyDistance->value();
        this->pluginSettings->difficultyNoEnemies = this->ui->cbNoEnemies->isChecked();
        this->pluginSettings->difficultyUnlimitedTime = this->ui->cbUnlimitedTime->isChecked();
        this->pluginSettings->difficultyHammerTimeIntensity = this->ui->sbHammerTimeIntensity->value();
        this->pluginSettings->difficultyReplaceCastleLoops = this->ui->comboReplaceCastleLoops->currentIndex();
        this->pluginSettings->difficultyMaxLevelLength = this->ui->comboMaxLevelLength->currentIndex();
        this->pluginSettings->difficultyAutoScrollChancePerLevel = this->ui->sbAutoScrollChancePerLevel->value();
        this->pluginSettings->difficultyLakituSpawnChancePerLevel = this->ui->sbLakituSpawnChancePerLevel->value();
        this->pluginSettings->difficultyLakituRespawnSpeed = this->ui->comboLakituRespawnSpeed->currentIndex();
        this->pluginSettings->lakituThrowArc = this->ui->cbLakituThrowArc->isChecked();
        this->pluginSettings->difficultySpinyEggBehavior = this->ui->comboSpinyEggBehavior->currentIndex();
        this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns = this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->isChecked();
        this->pluginSettings->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = this->ui->cbDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn->isChecked();
        this->pluginSettings->difficultySpawnerPriority = this->ui->comboSpawnerPriority->currentIndex();
        this->pluginSettings->difficultySurfingLiftSpeed = this->ui->comboSurfingLiftSpeed->currentIndex();
        this->pluginSettings->difficultyPiranhaPlantType = this->ui->comboPiranhaPlantType->currentIndex();
        this->pluginSettings->difficultyMaxNumberOfPiranhaPlants = this->ui->comboMaxNumberOfPiranhaPlants->currentIndex();
        this->pluginSettings->superMarioOnDamage = this->ui->cbRevertToSuperMario->checkState();
        this->pluginSettings->lakituThrowArc = this->ui->cbLakituThrowArc->checkState();
        this->pluginSettings->difficultyBasicEnemySpeed = this->ui->comboBasicEnemySpeed->currentIndex();
        this->pluginSettings->difficultyBowserFlameFrequency = this->ui->comboBowserFlameFrequency->currentIndex();
        this->pluginSettings->difficultyBulletBillFiringRate = this->ui->comboBulletBillFiringRate->currentIndex();
        this->pluginSettings->difficultyBulletBillSpeed = this->ui->comboBulletBillSpeed->currentIndex();
        this->pluginSettings->difficultyEnemyGroupSize = this->ui->comboEnemyGroupSize->currentIndex();
        this->pluginSettings->difficultyEnemyRevivalSpeed = this->ui->comboEnemyRevivalSpeed->currentIndex();
        this->pluginSettings->difficultyFlyingCheepCheepJumpHeight = this->ui->comboFlyingCheepCheepJumpHeight->currentIndex();
        this->pluginSettings->difficultyHammerBrosThrowRate = this->ui->comboHammerBrosThrowRate->currentIndex();
        this->pluginSettings->difficultySpeedyObjectsAndEnemies = this->ui->cbSpeedyObjectsAndEnemies->isChecked();
        this->pluginSettings->difficultyPiranhaPlantsOnFirstLevel = this->ui->cbPiranhaPlantsOnFirstLevel->isChecked();
    } else { //get difficulty settings from the present
        assert(Difficulty_Level_Configurations().Update_Plugin_Settings_For_Difficulty_Present(this->pluginSettings->difficultyComboIndex, this->pluginSettings, partialSupport));
    }
}
