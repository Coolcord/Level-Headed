#include "Tab_Level_Generator.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include "Configure_Settings_Form.h"
#include "ui_Configure_Settings_Form.h"
#include <QDir>
#include <QMessageBox>
#include <QString>

Tab_Level_Generator::Tab_Level_Generator(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Ui::Configure_Settings_Form *ui, Plugin_Settings *pluginSettings)
    : Tab_Interface(parent, applicationLocation, writerPlugin, ui, pluginSettings) {
    this->numWorlds = this->pluginSettings->numWorlds;
    this->numLevelsPerWorld = this->pluginSettings->numLevelsPerWorld;
    this->randomNumWorlds = this->pluginSettings->randomNumWorlds;

    //Get the Level Scripts Location
    this->levelLocation = this->applicationLocation + "/" + Common_Strings::STRING_LEVELS + "/" + Common_Strings::STRING_GAME_NAME;
    QDir dir(this->applicationLocation + "/" + Common_Strings::STRING_LEVELS);
    if (!dir.exists(this->levelLocation)) dir.mkdir(this->levelLocation); //don't bother checking for success here

    this->Populate_Chance_ComboBoxes();
    this->Populate_Level_Scripts_ComboBox();
}

void Tab_Level_Generator::Load_Settings() {
    if (this->pluginSettings->generateNewLevels) this->ui->radioGenerateNewLevels->setChecked(true);
    else this->ui->radioUseLevelScripts->setChecked(true);
    this->ui->sbRandomSeed->setValue(this->pluginSettings->randomSeed);
    this->ui->cbRandomNumWorlds->setChecked(this->pluginSettings->randomNumWorlds);
    this->Enable_Random_Number_Of_Worlds(this->pluginSettings->randomNumWorlds);
    this->ui->comboStandardOverworld->setCurrentText(this->pluginSettings->standardOverworldChance);
    this->ui->comboUnderground->setCurrentText(this->pluginSettings->undergroundChance);
    this->ui->comboUnderwater->setCurrentText(this->pluginSettings->underwaterChance);
    this->ui->comboBridge->setCurrentText(this->pluginSettings->bridgeChance);
    this->ui->comboIsland->setCurrentText(this->pluginSettings->islandChance);
    this->Enable_New_Level_Options(this->pluginSettings->generateNewLevels);
}

void Tab_Level_Generator::Save_Settings() {
    this->pluginSettings->generateNewLevels = this->ui->radioGenerateNewLevels->isChecked();
    if (!this->pluginSettings->generateNewLevels) {
        this->pluginSettings->levelScripts = this->ui->comboLevelScripts->currentText();
    } else {
        this->pluginSettings->randomNumWorlds = this->randomNumWorlds;
        if (!this->randomNumWorlds) {
            this->pluginSettings->numWorlds = this->ui->sbNumWorlds->value();
            this->pluginSettings->numLevelsPerWorld = this->ui->sbNumLevelsPerWorld->value();
        }
        this->pluginSettings->standardOverworldChance = this->ui->comboStandardOverworld->currentText();
        this->pluginSettings->undergroundChance = this->ui->comboUnderground->currentText();
        this->pluginSettings->underwaterChance = this->ui->comboUnderwater->currentText();
        this->pluginSettings->bridgeChance = this->ui->comboBridge->currentText();
        this->pluginSettings->islandChance = this->ui->comboIsland->currentText();
        this->pluginSettings->randomSeed = this->ui->sbRandomSeed->value();
    }
}

