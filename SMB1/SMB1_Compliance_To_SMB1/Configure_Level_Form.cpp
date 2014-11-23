#include "Configure_Level_Form.h"
#include "ui_Configure_Level_Form.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include <QMessageBox>
#include <QDir>
#include <assert.h>

Configure_Level_Form::Configure_Level_Form(QWidget *parent, Plugin_Settings *pluginSettings, const QString &location) :
    QDialog(parent),
    ui(new Ui::Configure_Level_Form) {
    assert(pluginSettings);
    this->pluginSettings = pluginSettings;
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->levelLocation = location + "/" + Common_Strings::LEVELS + "/" + Common_Strings::GAME_NAME;
    QDir dir(location + "/" + Common_Strings::LEVELS);
    if (!dir.exists(this->levelLocation)) dir.mkdir(this->levelLocation); //don't bother checking for success here

    //Setup the UI
    ui->setupUi(this);
    this->ui->cbGenerateNewLevels->setChecked(this->pluginSettings->generateNewLevels);
    this->Enable_New_Level_ComboBoxes(this->pluginSettings->generateNewLevels);
}

Configure_Level_Form::~Configure_Level_Form() {
    delete this->ui;
}

void Configure_Level_Form::on_buttonBox_clicked(QAbstractButton *button) {
    assert(button);
    if (!this->ui->buttonBox->buttons().contains(button)) return; //ignore other buttons
    if (this->ui->buttonBox->buttonRole(button) != QDialogButtonBox::AcceptRole) {
        this->close();
        return;
    }
    if (!this->At_Least_One_Very_Common_Selected()) {
        QMessageBox::critical(this, Common_Strings::LEVEL_HEADED,
                              "At least one level type must have a \"" + STRING_VERY_COMMON + "\" chance!",
                              Common_Strings::OK);
        return;
    }
    if (this->ui->comboLevelScripts->currentText() == STRING_NO_LEVEL_SCRIPTS_FOUND) {
        QMessageBox::critical(this, Common_Strings::LEVEL_HEADED,
                              "No level scripts could be found! Try generating new levels.",
                              Common_Strings::OK);
        return;
    }
    //Save the settings
    this->pluginSettings->generateNewLevels = this->ui->cbGenerateNewLevels->isChecked();
    if (!this->pluginSettings->generateNewLevels) {
        this->pluginSettings->levelScripts = this->ui->comboLevelScripts->currentText();
    } else {
        this->pluginSettings->standardOverworldChance = this->ui->comboStandardOverworld->currentText();
        this->pluginSettings->undergroundChance = this->ui->comboUnderground->currentText();
        this->pluginSettings->underwaterChance = this->ui->comboUnderwater->currentText();
        this->pluginSettings->bridgeChance = this->ui->comboBridge->currentText();
        this->pluginSettings->islandChance = this->ui->comboIsland->currentText();
    }
    this->close();
}

void Configure_Level_Form::Clear_Chance_ComboBoxes() {
    this->ui->comboStandardOverworld->clear();
    this->ui->comboUnderground->clear();
    this->ui->comboUnderwater->clear();
    this->ui->comboBridge->clear();
    this->ui->comboIsland->clear();
}

void Configure_Level_Form::Populate_Chance_ComboBoxes() {
    this->Populate_Chance_ComboBox(this->ui->comboStandardOverworld);
    this->Populate_Chance_ComboBox(this->ui->comboUnderground);
    this->Populate_Chance_ComboBox(this->ui->comboUnderwater);
    this->Populate_Chance_ComboBox(this->ui->comboBridge);
    this->Populate_Chance_ComboBox(this->ui->comboIsland);
    this->ui->comboStandardOverworld->setCurrentText(this->pluginSettings->standardOverworldChance);
    this->ui->comboUnderground->setCurrentText(this->pluginSettings->undergroundChance);
    this->ui->comboUnderwater->setCurrentText(this->pluginSettings->underwaterChance);
    this->ui->comboBridge->setCurrentText(this->pluginSettings->bridgeChance);
    this->ui->comboIsland->setCurrentText(this->pluginSettings->islandChance);
}

void Configure_Level_Form::Populate_Chance_ComboBox(QComboBox *comboBox) {
    assert(comboBox);
    comboBox->clear();
    comboBox->addItem(STRING_VERY_COMMON);
    comboBox->addItem(STRING_COMMON);
    comboBox->addItem(STRING_UNCOMMON);
    comboBox->addItem(STRING_RARE);
    comboBox->addItem(STRING_NONE);
}

void Configure_Level_Form::Populate_Level_Scripts_ComboBox() {
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
    if (validLevelFolders.contains(this->pluginSettings->levelScripts)) {
        this->ui->comboLevelScripts->setCurrentText(this->pluginSettings->levelScripts);
    } else {
        this->ui->comboLevelScripts->setCurrentIndex(0);
    }
}

bool Configure_Level_Form::At_Least_One_Very_Common_Selected() {
    if (!this->ui->cbGenerateNewLevels->isChecked()) return true;
    if (this->ui->comboStandardOverworld->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboUnderground->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboUnderwater->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboBridge->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboIsland->currentText() == STRING_VERY_COMMON) return true;
    return false;
}

void Configure_Level_Form::Enable_New_Level_ComboBoxes(bool enable) {
    //Toggle the Level Scripts
    this->ui->lblLevelScripts->setEnabled(!enable);
    this->ui->comboLevelScripts->setEnabled(!enable);
    this->ui->btnClearAllRandomLevelScripts->setEnabled(!enable);

    //Toggle the comboboxes
    this->ui->lblStandardOverworld->setEnabled(enable);
    this->ui->lblUnderground->setEnabled(enable);
    this->ui->lblUnderwater->setEnabled(enable);
    this->ui->lblBridge->setEnabled(enable);
    this->ui->lblIsland->setEnabled(enable);
    this->ui->comboStandardOverworld->setEnabled(enable);
    this->ui->comboUnderground->setEnabled(enable);
    this->ui->comboUnderwater->setEnabled(enable);
    this->ui->comboBridge->setEnabled(enable);
    this->ui->comboIsland->setEnabled(enable);

    //Populate the appropriate ComboBoxes
    if (enable) {
        this->ui->comboLevelScripts->clear();
        this->Populate_Chance_ComboBoxes();
    } else {
        this->Clear_Chance_ComboBoxes();
        this->Populate_Level_Scripts_ComboBox();
    }
}

void Configure_Level_Form::on_cbGenerateNewLevels_toggled(bool checked) {
    this->Enable_New_Level_ComboBoxes(checked);
}

void Configure_Level_Form::on_btnClearAllRandomLevelScripts_clicked() {
    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this, Common_Strings::LEVEL_HEADED,
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
            QMessageBox::information(this, Common_Strings::LEVEL_HEADED,
                                     "All previously generated random level scripts have been removed!",
                                     Common_Strings::OK);
        } else {
            QMessageBox::critical(this, Common_Strings::LEVEL_HEADED, Common_Strings::LEVEL_HEADED +
                                  " could not delete all of the randomly generated level scripts!",
                                  Common_Strings::OK);
        }
        this->Populate_Level_Scripts_ComboBox();
    }
}
