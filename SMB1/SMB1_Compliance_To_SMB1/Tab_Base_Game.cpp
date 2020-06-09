#include "Tab_Base_Game.h"
#include "Configure_Settings_Form.h"
#include "ui_Configure_Settings_Form.h"
#include "../SMB1_Writer/ROM_Filename.h"
#include <QDir>
#include <QFileInfo>
#include <QSet>

Tab_Base_Game::Tab_Base_Game(QWidget *p, const QString &apl, SMB1_Writer_Interface *wp, Ui::Configure_Settings_Form *u, Plugin_Settings *ps)
    : Tab_Interface(p, apl, wp, u, ps) {
    this->Populate_Installed_ROMs();
    this->Populate_Graphics_Packs();
    this->Populate_Mario_Sprites();
    this->Populate_Music_Packs();
    this->partialSupportMode = false;
}

void Tab_Base_Game::Load_Settings() {
    if (!this->pluginSettings->baseROM.isEmpty()) this->ui->comboBaseROM->setCurrentText(this->pluginSettings->baseROM);
    this->ui->leOutputROMLocation->setText(this->pluginSettings->outputROMLocation);
    if (this->pluginSettings->overwriteOuputROM) this->ui->radioOverwriteOutputROM->setChecked(true);
    else this->ui->radioAppendNumberToFilename->setChecked(true);
    if (this->pluginSettings->addLuigiGame) this->ui->radioAddLuigiGame->setChecked(true);
    else this->ui->radio2PlayerGame->setChecked(true);
    if (this->pluginSettings->music < this->ui->comboMusic->count()) this->ui->comboMusic->setCurrentIndex(this->pluginSettings->music);
    else this->ui->comboMusic->setCurrentIndex(2); //use original music
    this->ui->cbCombineWithOtherMusicPacks->setChecked(this->pluginSettings->combineMusicWithOtherPacks);
    this->ui->comboTone->setCurrentIndex(this->pluginSettings->toneColor);
    this->ui->cbRandomSoundEffects->setChecked(this->pluginSettings->randomSounds);
    this->ui->cbRandomText->setChecked(this->pluginSettings->randomText);
    this->ui->cbRandomizeSomeAnimations->setChecked(this->pluginSettings->randomizeSomeAnimations);
    if (this->pluginSettings->graphics < this->ui->comboGraphics->count()) this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    else this->ui->comboGraphics->setCurrentIndex(1); //use original graphics
    this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    this->ui->cbCombineWithOtherGraphicsPacks->setChecked(this->pluginSettings->combineGraphicsWithOtherPacks);
    this->ui->comboPalette->setCurrentIndex(this->pluginSettings->palette);
    this->ui->comboMarioSprite->setCurrentIndex(this->pluginSettings->marioSprite);
    this->ui->comboMarioSpritePalette->setCurrentIndex(this->pluginSettings->marioSpritePalette);
    this->ui->leMarioName->setText(this->pluginSettings->marioName);
    this->ui->leLuigiName->setText(this->pluginSettings->luigiName);
}

void Tab_Base_Game::Save_Settings() {
    QString baseROM = this->ui->comboBaseROM->currentText();
    if (!baseROM.isEmpty() && baseROM != STRING_NO_ROMS_INSTALLED) this->pluginSettings->baseROM = baseROM;
    if (QFileInfo(this->ui->leOutputROMLocation->text()).absoluteDir().exists()) this->pluginSettings->outputROMLocation = this->ui->leOutputROMLocation->text();
    this->pluginSettings->overwriteOuputROM = this->ui->radioOverwriteOutputROM->isChecked();
    this->pluginSettings->addLuigiGame = this->ui->radioAddLuigiGame->isChecked();
    this->pluginSettings->music = this->ui->comboMusic->currentIndex();
    this->pluginSettings->combineMusicWithOtherPacks = this->ui->cbCombineWithOtherMusicPacks->isChecked();
    this->pluginSettings->toneColor = this->ui->comboTone->currentIndex();
    this->pluginSettings->randomSounds = this->ui->cbRandomSoundEffects->isChecked();
    this->pluginSettings->randomText = this->ui->cbRandomText->isChecked();
    this->pluginSettings->randomizeSomeAnimations = this->ui->cbRandomizeSomeAnimations->isChecked();
    this->pluginSettings->graphics = this->ui->comboGraphics->currentIndex();
    this->pluginSettings->combineGraphicsWithOtherPacks = this->ui->cbCombineWithOtherGraphicsPacks->isChecked();
    this->pluginSettings->palette = this->ui->comboPalette->currentIndex();
    this->pluginSettings->marioSprite = this->ui->comboMarioSprite->currentIndex();
    this->pluginSettings->marioSpritePalette = this->ui->comboMarioSpritePalette->currentIndex();
    this->pluginSettings->marioName = this->ui->leMarioName->text();
    this->pluginSettings->luigiName = this->ui->leLuigiName->text();
}

