#include "Tab_Base_Game.h"
#include "Configure_Settings_Form.h"
#include "ui_Configure_Settings_Form.h"
#include <QDir>
#include <QFileInfo>

Tab_Base_Game::Tab_Base_Game(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Ui::Configure_Settings_Form *ui, Plugin_Settings *pluginSettings)
    : Tab_Interface(parent, applicationLocation, writerPlugin, ui, pluginSettings) {
    this->Populate_Installed_ROMs();
    this->Populate_Graphics_Packs();
    this->Populate_Music_Packs();
}

void Tab_Base_Game::Load_Settings() {
    if (!this->pluginSettings->baseROM.isEmpty()) this->ui->comboBaseROM->setCurrentText(this->pluginSettings->baseROM);
    this->ui->leOutputROMLocation->setText(this->pluginSettings->outputROMLocation);
    if (this->pluginSettings->overwriteOuputROM) this->ui->radioOverwriteOutputROM->setChecked(true);
    else this->ui->radioAppendNumberToFilename->setChecked(true);
    this->ui->cbModifyOnlyLevels->setChecked(this->pluginSettings->modifyOnlyLevels);
    if (this->pluginSettings->addLuigiGame) this->ui->radioAddLuigiGame->setChecked(true);
    else this->ui->radio2PlayerGame->setChecked(true);
    if (this->pluginSettings->music < this->ui->comboMusic->count()) this->ui->comboMusic->setCurrentIndex(this->pluginSettings->music);
    else this->ui->comboMusic->setCurrentIndex(2); //use original music
    this->ui->cbCombineWithOtherMusicPacks->setChecked(this->pluginSettings->combineMusicWithOtherPacks);
    this->ui->comboTone->setCurrentIndex(this->pluginSettings->toneColor);
    if (this->pluginSettings->graphics < this->ui->comboGraphics->count()) this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    else this->ui->comboGraphics->setCurrentIndex(1); //use original graphics
    this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    this->ui->comboPowerup->setCurrentIndex(this->pluginSettings->powerup);
    this->ui->comboSecondaryMushroom->setCurrentIndex(this->pluginSettings->secondaryMushroom);
}

void Tab_Base_Game::Save_Settings() {
    QString baseROM = this->ui->comboBaseROM->currentText();
    if (!baseROM.isEmpty() && baseROM != STRING_NO_ROMS_INSTALLED) this->pluginSettings->baseROM = baseROM;
    if (QFileInfo(this->ui->leOutputROMLocation->text()).absoluteDir().exists()) this->pluginSettings->outputROMLocation = this->ui->leOutputROMLocation->text();
    this->pluginSettings->overwriteOuputROM = this->ui->radioOverwriteOutputROM->isChecked();
    this->pluginSettings->modifyOnlyLevels = this->ui->cbModifyOnlyLevels->isChecked();
    this->pluginSettings->addLuigiGame = this->ui->radioAddLuigiGame->isChecked();
    this->pluginSettings->music = this->ui->comboMusic->currentIndex();
    this->pluginSettings->combineMusicWithOtherPacks = this->ui->cbCombineWithOtherMusicPacks->isChecked();
    this->pluginSettings->toneColor = this->ui->comboTone->currentIndex();
    this->pluginSettings->graphics = this->ui->comboGraphics->currentIndex();
    this->pluginSettings->powerup = this->ui->comboPowerup->currentIndex();
    this->pluginSettings->secondaryMushroom = this->ui->comboSecondaryMushroom->currentIndex();
}

void Tab_Base_Game::Install_New_ROM() {
    if (!this->writerPlugin->Install_ROM().isEmpty()) {
        this->Populate_Installed_ROMs();
    }
}

void Tab_Base_Game::Modify_Only_Levels(bool enabled) {
    if (enabled) {
        this->ui->radio2PlayerGame->setChecked(true);
        this->ui->comboGraphics->setCurrentIndex(1);
        this->ui->comboMusic->setCurrentIndex(2);
        this->ui->comboTone->setCurrentIndex(1);
        this->ui->cbCombineWithOtherMusicPacks->setChecked(true);
        this->ui->comboPowerup->setCurrentIndex(1);
        this->ui->comboSecondaryMushroom->setCurrentIndex(2);
    }
    this->ui->layoutNonLevelsWidget->setEnabled(!enabled);
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
    QStringList packsWithExtension = this->writerPlugin->Graphics_Get_Graphic_Packs();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboGraphics->addItems(packs);
}

void Tab_Base_Game::Populate_Music_Packs() {
    QStringList packsWithExtension = this->writerPlugin->Music_Get_Music_Packs();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboMusic->addItems(packs);
}
