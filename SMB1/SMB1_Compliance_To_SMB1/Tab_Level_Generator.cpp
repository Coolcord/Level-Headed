#include "Tab_Level_Generator.h"
#include "ui_Configure_Settings_Form.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include "Configure_Settings_Form.h"
#include "Plugin_Settings.h"
#include "Tab_Base_Game.h"
#include <QDir>
#include <QMessageBox>
#include <QString>

Tab_Level_Generator::Tab_Level_Generator(QWidget *p, const QString &apl, SMB1_Writer_Interface *wp, Ui::Configure_Settings_Form *u, Plugin_Settings *ps, Tab_Base_Game *tabBaseGame)
    : Tab_Interface(p, apl, wp, u, ps) {
    assert(tabBaseGame); this->tabBaseGame = tabBaseGame;
    //Get the Level Scripts Location
    this->levelLocation = this->applicationLocation + "/" + Common_Strings::STRING_LEVELS + "/" + Common_Strings::STRING_GAME_NAME;
    QDir dir(this->applicationLocation + "/" + Common_Strings::STRING_LEVELS);
    if (!dir.exists(this->levelLocation)) dir.mkdir(this->levelLocation); //don't bother checking for success here

    this->Populate_Chance_ComboBoxes();
    this->Populate_Level_Scripts_ComboBox();
}

void Tab_Level_Generator::Load_Settings() {
    this->Load_Settings(this->pluginSettings);
}

void Tab_Level_Generator::Load_Settings(Plugin_Settings *ps) {
    if (ps->generateNewLevels) {
        this->ui->radioGenerateNewLevels->setChecked(true);
    } else {
        this->ui->radioUseLevelScripts->setChecked(true);
        if (QFileInfo::exists(this->applicationLocation + "/" + Common_Strings::STRING_LEVELS + "/" + Common_Strings::STRING_GAME_NAME + "/" + ps->levelScripts + Common_Strings::STRING_LEVELS_EXTENSION)) {
            this->ui->comboLevelScripts->setCurrentText(ps->levelScripts);
        } else {
            this->ui->comboLevelScripts->setCurrentIndex(0);
        }
    }
    this->ui->cbRedistributePowerups->setChecked(ps->redistributePowerups);
    this->ui->cbRandomizeWarpZones->setChecked(ps->randomizeWarpZones);
    this->ui->comboLevelScriptEnemies->setCurrentIndex(ps->levelScriptEnemies);
    if (this->ui->cbNoAutoScrollingLevels->isEnabled()) this->ui->cbNoAutoScrollingLevels->setChecked(ps->noAutoScrollingLevels);
    if (this->ui->cbNoPipeIntroCutscenes->isEnabled()) this->ui->cbNoPipeIntroCutscenes->setChecked(ps->noPipeIntroCutscenes);
    if (ps->standardLevelDistribution) this->ui->radioStandardLevelDistribution->setChecked(true);
    else this->ui->radioRandomLevelDistribution->setChecked(true);
    this->Enable_Standard_Level_Distribution_Options(ps->standardLevelDistribution);
    this->ui->cbIncludeStandardOverworldLevels->setChecked(ps->includeStandardOverworldLevelsInRandomDistribution);
    this->ui->cbIncludeUndergroundLevels->setChecked(ps->includeUndergroundLevelsInRandomDistribution);
    this->ui->cbIncludeUnderwaterLevels->setChecked(ps->includeUnderwaterLevelsInRandomDistribution);
    this->ui->cbIncludeBridgeLevels->setChecked(ps->includeBridgeLevelsInRandomDistribution);
    this->ui->cbIncludeIslandLevels->setChecked(ps->includeIslandLevelsInRandomDistribution);
    this->ui->leRandomSeed->setText(ps->randomSeed);
    this->ui->cbRandomNumWorlds->setChecked(ps->randomNumWorlds);
    this->Load_Spin_Box_Value(this->ui->sbNumWorlds, ps->numWorlds);
    this->Load_Spin_Box_Value(this->ui->sbNumLevelsPerWorld, ps->numLevelsPerWorld);
    this->Enable_Random_Number_Of_Worlds(ps->randomNumWorlds);
    this->Load_Level_Commonality_Combo_Box(this->ui->comboStandardOverworld, ps->standardOverworldChance);
    this->Load_Level_Commonality_Combo_Box(this->ui->comboUnderground, ps->undergroundChance);
    this->Load_Level_Commonality_Combo_Box(this->ui->comboUnderwater, ps->underwaterChance);
    this->Load_Level_Commonality_Combo_Box(this->ui->comboBridge, ps->bridgeChance);
    this->Load_Level_Commonality_Combo_Box(this->ui->comboIsland, ps->islandChance);
    this->Enable_New_Level_Options(ps->generateNewLevels);
}

void Tab_Level_Generator::Save_Settings() {
    this->Save_Settings(this->pluginSettings);
}