void Tab_Base_Game::Install_New_ROM() {
    if (!this->writerPlugin->Install_ROM().isEmpty()) {
        this->Populate_Installed_ROMs();
    }
}

bool Tab_Base_Game::Is_Partial_Support_Mode_Enabled() {
    return this->partialSupportMode;
}

void Tab_Base_Game::Enable_Partial_Support_Mode(bool enabled) {
    //Reset All Settings to Original
    QSet<int> incompatibleASMDifficulties = {11, 12, 13, 14, 15, 16, 18, 24, 25};
    QSet<int> incompatibleScriptDifficulties = {17, 18, 19, 20, 21, 22, 23, 24, 25};
    bool usingScripts = !this->ui->radioGenerateNewLevels->isChecked();
    if (usingScripts) {
        if (incompatibleScriptDifficulties.contains(this->ui->comboDifficulty->currentIndex())) this->ui->comboDifficulty->setCurrentIndex(4); //set to Normal Difficulty
    }
    this->partialSupportMode = enabled;
    if (enabled) {
        this->Use_Original_Settings();

        if (incompatibleASMDifficulties.contains(this->ui->comboDifficulty->currentIndex())) this->ui->comboDifficulty->setCurrentIndex(4); //set to Normal Difficulty
        this->ui->sbLives->setMaximum(35);
        this->ui->cbGodMode->setChecked(false);
        this->ui->comboPowerup->setCurrentIndex(1); //Fire Flower (Original)

        this->ui->sbAutoScroll->setValue(11);
        this->ui->sbAutoScrollWithFlyingCheepCheeps->setValue(11);
        this->ui->sbWalkingHammerBros->setValue(11);
        this->ui->sbAutoScrollChancePerLevel->setValue(0);
        this->ui->comboBasicEnemySpeed->setCurrentIndex(3);
        this->ui->comboBowserFlameFrequency->setCurrentIndex(2);
        this->ui->comboBulletBillFiringRate->setCurrentIndex(3);
        this->ui->comboBulletBillSpeed->setCurrentIndex(2);
        this->ui->comboEnemyRevivalSpeed->setCurrentIndex(2);
        this->ui->comboLeapingParatroopaSpeed->setCurrentIndex(2);
        this->ui->comboFireBarLength->setCurrentIndex(3);
        this->ui->comboLongFireBarLength->setCurrentIndex(4);
        this->ui->comboFlyingCheepCheepJumpHeight->setCurrentIndex(2);
        this->ui->comboHammerBrosThrowRate->setCurrentIndex(4);
        this->ui->comboLakituRespawnSpeed->setCurrentIndex(3);
        this->ui->comboSurfingLiftSpeed->setCurrentIndex(3);
        this->ui->comboPiranhaPlantType->setCurrentIndex(1);
        this->ui->comboMaxNumberOfPiranhaPlants->setCurrentIndex(5);
        this->ui->comboSecondaryMushroom->setCurrentIndex(3);
        this->ui->comboSpinyEggBehavior->setCurrentIndex(8);
        this->ui->comboReplaceCastleLoops->setCurrentIndex(2);
        this->ui->cbSpeedyObjectsAndEnemies->setChecked(false);
        this->ui->cbHammerBrosNeverJump->setChecked(false);
        this->ui->cbEuropeanBlooperSwimHeight->setChecked(false);
        this->ui->cbFireBowserThrowsALudicrousAmountOfFireballs->setChecked(false);
        this->ui->cbPiranhaPlantsOnFirstLevel->setChecked(false);
        this->ui->cbLakituThrowArc->setChecked(false);
        this->ui->cbRevertToSuperMario->setChecked(false);
        this->ui->cbUnlimitedTime->setChecked(false);

        this->ui->cbNoAutoScrollingLevels->setChecked(true);
    } else {
        this->ui->sbLives->setMaximum(99);
        this->ui->cbGodMode->setChecked(this->pluginSettings->godMode);
        this->ui->comboPowerup->setCurrentIndex(this->pluginSettings->powerup);

        this->ui->cbNoAutoScrollingLevels->setChecked(this->pluginSettings->noAutoScrollingLevels);
    }
    this->ui->groupBaseGameSettings->setEnabled(!enabled);
    this->ui->cbGodMode->setEnabled(!enabled);
    this->ui->lblPowerup->setEnabled(!enabled);
    this->ui->comboPowerup->setEnabled(!enabled);

    //Enable Custom Difficulty Settings
    this->ui->lblAutoScroll->setEnabled(!enabled);
    this->ui->sbAutoScroll->setEnabled(!enabled);
    this->ui->lblAutoScrollWithFlyingCheepCheeps->setEnabled(!enabled);
    this->ui->sbAutoScrollWithFlyingCheepCheeps->setEnabled(!enabled);
    this->ui->layoutAutoScroll->setEnabled(!enabled);
    this->ui->lblWalkingHammerBros->setEnabled(!enabled);
    this->ui->sbWalkingHammerBros->setEnabled(!enabled);
    this->ui->layoutWalkingHammerBros->setEnabled(!enabled);
    this->ui->lblAutoScrollChancePerLevel->setEnabled(!enabled);
    this->ui->sbAutoScrollChancePerLevel->setEnabled(!enabled);
    this->ui->layoutAutoScrollChancePerLevel->setEnabled(!enabled);
    this->ui->lblBasicEnemySpeed->setEnabled(!enabled);
    this->ui->comboBasicEnemySpeed->setEnabled(!enabled);
    this->ui->layoutBasicEnemySpeed->setEnabled(!enabled);
    this->ui->lblBowserFlameFrequency->setEnabled(!enabled);
    this->ui->comboBowserFlameFrequency->setEnabled(!enabled);
    this->ui->layoutBowserFlameFrequency->setEnabled(!enabled);
    this->ui->lblBulletBillFiringRate->setEnabled(!enabled);
    this->ui->comboBulletBillFiringRate->setEnabled(!enabled);
    this->ui->layoutBulletBillFiringRate->setEnabled(!enabled);
    this->ui->lblBulletBillSpeed->setEnabled(!enabled);
    this->ui->comboBulletBillSpeed->setEnabled(!enabled);
    this->ui->layoutBulletBillSpeed->setEnabled(!enabled);
    this->ui->lblEnemyRevivalSpeed->setEnabled(!enabled);
    this->ui->comboEnemyRevivalSpeed->setEnabled(!enabled);
    this->ui->layoutEnemyRevivalSpeed->setEnabled(!enabled);
    this->ui->lblLeapingParatroopaSpeed->setEnabled(!enabled);
    this->ui->comboLeapingParatroopaSpeed->setEnabled(!enabled);
    this->ui->layoutLeapingParatroopaSpeed->setEnabled(!enabled);
    this->ui->lblFireBarLength->setEnabled(!enabled);
    this->ui->comboFireBarLength->setEnabled(!enabled);
    this->ui->layoutFireBarLength->setEnabled(!enabled);
    this->ui->lblLongFireBarLength->setEnabled(!enabled);
    this->ui->comboLongFireBarLength->setEnabled(!enabled);
    this->ui->layoutLongFireBarLength->setEnabled(!enabled);
    this->ui->lblFlyingCheepCheepJumpHeight->setEnabled(!enabled);
    this->ui->comboFlyingCheepCheepJumpHeight->setEnabled(!enabled);
    this->ui->layoutFlyingCheepCheepJumpHeight->setEnabled(!enabled);
    this->ui->lblHammerBrosThrowRate->setEnabled(!enabled);
    this->ui->comboHammerBrosThrowRate->setEnabled(!enabled);
    this->ui->layoutHammerBrosThrowRate->setEnabled(!enabled);
    this->ui->lblLakituRespawnSpeed->setEnabled(!enabled);
    this->ui->comboLakituRespawnSpeed->setEnabled(!enabled);
    this->ui->layoutLakituRespawnSpeed->setEnabled(!enabled);
    this->ui->lblSurfingLiftSpeed->setEnabled(!enabled);
    this->ui->comboSurfingLiftSpeed->setEnabled(!enabled);
    this->ui->layoutSurfingLiftSpeed->setEnabled(!enabled);
    this->ui->lblPiranhaPlantType->setEnabled(!enabled);
    this->ui->comboPiranhaPlantType->setEnabled(!enabled);
    this->ui->layoutPiranhaPlantType->setEnabled(!enabled);
    this->ui->lblMaxNumberOfPiranhaPlants->setEnabled(!enabled);
    this->ui->comboMaxNumberOfPiranhaPlants->setEnabled(!enabled);
    this->ui->layoutMaxNumberOfPiranhaPlants->setEnabled(!enabled);
    this->ui->lblSecondaryMushroom->setEnabled(!enabled);
    this->ui->comboSecondaryMushroom->setEnabled(!enabled);
    this->ui->lblSpinyEggBehavior->setEnabled(!enabled);
    this->ui->comboSpinyEggBehavior->setEnabled(!enabled);
    this->ui->layoutSpinyEggBehavior->setEnabled(!enabled);
    this->ui->lblReplaceCastleLoops->setEnabled(!enabled);
    this->ui->comboReplaceCastleLoops->setEnabled(!enabled);
    this->ui->layoutReplaceCastleLoops->setEnabled(!enabled);
    this->ui->cbSpeedyObjectsAndEnemies->setEnabled(!enabled);
    this->ui->cbHammerBrosNeverJump->setEnabled(!enabled);
    this->ui->cbEuropeanBlooperSwimHeight->setEnabled(!enabled);
    this->ui->cbFireBowserThrowsALudicrousAmountOfFireballs->setEnabled(!enabled);
    this->ui->cbPiranhaPlantsOnFirstLevel->setEnabled(!enabled);
    this->ui->cbLakituThrowArc->setEnabled(!enabled);
    this->ui->cbRevertToSuperMario->setEnabled(!enabled);
    this->ui->cbUnlimitedTime->setEnabled(!enabled);

    this->ui->cbNoAutoScrollingLevels->setEnabled(!enabled);

    //Disable incompatible difficulties
    for (int i = 0; i < this->ui->comboDifficulty->count(); ++i) {
        int value = 33;
        if (enabled) {
            if (usingScripts) {
                if (incompatibleScriptDifficulties.constFind(i) != incompatibleScriptDifficulties.end() || incompatibleASMDifficulties.constFind(i) != incompatibleASMDifficulties.end()) value = 0;
            } else {
                if (incompatibleASMDifficulties.constFind(i) != incompatibleASMDifficulties.end()) value = 0;
            }
        } else {
            if (usingScripts) {
                if (incompatibleScriptDifficulties.constFind(i) != incompatibleScriptDifficulties.end()) value = 0;
            }
        }
        this->ui->comboDifficulty->setItemData(i, value, Qt::UserRole-1);
    }
}

