#include "Tab_Difficulty.h"
#include "Configure_Settings_Form.h"
#include "Difficulty_Level_Configurations.h"
#include "ui_Configure_Settings_Form.h"
#include "../SMB1_Writer/ROM_Filename.h"

void Tab_Difficulty::Load_Settings() {
    this->Load_Settings(this->pluginSettings);
}

void Tab_Difficulty::Load_Settings(Plugin_Settings *ps) {
    if (ps->infiniteLives) {
        this->ui->radioInfiniteLives->setChecked(true);
        this->ui->cbAlwaysRestartFromCurrentWorldAfterGameOver->setChecked(false);
    } else if (ps->permadeath) {
        this->ui->radioPermadeath->setChecked(true);
        this->ui->cbAlwaysRestartFromCurrentWorldAfterGameOver->setChecked(false);
    } else {
        this->ui->radioStartingLives->setChecked(true);
        this->ui->cbAlwaysRestartFromCurrentWorldAfterGameOver->setChecked(ps->alwaysRestartFromCurrentWorldAfterGameOver);
    }
    this->ui->sbLives->setValue(ps->numLives);
    if (this->ui->cbGodMode->isEnabled()) this->ui->cbGodMode->setChecked(ps->godMode);
    if (this->ui->comboPowerup->isEnabled()) this->ui->comboPowerup->setCurrentIndex(ps->powerup);
    this->ui->cbRevertToSuperMario->setChecked(ps->superMarioOnDamage);
    this->ui->cbEuropeanBlooperSwimHeight->setChecked(ps->difficultyEuropeanBlooperSwimHeight);
    this->ui->cbLakituThrowArc->setChecked(ps->lakituThrowArc);
    this->ui->comboBasicEnemySpeed->setCurrentIndex(ps->difficultyBasicEnemySpeed);
    this->ui->comboBowserFlameFrequency->setCurrentIndex(ps->difficultyBowserFlameFrequency);
    this->ui->comboBulletBillFiringRate->setCurrentIndex(ps->difficultyBulletBillFiringRate);
    this->ui->comboBulletBillSpeed->setCurrentIndex(ps->difficultyBulletBillSpeed);
    this->ui->comboLeapingParatroopaSpeed->setCurrentIndex(ps->difficultyLeapingParatroopaSpeed);
    this->ui->comboEnemyRevivalSpeed->setCurrentIndex(ps->difficultyEnemyRevivalSpeed);
    this->ui->comboFireBarLength->setCurrentIndex(ps->difficultyFireBarLength);
    this->ui->comboLongFireBarLength->setCurrentIndex(ps->difficultyLongFireBarLength);
    this->ui->comboFlyingCheepCheepJumpHeight->setCurrentIndex(ps->difficultyFlyingCheepCheepJumpHeight);
    this->ui->comboHammerBrosThrowRate->setCurrentIndex(ps->difficultyHammerBrosThrowRate);
    this->ui->cbHammerBrosNeverJump->setChecked(ps->difficultyHammerBrosNeverJump);
    this->ui->cbFireBowserThrowsALudicrousAmountOfFireballs->setChecked(ps->difficultyFireBowserThrowsALudicrousAmountOfFireballs);
    this->ui->cbSpeedyObjectsAndEnemies->setChecked(ps->difficultySpeedyObjectsAndEnemies);
    this->ui->cbPiranhaPlantsOnFirstLevel->setChecked(ps->difficultyPiranhaPlantsOnFirstLevel);
    this->ui->cbPreventTheFirstLevelFromBeingUnderwater->setChecked(ps->difficultyPreventTheFirstLevelFromBeingUnderwater);
    this->ui->cbPreventHammerTimeFromSpawningTwoHammerBrosInARow->setChecked(ps->difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow);
    this->ui->cbAllowHammerBrosWhenRandomizingEnemiesInLevelScripts->setChecked(ps->difficultyAllowHammerBrosWhenRandomizingEnemiesInLevelScripts);
    this->ui->cbAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts->setChecked(ps->difficultyAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts);
    this->ui->cbAllowLakitusWhenRandomizingEnemiesInLevelScripts->setChecked(ps->difficultyAllowLakitusWhenRandomizingEnemiesInLevelScripts);
    this->ui->cbAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts->setChecked(ps->difficultyAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts);
    this->ui->comboDifficulty->setCurrentIndex(ps->difficultyComboIndex);
    this->ui->sbAutoScroll->setValue(ps->difficultyAutoScroll);
    this->ui->sbAutoScrollWithFlyingCheepCheeps->setValue(ps->difficultyAutoScrollWithFlyingCheepCheeps);
    this->ui->sbBulletTime->setValue(ps->difficultyBulletTime);
    this->ui->sbHammerTime->setValue(ps->difficultyHammerTime);
    this->ui->sbWalkingHammerBros->setValue(ps->difficultyWalkingHammerBros);
    this->ui->sbBuzzyBeetleReplaceLoneGoombas->setValue(ps->difficultyBuzzyBeetlesReplaceLoneGoombas);
    this->ui->sbBridgeFlyingCheepCheeps->setValue(ps->difficultyBridgeFlyingCheepCheeps);
    this->ui->sbBridgeLakitus->setValue(ps->difficultyBridgeLakitus);
    this->ui->sbBridgeOffscreenBulletBills->setValue(ps->difficultyBridgeOffscreenBulletBills);
    this->ui->sbCastleFireBars->setValue(ps->difficultyCastleFireBars);
    this->ui->sbCastleFlyingCheepCheeps->setValue(ps->difficultyCastleFlyingCheepCheeps);
    this->ui->sbCastleLakitus->setValue(ps->difficultyCastleLakitus);
    this->ui->sbCastleOffscreenBulletBills->setValue(ps->difficultyCastleOffscreenBulletBills);
    this->ui->sbIslandFlyingCheepCheeps->setValue(ps->difficultyIslandFlyingCheepCheeps);
    this->ui->sbIslandLakitus->setValue(ps->difficultyIslandLakitus);
    this->ui->sbIslandOffscreenBulletBills->setValue(ps->difficultyIslandOffscreenBulletBills);
    this->ui->sbUndergroundFlyingCheepCheeps->setValue(ps->difficultyUndergroundFlyingCheepCheeps);
    this->ui->sbUndergroundLakitus->setValue(ps->difficultyUndergroundLakitus);
    this->ui->sbUndergroundOffscreenBulletBills->setValue(ps->difficultyUndergroundOffscreenBulletBills);
    this->ui->sbUnderwaterBloopers->setValue(ps->difficultyUnderwaterBloopers);
    this->ui->sbUnderwaterFlyingCheepCheeps->setValue(ps->difficultyUnderwaterFlyingCheepCheeps);
    this->ui->sbUnderwaterHammerBros->setValue(ps->difficultyUnderwaterHammerBros);
    this->ui->sbUnderwaterLakitus->setValue(ps->difficultyUnderwaterLakitus);
    this->ui->sbUnderwaterSwimmingCheepCheeps->setValue(ps->difficultyUnderwaterSwimmingCheepCheeps);
    this->ui->sbStandardOverworldFlyingCheepCheeps->setValue(ps->difficultyStandardOverworldFlyingCheepCheeps);
    this->ui->sbStandardOverworldLakitus->setValue(ps->difficultyStandardOverworldLakitus);
    this->ui->sbStandardOverworldOffscreenBulletBills->setValue(ps->difficultyStandardOverworldOffscreenBulletBills);
    this->ui->sbMinimumEnemyDistance->setValue(ps->difficultyMinimumEnemyDistance);
    this->ui->sbMinimumUnderwaterEnemyDistance->setValue(ps->difficultyMinimumUnderwaterEnemyDistance);
    this->ui->cbNoEnemies->setChecked(ps->difficultyNoEnemies);
    this->ui->cbUnlimitedTime->setChecked(ps->difficultyUnlimitedTime);
    this->ui->sbHammerTimeIntensity->setValue(ps->difficultyHammerTimeIntensity);
    this->ui->comboReplaceCastleLoops->setCurrentIndex(ps->difficultyReplaceCastleLoops);
    this->ui->comboMaxLevelLength->setCurrentIndex(ps->difficultyMaxLevelLength);
    this->ui->sbAutoScrollChancePerLevel->setValue(ps->difficultyAutoScrollChancePerLevel);
    this->ui->sbLakituSpawnChancePerLevel->setValue(ps->difficultyLakituSpawnChancePerLevel);
    this->ui->comboLakituRespawnSpeed->setCurrentIndex(ps->difficultyLakituRespawnSpeed);
    this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->setChecked(ps->difficultyDisableAllOtherEnemiesWhenALakituSpawns);
    this->ui->cbDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn->setChecked(ps->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn);
    this->ui->comboSpinyEggBehavior->setCurrentIndex(ps->difficultySpinyEggBehavior);
    this->ui->comboSpawnerPriority->setCurrentIndex(ps->difficultySpawnerPriority);
    this->ui->comboSecondaryMushroom->setCurrentIndex(ps->difficultySecondaryMushroom);
    this->ui->comboSurfingLiftSpeed->setCurrentIndex(ps->difficultySurfingLiftSpeed);
    this->ui->comboPiranhaPlantType->setCurrentIndex(ps->difficultyPiranhaPlantType);
    this->ui->comboMaxNumberOfPiranhaPlants->setCurrentIndex(ps->difficultyMaxNumberOfPiranhaPlants);
    this->ui->sbMaxQuestionBlockPowerups->setValue(ps->difficultyMaxPowerups);
    this->ui->sbMaxBrickBlockPowerups->setValue(ps->difficultyMaxHiddenPowerups);
    this->ui->sbMaxOneUps->setValue(ps->difficultyMaxOneUps);
    this->ui->sbMaxTenCoinBlocks->setValue(ps->difficultyMaxTenCoinBlocks);
    this->ui->sbMaxStarmen->setValue(ps->difficultyMaxStars);
    this->ui->sbBrickBlockPowerupSpawnChance->setValue(ps->difficultyHiddenPowerupChance);
    this->ui->sbOneUpSpawnChance->setValue(ps->difficultyOneUpChance);
    this->ui->sbTenCoinBlockSpawnChance->setValue(ps->difficultyTenCoinBlockChance);
    this->ui->sbStarmanSpawnChance->setValue(ps->difficultyStarChance);
}