void Tab_Level_Generator::Save_Settings(Plugin_Settings *ps) {
    ps->generateNewLevels = this->ui->radioGenerateNewLevels->isChecked();
    ps->randomSeed = this->ui->leRandomSeed->text();
    ps->levelScripts = this->ui->comboLevelScripts->currentText();
    ps->redistributePowerups = this->ui->cbRedistributePowerups->isChecked();
    ps->randomizeWarpZones = this->ui->cbRandomizeWarpZones->isChecked();
    ps->levelScriptEnemies = this->ui->comboLevelScriptEnemies->currentIndex();
    if (this->ui->cbNoAutoScrollingLevels->isEnabled()) ps->noAutoScrollingLevels = this->ui->cbNoAutoScrollingLevels->isChecked();
    if (this->ui->cbNoPipeIntroCutscenes->isEnabled()) ps->noPipeIntroCutscenes = this->ui->cbNoPipeIntroCutscenes->isChecked();
    ps->standardLevelDistribution = this->ui->radioStandardLevelDistribution->isChecked();
    ps->randomNumWorlds = this->ui->cbRandomNumWorlds->isChecked();
    ps->numWorlds = this->ui->sbNumWorlds->value();
    ps->numLevelsPerWorld = this->ui->sbNumLevelsPerWorld->value();
    ps->standardOverworldChance = this->ui->comboStandardOverworld->currentText();
    ps->undergroundChance = this->ui->comboUnderground->currentText();
    ps->underwaterChance = this->ui->comboUnderwater->currentText();
    ps->bridgeChance = this->ui->comboBridge->currentText();
    ps->islandChance = this->ui->comboIsland->currentText();
    ps->includeStandardOverworldLevelsInRandomDistribution = this->ui->cbIncludeStandardOverworldLevels->isChecked();
    ps->includeUndergroundLevelsInRandomDistribution = this->ui->cbIncludeUndergroundLevels->isChecked();
    ps->includeUnderwaterLevelsInRandomDistribution = this->ui->cbIncludeUnderwaterLevels->isChecked();
    ps->includeBridgeLevelsInRandomDistribution = this->ui->cbIncludeBridgeLevels->isChecked();
    ps->includeIslandLevelsInRandomDistribution = this->ui->cbIncludeIslandLevels->isChecked();
}

void Tab_Level_Generator::Clear_All_Random_Level_Scripts() {
    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                   "This will delete all of your previously generated random level packs! Are you sure that you want to do this?",
                                   QMessageBox::Yes | QMessageBox::No);

    //Delete all random level scripts
    if (answer == QMessageBox::Yes) {
        QDir dir(this->levelLocation);
        bool success = true;
        if (dir.exists()) {
            //Scan for valid level scripts
            QStringList filters; filters.append("*"+Common_Strings::STRING_LEVELS_EXTENSION);
            QStringList levelFolders = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
            foreach (QString level, levelFolders) {
                if (level.size() == 33 && level.startsWith("Random ")) {
                    QFile levelFile(dir.path() + "/" + level);
                    if (!levelFile.remove()) success = false;
                }
            }
        }

        //Show a message for the status of the operation
        if (success) {
            QMessageBox::information(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                     "All previously generated random level packs have been removed!",
                                     QMessageBox::Ok);
        } else {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, Common_Strings::STRING_LEVEL_HEADED +
                                  " could not delete all of the randomly generated level packs!",
                                  QMessageBox::Ok);
        }
        this->Populate_Level_Scripts_ComboBox();
        this->Save_Settings();
    }
}

void Tab_Level_Generator::Clear_Chance_ComboBoxes() {
    this->ui->comboDifficulty->setCurrentIndex(0);
    this->ui->comboStandardOverworld->clear();
    this->ui->comboUnderground->clear();
    this->ui->comboUnderwater->clear();
    this->ui->comboBridge->clear();
    this->ui->comboIsland->clear();
}

void Tab_Level_Generator::Populate_Chance_ComboBoxes() {
    this->Populate_Chance_ComboBox(this->ui->comboStandardOverworld);
    this->Populate_Chance_ComboBox(this->ui->comboUnderground);
    this->Populate_Chance_ComboBox(this->ui->comboUnderwater);
    this->Populate_Chance_ComboBox(this->ui->comboBridge);
    this->Populate_Chance_ComboBox(this->ui->comboIsland);
}

void Tab_Level_Generator::Populate_Chance_ComboBox(QComboBox *comboBox) {
    assert(comboBox);
    comboBox->clear();
    comboBox->addItem(STRING_VERY_COMMON);
    comboBox->addItem(STRING_COMMON);
    comboBox->addItem(STRING_UNCOMMON);
    comboBox->addItem(STRING_RARE);
    comboBox->addItem(STRING_VERY_RARE);
    comboBox->addItem(STRING_MYTHIC);
    comboBox->addItem(STRING_NONE);
}