void Tab_Base_Game::Use_Original_Settings() {
    this->ui->radio2PlayerGame->setChecked(true);
    this->ui->comboGraphics->setCurrentIndex(1);
    this->ui->comboPalette->setCurrentIndex(1);
    this->ui->comboMarioSprite->setCurrentIndex(2);
    this->ui->comboMarioSpritePalette->setCurrentIndex(3);
    this->ui->cbCombineWithOtherGraphicsPacks->setChecked(false);
    this->ui->comboMusic->setCurrentIndex(2);
    this->ui->comboTone->setCurrentIndex(1);
    this->ui->cbCombineWithOtherMusicPacks->setChecked(false);
    this->ui->cbRandomSoundEffects->setChecked(false);
    this->ui->cbRandomText->setChecked(false);
    this->ui->cbRandomizeSomeAnimations->setChecked(false);
    this->ui->leMarioName->setText("MARIO");
    this->ui->leLuigiName->setText("LUIGI");
}

void Tab_Base_Game::Use_Basic_Settings() {
    this->ui->radio2PlayerGame->setChecked(true);
    this->ui->comboGraphics->setCurrentIndex(2);
    this->ui->comboPalette->setCurrentIndex(0);
    this->ui->comboMarioSprite->setCurrentIndex(2);
    this->ui->comboMarioSpritePalette->setCurrentIndex(0);
    this->ui->cbCombineWithOtherGraphicsPacks->setChecked(false);
    this->ui->comboMusic->setCurrentIndex(2);
    this->ui->comboTone->setCurrentIndex(0);
    this->ui->cbCombineWithOtherMusicPacks->setChecked(false);
    this->ui->cbRandomSoundEffects->setChecked(false);
    this->ui->cbRandomText->setChecked(true);
    this->ui->cbRandomizeSomeAnimations->setChecked(true);
}