void Tab_Level_Generator::Clear_All_Random_Level_Scripts() {
    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                   "This will delete all of your previously generated random level scripts! Are you sure that you want to do this?",
                                   QMessageBox::Yes | QMessageBox::No);

    //Delete all random level scripts
    if (answer == QMessageBox::Yes) {
        QDir dir(this->levelLocation);
        bool success = true;
        if (dir.exists()) {
            //Scan for valid level scripts
            QStringList levelFolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
            foreach (QString level, levelFolders) {
                if (level.size() == 28 && level.startsWith("Random ")) {
                    QDir levelDir(dir.path() + "/" + level);
                    if (!levelDir.removeRecursively()) success = false;
                }
            }
        }

        //Show a message for the status of the operation
        if (success) {
            QMessageBox::information(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                     "All previously generated random level scripts have been removed!",
                                     Common_Strings::STRING_OK);
        } else {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, Common_Strings::STRING_LEVEL_HEADED +
                                  " could not delete all of the randomly generated level scripts!",
                                  Common_Strings::STRING_OK);
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
    QStringList levelFolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    QStringList validLevelFolders;
    QStringList validRandomFolders;
    foreach (QString level, levelFolders) {
        if (!dir.cd(level)) continue;
        if (dir.exists(level + ".map")) {
            if (level.startsWith("Random ")) validRandomFolders.append(level);
            else validLevelFolders.append(level);
        }
        if (!dir.cdUp()) {
            this->ui->comboLevelScripts->addItem(STRING_NO_LEVEL_SCRIPTS_FOUND);
            return; //this shouldn't happen unless the parent directory is removed
        }
    }

    //Add the valid folders to the ComboBox
    if (validLevelFolders.isEmpty() && validRandomFolders.isEmpty()) {
        this->ui->comboLevelScripts->addItem(STRING_NO_LEVEL_SCRIPTS_FOUND);
    } else {
        if (!validLevelFolders.isEmpty()) this->ui->comboLevelScripts->addItems(validLevelFolders);
        if (!validRandomFolders.isEmpty()) this->ui->comboLevelScripts->addItems(validRandomFolders);
    }

    //Use the last selected level scripts
    if (validLevelFolders.contains(this->pluginSettings->levelScripts) || validRandomFolders.contains(this->pluginSettings->levelScripts)) {
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

    /*
    //Toggle the Level Scripts
    this->ui->lblLevelScripts->setEnabled(!enable);
    this->ui->comboLevelScripts->setEnabled(!enable);
    this->ui->btnClearAllRandomLevelScripts->setEnabled(!enable);

    //Toggle the checkboxes
    this->ui->cbRandomNumWorlds->setEnabled(enable);
    if (enable) this->ui->cbRandomNumWorlds->setChecked(this->randomNumWorlds);
    else this->ui->cbRandomNumWorlds->setChecked(false);

    //Toggle the spinboxes
    this->ui->lblNumWorlds->setEnabled(enable && !this->randomNumWorlds);
    this->ui->sbNumWorlds->setEnabled(enable && !this->randomNumWorlds);
    if (enable && !this->randomNumWorlds) { //reset the values of the spinboxes
        this->ui->sbNumWorlds->setValue(this->pluginSettings->numWorlds);
        this->ui->sbNumLevelsPerWorld->setValue(this->pluginSettings->numLevelsPerWorld);
    } else {
        this->ui->sbNumWorlds->clear();
        this->ui->sbNumLevelsPerWorld->clear();
    }
    this->ui->lblNumLevelsPerWorld->setEnabled(enable && !this->randomNumWorlds);
    this->ui->sbNumLevelsPerWorld->setEnabled(enable && !this->randomNumWorlds);


    //Toggle the comboboxes
    this->ui->lblDifficulty->setEnabled(enable);
    this->ui->lblStandardOverworld->setEnabled(enable);
    this->ui->lblUnderground->setEnabled(enable);
    this->ui->lblUnderwater->setEnabled(enable);
    this->ui->lblBridge->setEnabled(enable);
    this->ui->lblIsland->setEnabled(enable);
    this->ui->comboDifficulty->setEnabled(enable);
    this->ui->comboStandardOverworld->setEnabled(enable);
    this->ui->comboUnderground->setEnabled(enable);
    this->ui->comboUnderwater->setEnabled(enable);
    this->ui->comboBridge->setEnabled(enable);
    this->ui->comboIsland->setEnabled(enable);

    //Toggle the random seed
    this->ui->lblRandomSeed->setEnabled(enable);
    this->ui->sbRandomSeed->setEnabled(enable);
    this->ui->btnNewRandomSeed->setEnabled(enable);
    if (enable) this->ui->sbRandomSeed->setValue(this->pluginSettings->randomSeed);
    else this->ui->sbRandomSeed->clear();

    //Populate the appropriate ComboBoxes
    if (enable) {
        this->ui->comboLevelScripts->clear();
        this->Populate_Chance_ComboBoxes();
    } else {
        this->Clear_Chance_ComboBoxes();
        this->Populate_Level_Scripts_ComboBox();
    }*/
}

void Tab_Level_Generator::Enable_Random_Number_Of_Worlds(bool enable) {
    this->ui->lblNumWorlds->setEnabled(!enable);
    this->ui->sbNumWorlds->setEnabled(!enable);
    this->ui->lblNumLevelsPerWorld->setEnabled(!enable);
    this->ui->sbNumLevelsPerWorld->setEnabled(!enable);
    if (enable) {
        this->ui->sbNumWorlds->clear();
        this->ui->sbNumLevelsPerWorld->clear();
    } else {
        this->ui->sbNumWorlds->setValue(this->numWorlds);
        this->ui->sbNumLevelsPerWorld->setValue(this->numLevelsPerWorld);
    }
    this->randomNumWorlds = enable;
}

void Tab_Level_Generator::Update_Worlds() {
    int numWorlds = this->ui->sbNumWorlds->value();
    int numLevelsPerWorld = this->ui->sbNumLevelsPerWorld->value();
    while (numLevelsPerWorld*numWorlds > 20) {
        --numLevelsPerWorld;
    }
    this->ui->sbNumLevelsPerWorld->setValue(numLevelsPerWorld);
    this->numWorlds = numWorlds;
}
