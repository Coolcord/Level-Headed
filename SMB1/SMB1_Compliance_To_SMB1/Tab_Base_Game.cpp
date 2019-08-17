#include "Tab_Base_Game.h"
#include "Configure_Settings_Form.h"
#include "ui_Configure_Settings_Form.h"
#include "../SMB1_Writer/ROM_Filename.h"
#include <QDir>
#include <QFileInfo>
#include <QVector>

Tab_Base_Game::Tab_Base_Game(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Ui::Configure_Settings_Form *ui, Plugin_Settings *pluginSettings)
    : Tab_Interface(parent, applicationLocation, writerPlugin, ui, pluginSettings) {
    this->Populate_Installed_ROMs();
    this->Populate_Graphics_Packs();
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
    if (this->pluginSettings->graphics < this->ui->comboGraphics->count()) this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    else this->ui->comboGraphics->setCurrentIndex(1); //use original graphics
    this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    this->ui->comboPowerup->setCurrentIndex(this->pluginSettings->powerup);
    this->ui->comboSecondaryMushroom->setCurrentIndex(this->pluginSettings->secondaryMushroom);
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
    this->pluginSettings->graphics = this->ui->comboGraphics->currentIndex();
    this->pluginSettings->powerup = this->ui->comboPowerup->currentIndex();
    this->pluginSettings->secondaryMushroom = this->ui->comboSecondaryMushroom->currentIndex();
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
    QVector<int> asmDifficulties = {9, 11, 12, 13, 15, 20, 21, 22};
    this->partialSupportMode = enabled;
    if (enabled) {
        this->Use_Original_Settings();

        if (asmDifficulties.contains(this->ui->comboDifficulty->currentIndex())) this->ui->comboDifficulty->setCurrentIndex(4); //set to Normal Difficulty
        if (!this->ui->radioStartingLives->isChecked()) {
            this->ui->radioStartingLives->setChecked(true);
            this->ui->sbLives->setValue(this->pluginSettings->numLives);
        }
        this->ui->sbLives->setMaximum(35);
        this->ui->cbGodMode->setChecked(false);

        this->ui->sbAutoScroll->setValue(11);
        this->ui->sbWalkingHammerBros->setValue(11);
        this->ui->sbAutoScrollChancePerLevel->setValue(0);
        this->ui->comboBasicEnemySpeed->setCurrentIndex(1);
        this->ui->comboBulletBillFiringRate->setCurrentIndex(3);
        this->ui->comboBulletBillSpeed->setCurrentIndex(2);
        this->ui->comboFlyingCheepCheepJumpHeight->setCurrentIndex(2);
        this->ui->comboHammerBrosThrowRate->setCurrentIndex(4);
        this->ui->comboLakituRespawnSpeed->setCurrentIndex(3);
        this->ui->comboPiranhaPlantType->setCurrentIndex(1);
        this->ui->comboSpinyEggBehavior->setCurrentIndex(7);
        this->ui->comboReplaceCastleLoops->setCurrentIndex(1);
        this->ui->cbSpeedyObjectsAndEnemies->setChecked(false);
        this->ui->cbLakituThrowArc->setChecked(false);
        this->ui->cbRevertToSuperMario->setChecked(false);
    } else {
        this->ui->sbLives->setMaximum(99);
    }
    this->ui->groupBaseGameSettings->setEnabled(!enabled);
    this->ui->radioInfiniteLives->setEnabled(!enabled);
    this->ui->radioPermadeath->setEnabled(!enabled);
    this->ui->cbGodMode->setEnabled(!enabled);

    //Enable Custom Difficulty Settings
    this->ui->lblAutoScroll->setEnabled(!enabled);
    this->ui->sbAutoScroll->setEnabled(!enabled);
    this->ui->lblWalkingHammerBros->setEnabled(!enabled);
    this->ui->sbWalkingHammerBros->setEnabled(!enabled);
    this->ui->lblAutoScrollChancePerLevel->setEnabled(!enabled);
    this->ui->sbAutoScrollChancePerLevel->setEnabled(!enabled);
    this->ui->lblBasicEnemySpeed->setEnabled(!enabled);
    this->ui->comboBasicEnemySpeed->setEnabled(!enabled);
    this->ui->lblBulletBillFiringRate->setEnabled(!enabled);
    this->ui->comboBulletBillFiringRate->setEnabled(!enabled);
    this->ui->lblBulletBillSpeed->setEnabled(!enabled);
    this->ui->comboBulletBillSpeed->setEnabled(!enabled);
    this->ui->lblLakituRespawnSpeed->setEnabled(!enabled);
    this->ui->comboLakituRespawnSpeed->setEnabled(!enabled);
    this->ui->lblPiranhaPlantType->setEnabled(!enabled);
    this->ui->comboPiranhaPlantType->setEnabled(!enabled);
    this->ui->lblSpinyEggBehavior->setEnabled(!enabled);
    this->ui->comboSpinyEggBehavior->setEnabled(!enabled);
    this->ui->layoutReplaceCastleLoops->setEnabled(!enabled);
    this->ui->lblReplaceCastleLoops->setEnabled(!enabled);
    this->ui->comboReplaceCastleLoops->setEnabled(!enabled);
    this->ui->cbSpeedyObjectsAndEnemies->setEnabled(!enabled);
    this->ui->cbLakituThrowArc->setEnabled(!enabled);
    this->ui->cbRevertToSuperMario->setEnabled(!enabled);

    //Disable the ASM Difficulty Presents
    for (int i = 0; i < asmDifficulties.size(); ++i) {
        int value = 33;
        if (enabled) value = 0;
        this->ui->comboDifficulty->setItemData(asmDifficulties.at(i), value, Qt::UserRole-1);
    }
}

void Tab_Base_Game::Use_Original_Settings() {
    this->ui->radio2PlayerGame->setChecked(true);
    this->ui->comboGraphics->setCurrentIndex(1);
    this->ui->comboMusic->setCurrentIndex(2);
    this->ui->comboTone->setCurrentIndex(1);
    this->ui->cbCombineWithOtherMusicPacks->setChecked(false);
    this->ui->cbRandomSoundEffects->setChecked(false);
    this->ui->comboPowerup->setCurrentIndex(1);
    this->ui->comboSecondaryMushroom->setCurrentIndex(2);
    this->ui->leMarioName->setText("MARIO");
    this->ui->leLuigiName->setText("LUIGI");
}

void Tab_Base_Game::Use_Random_Settings() {
    this->ui->radioAddLuigiGame->setChecked(true);
    this->ui->comboGraphics->setCurrentIndex(0);
    this->ui->comboMusic->setCurrentIndex(0);
    this->ui->comboTone->setCurrentIndex(0);
    this->ui->cbCombineWithOtherMusicPacks->setChecked(true);
    this->ui->cbRandomSoundEffects->setChecked(true);
    this->ui->comboPowerup->setCurrentIndex(0);
    this->ui->comboSecondaryMushroom->setCurrentIndex(0);
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

void Tab_Base_Game::Populate_Music_Packs() {
    QStringList packsWithExtension = this->writerPlugin->Music_Get_Music_Packs();
    packsWithExtension += this->writerPlugin->Music_Get_Bonus_Music_Packs();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboMusic->addItems(packs);
}