void Tab_Base_Game::Use_Random_Settings() {
    this->ui->radioAddLuigiGame->setChecked(true);
    this->ui->comboGraphics->setCurrentIndex(0);
    this->ui->comboPalette->setCurrentIndex(0);
    this->ui->comboMarioSprite->setCurrentIndex(0);
    this->ui->comboMarioSpritePalette->setCurrentIndex(0);
    this->ui->cbCombineWithOtherGraphicsPacks->setChecked(true);
    this->ui->comboMusic->setCurrentIndex(0);
    this->ui->comboTone->setCurrentIndex(0);
    this->ui->cbCombineWithOtherMusicPacks->setChecked(true);
    this->ui->cbRandomSoundEffects->setChecked(true);
    this->ui->cbRandomText->setChecked(true);
    this->ui->cbRandomizeSomeAnimations->setChecked(true);
}

void Tab_Base_Game::Populate_Installed_ROMs() {
    this->ui->comboBaseROM->clear();
    QStringList installedROMs = this->writerPlugin->Get_Installed_ROMs();
    QString tmpBaseROM = this->pluginSettings->baseROM;
    if (installedROMs.isEmpty()) {
        this->ui->comboBaseROM->addItem(STRING_NO_ROMS_INSTALLED);
    } else {
        this->ui->comboBaseROM->addItems(installedROMs);
    }
    if (tmpBaseROM != "" && installedROMs.contains(tmpBaseROM)) {
        this->ui->comboBaseROM->setCurrentText(tmpBaseROM);
    } else {
        this->ui->comboBaseROM->setCurrentIndex(0);
    }
}

void Tab_Base_Game::Populate_Graphics_Packs() {
    QStringList packsWithExtension = this->writerPlugin->Graphics_Get_Graphics_Packs();
    packsWithExtension += this->writerPlugin->Graphics_Get_Bonus_Graphics_Packs();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboGraphics->addItems(packs);
}

void Tab_Base_Game::Populate_Mario_Sprites() {
    QStringList packsWithExtension = this->writerPlugin->Graphics_Get_Mario_Sprites();
    packsWithExtension += this->writerPlugin->Graphics_Get_Bonus_Mario_Sprites();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboMarioSprite->addItems(packs);
}

void Tab_Base_Game::Populate_Music_Packs() {
    QStringList packsWithExtension = this->writerPlugin->Music_Get_Music_Packs();
    packsWithExtension += this->writerPlugin->Music_Get_Bonus_Music_Packs();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboMusic->addItems(packs);
}