void Tab_Difficulty::Save_Settings() {
    this->Save_Settings(this->pluginSettings);
}

void Tab_Difficulty::Save_Settings(Plugin_Settings *ps) {
    bool partialSupport = this->ui->comboBaseROM->currentText().startsWith(ROM_Filename::STRING_PARTIAL_SUPPORT);
    ps->infiniteLives = this->ui->radioInfiniteLives->isChecked();
    ps->permadeath = this->ui->radioPermadeath->isChecked();
    ps->alwaysRestartFromCurrentWorldAfterGameOver = this->ui->cbAlwaysRestartFromCurrentWorldAfterGameOver->isChecked();
    ps->numLives = this->ui->sbLives->value();
    if (this->ui->cbGodMode->isEnabled()) ps->godMode = this->ui->cbGodMode->isChecked();
    if (this->ui->comboPowerup->isEnabled()) ps->powerup = this->ui->comboPowerup->currentIndex();

    //Save the Difficulty Settings
    ps->difficultyComboIndex = this->ui->comboDifficulty->currentIndex();
    if (ps->difficultyComboIndex == 0) { //save custom difficulty settings
        ps->difficultyAutoScroll = this->ui->sbAutoScroll->value();
        ps->difficultyAutoScrollWithFlyingCheepCheeps = this->ui->sbAutoScrollWithFlyingCheepCheeps->value();
        ps->difficultyBulletTime = this->ui->sbBulletTime->value();
        ps->difficultyHammerTime = this->ui->sbHammerTime->value();
        ps->difficultyWalkingHammerBros = this->ui->sbWalkingHammerBros->value();
        ps->difficultyBuzzyBeetlesReplaceLoneGoombas = this->ui->sbBuzzyBeetleReplaceLoneGoombas->value();
        ps->difficultyBridgeFlyingCheepCheeps = this->ui->sbBridgeFlyingCheepCheeps->value();
        ps->difficultyBridgeLakitus = this->ui->sbBridgeLakitus->value();
        ps->difficultyBridgeOffscreenBulletBills = this->ui->sbBridgeOffscreenBulletBills->value();
        ps->difficultyCastleFireBars = this->ui->sbCastleFireBars->value();
        ps->difficultyCastleFlyingCheepCheeps = this->ui->sbCastleFlyingCheepCheeps->value();
        ps->difficultyCastleLakitus = this->ui->sbCastleLakitus->value();
        ps->difficultyCastleOffscreenBulletBills = this->ui->sbCastleOffscreenBulletBills->value();
        ps->difficultyIslandFlyingCheepCheeps = this->ui->sbIslandFlyingCheepCheeps->value();
        ps->difficultyIslandLakitus = this->ui->sbIslandLakitus->value();
        ps->difficultyIslandOffscreenBulletBills = this->ui->sbIslandOffscreenBulletBills->value();
        ps->difficultyUndergroundFlyingCheepCheeps = this->ui->sbUndergroundFlyingCheepCheeps->value();
        ps->difficultyUndergroundLakitus = this->ui->sbUndergroundLakitus->value();
        ps->difficultyUndergroundOffscreenBulletBills = this->ui->sbUndergroundOffscreenBulletBills->value();
        ps->difficultyUnderwaterBloopers = this->ui->sbUnderwaterBloopers->value();
        ps->difficultyUnderwaterFlyingCheepCheeps = this->ui->sbUnderwaterFlyingCheepCheeps->value();
        ps->difficultyUnderwaterHammerBros = this->ui->sbUnderwaterHammerBros->value();
        ps->difficultyUnderwaterLakitus = this->ui->sbUnderwaterLakitus->value();
        ps->difficultyUnderwaterSwimmingCheepCheeps = this->ui->sbUnderwaterSwimmingCheepCheeps->value();
        ps->difficultyStandardOverworldFlyingCheepCheeps = this->ui->sbStandardOverworldFlyingCheepCheeps->value();
        ps->difficultyStandardOverworldLakitus = this->ui->sbStandardOverworldLakitus->value();
        ps->difficultyStandardOverworldOffscreenBulletBills = this->ui->sbStandardOverworldOffscreenBulletBills->value();
        ps->difficultyMinimumEnemyDistance = this->ui->sbMinimumEnemyDistance->value();
        ps->difficultyMinimumUnderwaterEnemyDistance = this->ui->sbMinimumUnderwaterEnemyDistance->value();
        ps->difficultyNoEnemies = this->ui->cbNoEnemies->isChecked();
        ps->difficultyUnlimitedTime = this->ui->cbUnlimitedTime->isChecked();
        ps->difficultyHammerTimeIntensity = this->ui->sbHammerTimeIntensity->value();
        ps->difficultyReplaceCastleLoops = this->ui->comboReplaceCastleLoops->currentIndex();
        ps->difficultyMaxLevelLength = this->ui->comboMaxLevelLength->currentIndex();
        ps->difficultyAutoScrollChancePerLevel = this->ui->sbAutoScrollChancePerLevel->value();
        ps->difficultyLakituSpawnChancePerLevel = this->ui->sbLakituSpawnChancePerLevel->value();
        ps->difficultyLakituRespawnSpeed = this->ui->comboLakituRespawnSpeed->currentIndex();
        ps->difficultySpinyEggBehavior = this->ui->comboSpinyEggBehavior->currentIndex();
        ps->difficultyDisableAllOtherEnemiesWhenALakituSpawns = this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->isChecked();
        ps->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = this->ui->cbDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn->isChecked();
        ps->difficultySpawnerPriority = this->ui->comboSpawnerPriority->currentIndex();
        ps->difficultySecondaryMushroom = this->ui->comboSecondaryMushroom->currentIndex();
        ps->difficultySurfingLiftSpeed = this->ui->comboSurfingLiftSpeed->currentIndex();
        ps->difficultyPiranhaPlantType = this->ui->comboPiranhaPlantType->currentIndex();
        ps->difficultyMaxNumberOfPiranhaPlants = this->ui->comboMaxNumberOfPiranhaPlants->currentIndex();
        ps->superMarioOnDamage = this->ui->cbRevertToSuperMario->isChecked();
        ps->difficultyEuropeanBlooperSwimHeight = this->ui->cbEuropeanBlooperSwimHeight->isChecked();
        ps->lakituThrowArc = this->ui->cbLakituThrowArc->isChecked();
        ps->difficultyBasicEnemySpeed = this->ui->comboBasicEnemySpeed->currentIndex();
        ps->difficultyBowserFlameFrequency = this->ui->comboBowserFlameFrequency->currentIndex();
        ps->difficultyBulletBillFiringRate = this->ui->comboBulletBillFiringRate->currentIndex();
        ps->difficultyBulletBillSpeed = this->ui->comboBulletBillSpeed->currentIndex();
        ps->difficultyLeapingParatroopaSpeed = this->ui->comboLeapingParatroopaSpeed->currentIndex();
        ps->difficultyEnemyRevivalSpeed = this->ui->comboEnemyRevivalSpeed->currentIndex();
        ps->difficultyFireBarLength = this->ui->comboFireBarLength->currentIndex();
        ps->difficultyLongFireBarLength = this->ui->comboLongFireBarLength->currentIndex();
        ps->difficultyFlyingCheepCheepJumpHeight = this->ui->comboFlyingCheepCheepJumpHeight->currentIndex();
        ps->difficultyHammerBrosThrowRate = this->ui->comboHammerBrosThrowRate->currentIndex();
        ps->difficultyHammerBrosNeverJump = this->ui->cbHammerBrosNeverJump->isChecked();
        ps->difficultyFireBowserThrowsALudicrousAmountOfFireballs = this->ui->cbFireBowserThrowsALudicrousAmountOfFireballs->isChecked();
        ps->difficultySpeedyObjectsAndEnemies = this->ui->cbSpeedyObjectsAndEnemies->isChecked();
        ps->difficultyPiranhaPlantsOnFirstLevel = this->ui->cbPiranhaPlantsOnFirstLevel->isChecked();
        ps->difficultyPreventTheFirstLevelFromBeingUnderwater = this->ui->cbPreventTheFirstLevelFromBeingUnderwater->isChecked();
        ps->difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow = this->ui->cbPreventHammerTimeFromSpawningTwoHammerBrosInARow->isChecked();
        ps->difficultyAllowHammerBrosWhenRandomizingEnemiesInLevelScripts = this->ui->cbAllowHammerBrosWhenRandomizingEnemiesInLevelScripts->isChecked();
        ps->difficultyAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts = this->ui->cbAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts->isChecked();
        ps->difficultyAllowLakitusWhenRandomizingEnemiesInLevelScripts = this->ui->cbAllowLakitusWhenRandomizingEnemiesInLevelScripts->isChecked();
        ps->difficultyAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts = this->ui->cbAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts->isChecked();
        ps->difficultyMaxPowerups = this->ui->sbMaxQuestionBlockPowerups->value();
        ps->difficultyMaxHiddenPowerups = this->ui->sbMaxBrickBlockPowerups->value();
        ps->difficultyMaxOneUps = this->ui->sbMaxOneUps->value();
        ps->difficultyMaxTenCoinBlocks = this->ui->sbMaxTenCoinBlocks->value();
        ps->difficultyMaxStars = this->ui->sbMaxStarmen->value();
        ps->difficultyHiddenPowerupChance = this->ui->sbBrickBlockPowerupSpawnChance->value();
        ps->difficultyOneUpChance = this->ui->sbOneUpSpawnChance->value();
        ps->difficultyTenCoinBlockChance = this->ui->sbTenCoinBlockSpawnChance->value();
        ps->difficultyStarChance = this->ui->sbStarmanSpawnChance->value();
    } else { //get difficulty settings from the present
        assert(Difficulty_Level_Configurations().Update_Plugin_Settings_For_Difficulty_Present(ps->difficultyComboIndex, this->pluginSettings, partialSupport));
    }
}
