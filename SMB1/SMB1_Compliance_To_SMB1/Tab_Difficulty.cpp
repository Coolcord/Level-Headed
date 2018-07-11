#include "Tab_Difficulty.h"
#include "Configure_Settings_Form.h"
#include "Difficulty_Level_Configurations.h"
#include "ui_Configure_Settings_Form.h"

void Tab_Difficulty::Load_Settings() {
    this->ui->sbLives->setValue(this->pluginSettings->numLives);
    this->ui->cbInfiniteLives->setChecked(this->pluginSettings->infiniteLives);
    this->ui->cbGodMode->setChecked(this->pluginSettings->godMode);
    this->ui->cb2PlayerGame->setChecked(!this->pluginSettings->addLuigiGame);
    this->ui->cbRevertToSuperMario->setChecked(this->pluginSettings->superMarioOnDamage);
    this->ui->cbLakituThrowArc->setChecked(this->pluginSettings->lakituThrowArc);
    this->ui->cbAutoscroll->setCheckState(this->pluginSettings->autoscroll);
    this->ui->comboEnemySpeed->setCurrentIndex(this->pluginSettings->enemySpeed-1);
    this->ui->comboDifficulty->setCurrentIndex(this->pluginSettings->difficultyComboIndex);
    this->ui->sbBulletTime->setValue(this->pluginSettings->difficultyBulletTime);
    this->ui->sbHammerTime->setValue(this->pluginSettings->difficultyHammerTime);
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
    this->ui->sbHammerTimeIntensity->setValue(this->pluginSettings->difficultyHammerTimeIntensity);
    this->ui->sbLakituSpawnChancePerLevel->setValue(this->pluginSettings->difficultyLakituSpawnChancePerLevel);
    this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->setChecked(this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns);
    this->ui->comboSpawnerPriority->setCurrentIndex(this->pluginSettings->difficultySpawnerPriority);
}

void Tab_Difficulty::Save_Settings() {
    this->pluginSettings->numLives = this->ui->sbLives->value();
    this->pluginSettings->godMode = this->ui->cbGodMode->isChecked();
    this->pluginSettings->addLuigiGame = !this->ui->cb2PlayerGame->isChecked();
    this->pluginSettings->superMarioOnDamage = this->ui->cbRevertToSuperMario->checkState();
    this->pluginSettings->lakituThrowArc = this->ui->cbLakituThrowArc->checkState();
    this->pluginSettings->autoscroll = this->ui->cbAutoscroll->checkState();
    this->pluginSettings->enemySpeed = this->ui->comboEnemySpeed->currentIndex()+1;

    //Save the Difficulty Settings
    this->pluginSettings->difficultyComboIndex = this->ui->comboDifficulty->currentIndex();
    switch (this->pluginSettings->difficultyComboIndex) {
    default: assert(false); break;
    case 0: //Custom
        this->pluginSettings->difficultyBulletTime = this->ui->sbBulletTime->value();
        this->pluginSettings->difficultyHammerTime = this->ui->sbHammerTime->value();
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
        this->pluginSettings->difficultyIslandOffscreenBulletBills = this->ui->sbBridgeOffscreenBulletBills->value();
        this->pluginSettings->difficultyUndergroundFlyingCheepCheeps = this->ui->sbBridgeFlyingCheepCheeps->value();
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
        this->pluginSettings->difficultyHammerTimeIntensity = this->ui->sbHammerTimeIntensity->value();
        this->pluginSettings->difficultyLakituSpawnChancePerLevel = this->ui->sbLakituSpawnChancePerLevel->value();
        this->pluginSettings->lakituThrowArc = this->ui->cbLakituThrowArc->isChecked();
        this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns = this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->isChecked();
        this->pluginSettings->difficultySpawnerPriority = this->ui->comboSpawnerPriority->currentIndex();
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
    case 8: //Walk in the Park
        Difficulty_Level_Configurations().Walk_In_The_Park(this->pluginSettings);
        break;
    case 9: //Riddled with Bullets
        Difficulty_Level_Configurations().Riddled_With_Bullets(this->pluginSettings);
        break;
    case 10: //Lakitu's Challenge
        Difficulty_Level_Configurations().Lakitus_Challenge(this->pluginSettings);
        break;
    case 11: //Cheep-Cheep Frenzy
        Difficulty_Level_Configurations().Cheep_Cheep_Frenzy(this->pluginSettings);
        break;
    case 12: //Hammer Time
        Difficulty_Level_Configurations().Hammer_Time(this->pluginSettings);
        break;
    case 13: //Extreme Hammer Time
        Difficulty_Level_Configurations().Extreme_Hammer_Time(this->pluginSettings);
        break;
    }
}
