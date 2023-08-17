#include "Tab_Difficulty.h"
#include "Configure_Settings_Form.h"
#include "Difficulty_Level_Configurations.h"
#include "ui_Configure_Settings_Form.h"
#include "../SMB1_Writer/ROM_Filename.h"
#include "../SMB1_Compliance_Generator/Difficulty.h"

void Tab_Difficulty::Load_Settings() {
    this->Load_Settings(this->pluginSettings);
}

void Tab_Difficulty::Load_Settings(Plugin_Settings *ps) {
    //Fix Settings First
    this->Make_Difficulty_Value_Sane(ps->difficultyAutoScroll);
    this->Make_Difficulty_Value_Sane(ps->difficultyAutoScrollWithFlyingCheepCheeps);
    this->Make_Difficulty_Value_Sane(ps->difficultyBulletTime);
    this->Make_Difficulty_Value_Sane(ps->difficultyHammerTime);
    this->Make_Difficulty_Value_Sane(ps->difficultyWalkingHammerBros);
    this->Make_Difficulty_Value_Sane(ps->difficultyBuzzyBeetlesReplaceLoneGoombas);
    this->Make_Difficulty_Value_Sane(ps->difficultyBridgeFlyingCheepCheeps);
    this->Make_Difficulty_Value_Sane(ps->difficultyBridgeLakitus);
    this->Make_Difficulty_Value_Sane(ps->difficultyBridgeOffscreenBulletBills);
    this->Make_Difficulty_Value_Sane(ps->difficultyCastleFireBars);
    this->Make_Difficulty_Value_Sane(ps->difficultyCastleFlyingCheepCheeps);
    this->Make_Difficulty_Value_Sane(ps->difficultyCastleLakitus);
    this->Make_Difficulty_Value_Sane(ps->difficultyCastleOffscreenBulletBills);
    this->Make_Difficulty_Value_Sane(ps->difficultyIslandFlyingCheepCheeps);
    this->Make_Difficulty_Value_Sane(ps->difficultyIslandLakitus);
    this->Make_Difficulty_Value_Sane(ps->difficultyIslandOffscreenBulletBills);
    this->Make_Difficulty_Value_Sane(ps->difficultyUndergroundFlyingCheepCheeps);
    this->Make_Difficulty_Value_Sane(ps->difficultyUndergroundLakitus);
    this->Make_Difficulty_Value_Sane(ps->difficultyUndergroundOffscreenBulletBills);
    this->Make_Difficulty_Value_Sane(ps->difficultyUnderwaterBloopers);
    this->Make_Difficulty_Value_Sane(ps->difficultyUnderwaterFlyingCheepCheeps);
    this->Make_Difficulty_Value_Sane(ps->difficultyUnderwaterHammerBros);
    this->Make_Difficulty_Value_Sane(ps->difficultyUnderwaterLakitus);
    this->Make_Difficulty_Value_Sane(ps->difficultyUnderwaterSwimmingCheepCheeps);
    this->Make_Difficulty_Value_Sane(ps->difficultyStandardOverworldFlyingCheepCheeps);
    this->Make_Difficulty_Value_Sane(ps->difficultyStandardOverworldLakitus);
    this->Make_Difficulty_Value_Sane(ps->difficultyStandardOverworldOffscreenBulletBills);

    //Load Settings
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
    this->Load_Spin_Box_Value(this->ui->sbLives, ps->numLives);
    if (this->ui->cbGodMode->isEnabled()) this->ui->cbGodMode->setChecked(ps->godMode);
    if (this->ui->comboPowerup->isEnabled()) this->ui->comboPowerup->setCurrentIndex(ps->powerup);
    this->ui->cbRevertToSuperMario->setChecked(ps->superMarioOnDamage);
    this->ui->cbEuropeanBlooperSwimHeight->setChecked(ps->difficultyEuropeanBlooperSwimHeight);
    this->ui->cbLakituThrowArc->setChecked(ps->lakituThrowArc);
    this->Load_Combo_Box_Value(this->ui->comboBasicEnemySpeed, ps->difficultyBasicEnemySpeed);
    this->Load_Combo_Box_Value(this->ui->comboBowserFlameFrequency, ps->difficultyBowserFlameFrequency);
    this->Load_Combo_Box_Value(this->ui->comboBulletBillFiringRate, ps->difficultyBulletBillFiringRate);
    this->Load_Combo_Box_Value(this->ui->comboBulletBillSpeed, ps->difficultyBulletBillSpeed);
    this->Load_Combo_Box_Value(this->ui->comboLeapingParatroopaSpeed, ps->difficultyLeapingParatroopaSpeed);
    this->Load_Combo_Box_Value(this->ui->comboEnemyRevivalSpeed, ps->difficultyEnemyRevivalSpeed);
    this->Load_Combo_Box_Value(this->ui->comboFireBarLength, ps->difficultyFireBarLength);
    this->Load_Combo_Box_Value(this->ui->comboLongFireBarLength, ps->difficultyLongFireBarLength);
    this->Load_Combo_Box_Value(this->ui->comboFlyingCheepCheepJumpHeight, ps->difficultyFlyingCheepCheepJumpHeight);
    this->Load_Combo_Box_Value(this->ui->comboHammerBrosThrowRate, ps->difficultyHammerBrosThrowRate);
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
    this->Load_Combo_Box_Value(this->ui->comboDifficulty, ps->difficultyComboIndex);
    this->Load_Spin_Box_Value(this->ui->sbAutoScroll, ps->difficultyAutoScroll);
    this->Load_Spin_Box_Value(this->ui->sbAutoScrollWithFlyingCheepCheeps, ps->difficultyAutoScrollWithFlyingCheepCheeps);
    this->Load_Spin_Box_Value(this->ui->sbBulletTime, ps->difficultyBulletTime);
    this->Load_Spin_Box_Value(this->ui->sbHammerTime, ps->difficultyHammerTime);
    this->Load_Spin_Box_Value(this->ui->sbWalkingHammerBros, ps->difficultyWalkingHammerBros);
    this->Load_Spin_Box_Value(this->ui->sbBuzzyBeetleReplaceLoneGoombas, ps->difficultyBuzzyBeetlesReplaceLoneGoombas);
    this->Load_Spin_Box_Value(this->ui->sbBridgeFlyingCheepCheeps, ps->difficultyBridgeFlyingCheepCheeps);
    this->Load_Spin_Box_Value(this->ui->sbBridgeLakitus, ps->difficultyBridgeLakitus);
    this->Load_Spin_Box_Value(this->ui->sbBridgeOffscreenBulletBills, ps->difficultyBridgeOffscreenBulletBills);
    this->Load_Spin_Box_Value(this->ui->sbCastleFireBars, ps->difficultyCastleFireBars);
    this->Load_Spin_Box_Value(this->ui->sbCastleFlyingCheepCheeps, ps->difficultyCastleFlyingCheepCheeps);
    this->Load_Spin_Box_Value(this->ui->sbCastleLakitus, ps->difficultyCastleLakitus);
    this->Load_Spin_Box_Value(this->ui->sbCastleOffscreenBulletBills, ps->difficultyCastleOffscreenBulletBills);
    this->Load_Spin_Box_Value(this->ui->sbIslandFlyingCheepCheeps, ps->difficultyIslandFlyingCheepCheeps);
    this->Load_Spin_Box_Value(this->ui->sbIslandLakitus, ps->difficultyIslandLakitus);
    this->Load_Spin_Box_Value(this->ui->sbIslandOffscreenBulletBills, ps->difficultyIslandOffscreenBulletBills);
    this->Load_Spin_Box_Value(this->ui->sbUndergroundFlyingCheepCheeps, ps->difficultyUndergroundFlyingCheepCheeps);
    this->Load_Spin_Box_Value(this->ui->sbUndergroundLakitus, ps->difficultyUndergroundLakitus);
    this->Load_Spin_Box_Value(this->ui->sbUndergroundOffscreenBulletBills, ps->difficultyUndergroundOffscreenBulletBills);
    this->Load_Spin_Box_Value(this->ui->sbUnderwaterBloopers, ps->difficultyUnderwaterBloopers);
    this->Load_Spin_Box_Value(this->ui->sbUnderwaterFlyingCheepCheeps, ps->difficultyUnderwaterFlyingCheepCheeps);
    this->Load_Spin_Box_Value(this->ui->sbUnderwaterHammerBros, ps->difficultyUnderwaterHammerBros);
    this->Load_Spin_Box_Value(this->ui->sbUnderwaterLakitus, ps->difficultyUnderwaterLakitus);
    this->Load_Spin_Box_Value(this->ui->sbUnderwaterSwimmingCheepCheeps, ps->difficultyUnderwaterSwimmingCheepCheeps);
    this->Load_Spin_Box_Value(this->ui->sbStandardOverworldFlyingCheepCheeps, ps->difficultyStandardOverworldFlyingCheepCheeps);
    this->Load_Spin_Box_Value(this->ui->sbStandardOverworldLakitus, ps->difficultyStandardOverworldLakitus);
    this->Load_Spin_Box_Value(this->ui->sbStandardOverworldOffscreenBulletBills, ps->difficultyStandardOverworldOffscreenBulletBills);
    this->Load_Spin_Box_Value(this->ui->sbMinimumEnemyDistance, ps->difficultyMinimumEnemyDistance);
    this->Load_Spin_Box_Value(this->ui->sbMinimumUnderwaterEnemyDistance, ps->difficultyMinimumUnderwaterEnemyDistance);
    this->ui->cbNoEnemies->setChecked(ps->difficultyNoEnemies);
    this->ui->cbUnlimitedTime->setChecked(ps->difficultyUnlimitedTime);
    this->Load_Spin_Box_Value(this->ui->sbHammerTimeIntensity, ps->difficultyHammerTimeIntensity);
    this->Load_Combo_Box_Value(this->ui->comboReplaceCastleLoops, ps->difficultyReplaceCastleLoops);
    this->Load_Combo_Box_Value(this->ui->comboMaxLevelLength, ps->difficultyMaxLevelLength);
    this->Load_Spin_Box_Value(this->ui->sbAutoScrollChancePerLevel, ps->difficultyAutoScrollChancePerLevel);
    this->Load_Spin_Box_Value(this->ui->sbLakituSpawnChancePerLevel, ps->difficultyLakituSpawnChancePerLevel);
    this->Load_Combo_Box_Value(this->ui->comboLakituRespawnSpeed, ps->difficultyLakituRespawnSpeed);
    this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->setChecked(ps->difficultyDisableAllOtherEnemiesWhenALakituSpawns);
    this->ui->cbDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn->setChecked(ps->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn);
    this->Load_Combo_Box_Value(this->ui->comboSpinyEggBehavior, ps->difficultySpinyEggBehavior);
    this->Load_Combo_Box_Value(this->ui->comboSpawnerPriority, ps->difficultySpawnerPriority);
    this->Load_Combo_Box_Value(this->ui->comboSecondaryMushroom, ps->difficultySecondaryMushroom);
    this->Load_Combo_Box_Value(this->ui->comboSurfingLiftSpeed, ps->difficultySurfingLiftSpeed);
    this->Load_Combo_Box_Value(this->ui->comboPiranhaPlantType, ps->difficultyPiranhaPlantType);
    this->Load_Combo_Box_Value(this->ui->comboMaxNumberOfPiranhaPlants, ps->difficultyMaxNumberOfPiranhaPlants);
    this->Load_Spin_Box_Value(this->ui->sbMaxQuestionBlockPowerups, ps->difficultyMaxPowerups);
    this->Load_Spin_Box_Value(this->ui->sbMaxBrickBlockPowerups, ps->difficultyMaxHiddenPowerups);
    this->Load_Spin_Box_Value(this->ui->sbMaxOneUps, ps->difficultyMaxOneUps);
    this->Load_Spin_Box_Value(this->ui->sbMaxTenCoinBlocks, ps->difficultyMaxTenCoinBlocks);
    this->Load_Spin_Box_Value(this->ui->sbMaxStarmen, ps->difficultyMaxStars);
    this->Load_Spin_Box_Value(this->ui->sbBrickBlockPowerupSpawnChance, ps->difficultyHiddenPowerupChance);
    this->Load_Spin_Box_Value(this->ui->sbOneUpSpawnChance, ps->difficultyOneUpChance);
    this->Load_Spin_Box_Value(this->ui->sbTenCoinBlockSpawnChance, ps->difficultyTenCoinBlockChance);
    this->Load_Spin_Box_Value(this->ui->sbStarmanSpawnChance, ps->difficultyStarChance);
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

void Tab_Difficulty::Load_Combo_Box_Value(QComboBox *comboBox, int &value) {
    if (value < 0 || value >= comboBox->count()) value = 0;
    comboBox->setCurrentIndex(value);
}

void Tab_Difficulty::Load_Spin_Box_Value(QSpinBox *spinBox, int &value) {
    if (value < spinBox->minimum()) value = spinBox->minimum();
    if (value > spinBox->maximum()) value = spinBox->maximum();
    spinBox->setValue(value);
}

void Tab_Difficulty::Make_Difficulty_Value_Sane(int &value) {
    if (value < Difficulty::DIFFICULTY_MIN) value = Difficulty::DIFFICULTY_MIN;
    if (value > Difficulty::DIFFICULTY_MAX+1) value = Difficulty::DIFFICULTY_MAX+1;
}