void Tab_Level_Generator::Populate_Level_Scripts_ComboBox() {
    this->ui->comboLevelScripts->clear();
    QDir dir(this->levelLocation);
    if (!dir.exists()) {
        this->ui->comboLevelScripts->addItem(STRING_NO_LEVEL_SCRIPTS_FOUND);
        return;
    }

    //Scan for valid level scripts
    QStringList filters; filters.append("*"+Common_Strings::STRING_LEVELS_EXTENSION);
    QStringList levelFolders = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    QStringList validLevelFolders;
    QStringList validTechDemoFolders;
    QStringList validRandomFolders;
    for (QString level : levelFolders) {
        level.chop(Common_Strings::STRING_LEVELS_EXTENSION.size());
        if (level.startsWith("Random ")) validRandomFolders.append(level);
        else if (level.startsWith("(Tech Demo)")) validTechDemoFolders.append(level);
        else validLevelFolders.append(level);
    }

    //Add the valid folders to the ComboBox
    if (validLevelFolders.isEmpty() && validRandomFolders.isEmpty() && validTechDemoFolders.isEmpty()) {
        this->ui->comboLevelScripts->addItem(STRING_NO_LEVEL_SCRIPTS_FOUND);
    } else {
        if (!validLevelFolders.isEmpty()) this->ui->comboLevelScripts->addItems(validLevelFolders);
        if (!validTechDemoFolders.isEmpty()) this->ui->comboLevelScripts->addItems(validTechDemoFolders);
        if (!validRandomFolders.isEmpty()) this->ui->comboLevelScripts->addItems(validRandomFolders);
    }

    //Use the last selected level scripts
    if (validLevelFolders.contains(this->pluginSettings->levelScripts)
            || validRandomFolders.contains(this->pluginSettings->levelScripts)
            || validTechDemoFolders.contains(this->pluginSettings->levelScripts)) {
        this->ui->comboLevelScripts->setCurrentText(this->pluginSettings->levelScripts);
    } else {
        this->ui->comboLevelScripts->setCurrentIndex(0);
    }
}

bool Tab_Level_Generator::At_Least_One_Very_Common_Selected() {
    if (!this->ui->radioGenerateNewLevels->isChecked()) return true;
    if (this->ui->comboStandardOverworld->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboUnderground->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboUnderwater->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboBridge->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboIsland->currentText() == STRING_VERY_COMMON) return true;
    return false;
}

void Tab_Level_Generator::Enable_New_Level_Options(bool enable) {
    if (enable) {
        this->ui->layoutGenerateNewLevelsWidget->show();
        this->ui->layoutLevelScriptsWidget->hide();
    } else {
        this->ui->layoutGenerateNewLevelsWidget->hide();
        this->ui->layoutLevelScriptsWidget->show();
    }
    this->tabBaseGame->Enable_Partial_Support_Mode(this->tabBaseGame->Is_Partial_Support_Mode_Enabled());
}

void Tab_Level_Generator::Enable_Random_Number_Of_Worlds(bool enable) {
    if (enable) this->ui->layoutNumWorldsWidget->hide();
    else this->ui->layoutNumWorldsWidget->show();
}

void Tab_Level_Generator::Enable_Standard_Level_Distribution_Options(bool enable) {
    if (enable) {
        this->ui->uniformLevelDistributionWidget->show();
        this->ui->randomLevelDistributionWidget->hide();
    } else {
        this->ui->uniformLevelDistributionWidget->hide();
        this->ui->randomLevelDistributionWidget->show();
    }
}

void Tab_Level_Generator::Update_Worlds() {
    int numWorlds = this->ui->sbNumWorlds->value();
    int numLevelsPerWorld = this->ui->sbNumLevelsPerWorld->value();
    while (numLevelsPerWorld*numWorlds > 20) {
        --numLevelsPerWorld;
    }
    this->ui->sbNumLevelsPerWorld->setValue(numLevelsPerWorld);
}

void Tab_Level_Generator::Use_Default_Settings() {
    this->ui->radioStandardLevelDistribution->setChecked(true);
    this->ui->comboStandardOverworld->setCurrentText(STRING_VERY_COMMON);
    this->ui->comboUnderground->setCurrentText(STRING_COMMON);
    this->ui->comboUnderwater->setCurrentText(STRING_UNCOMMON);
    this->ui->comboBridge->setCurrentText(STRING_COMMON);
    this->ui->comboIsland->setCurrentText(STRING_COMMON);
}

void Tab_Level_Generator::Load_Spin_Box_Value(QSpinBox *spinBox, int &value) {
    if (value < spinBox->minimum()) value = spinBox->minimum();
    if (value > spinBox->maximum()) value = spinBox->maximum();
    spinBox->setValue(value);
}

void Tab_Level_Generator::Load_Level_Commonality_Combo_Box(QComboBox *comboBox, QString &value) {
    comboBox->setCurrentText(value);
    if (comboBox->currentText().isEmpty()) {
        comboBox->setCurrentIndex(0);
        value = comboBox->currentText();
    }
}
