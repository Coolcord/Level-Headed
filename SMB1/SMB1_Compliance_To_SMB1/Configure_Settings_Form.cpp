#include "Configure_Settings_Form.h"
#include "ui_Configure_Settings_Form.h"
#include "../../Common_Files/Version.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Writer/ROM_Filename.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include "Config_File_Handler.h"
#include "Tab_Base_Game.h"
#include "Tab_Difficulty.h"
#include "Tab_Level_Generator.h"
#include <QCryptographicHash>
#include <QFileDialog>
#include <QFileInfo>
#include <QTime>
#include <QMessageBox>
#include <QDir>
#include <assert.h>

const static QString STRING_UPDATES_LINK = "https://github.com/Coolcord/Level-Headed/releases";
const static QString STRING_SOURCE_CODE_LINK = "https://github.com/Coolcord/Level-Headed";

Configure_Settings_Form::Configure_Settings_Form(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Plugin_Settings *pluginSettings) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Configure_Settings_Form)
{
    assert(parent); assert(writerPlugin); assert(pluginSettings);
    this->loading = true;
    this->parent = parent;
    this->applicationLocation = applicationLocation;
    this->pluginSettings = pluginSettings;
    this->writerPlugin = writerPlugin;
    ui->setupUi(this);

    //Set the About Text
    this->ui->textAbout->setHtml("<p align=\"center\">"+Common_Strings::STRING_LEVEL_HEADED+" "+Version::VERSION+Common_Strings::STRING_HTML_NEW_LINE+
                                 "Using SMB1 Compliance Generator for SMB1</p>"+this->ui->textAbout->toHtml());
    int numberOfLines = 19; //necessary for determining the QTextBrowser height
    int height = QFontMetrics(this->ui->textAbout->font()).height()*numberOfLines;
    this->ui->textAbout->setMinimumHeight(height);
    this->ui->textAbout->setMaximumHeight(height);

    this->tabBaseGame = new Tab_Base_Game(this, applicationLocation, writerPlugin, this->ui, pluginSettings);
    this->tabDifficulty = new Tab_Difficulty(this, applicationLocation, writerPlugin, this->ui, pluginSettings);
    this->tabLevelGenerator = new Tab_Level_Generator(this, applicationLocation, writerPlugin, this->ui, pluginSettings, this->tabBaseGame);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->Load_Settings();
    this->ui->tabWidget->setCurrentIndex(this->pluginSettings->tab);
    this->tabBaseGame->Enable_Partial_Support_Mode(this->ui->comboBaseROM->currentText().startsWith(ROM_Filename::STRING_PARTIAL_SUPPORT));
    this->loading = false;
}

Configure_Settings_Form::~Configure_Settings_Form() {
    this->pluginSettings->tab = this->ui->tabWidget->currentIndex();
    delete this->tabBaseGame;
    delete this->tabDifficulty;
    delete this->tabLevelGenerator;
    delete ui;
}

void Configure_Settings_Form::accept() {
    if (!this->tabLevelGenerator->At_Least_One_Very_Common_Selected()) {
        QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED,
                              "At least one level type must have a \"" + STRING_VERY_COMMON + "\" chance!",
                              QMessageBox::Ok);
        return;
    }
    if (this->ui->radioUseLevelScripts->isChecked() && this->ui->comboLevelScripts->currentText() == STRING_NO_LEVEL_SCRIPTS_FOUND) {
        QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED,
                              "No level scripts could be found! Try generating new levels.",
                              QMessageBox::Ok);
        return;
    }

    this->Save_Settings();
}

Ui::Configure_Settings_Form *Configure_Settings_Form::Get_UI() {
    return this->ui;
}

void Configure_Settings_Form::Load_Settings() {
    this->tabBaseGame->Load_Settings();
    this->tabLevelGenerator->Load_Settings();
    this->tabDifficulty->Load_Settings();
}

void Configure_Settings_Form::Save_Settings() {
    this->tabBaseGame->Save_Settings();
    this->tabLevelGenerator->Save_Settings();
    this->tabDifficulty->Save_Settings();
    this->close();
}

void Configure_Settings_Form::on_btnLoadConfig_clicked() {
    //Open a Load File Dialog
    if (this->pluginSettings->lastExternalConfigLocation.isEmpty()) this->pluginSettings->lastExternalConfigLocation = this->applicationLocation;
    QString configFileLocation = QFileDialog::getOpenFileName(this->parent, "Open a Config File", this->pluginSettings->lastExternalConfigLocation, "Level-Headed Config Files (*.lcfg)");
    if (configFileLocation == nullptr || configFileLocation.isEmpty()) return;

    Plugin_Settings tmpSettings = Plugin_Settings(*this->pluginSettings);
    bool messageShown = false;
    if (Config_File_Handler(this->parent, this->applicationLocation).Load_Plugin_Settings(&tmpSettings, configFileLocation, false, messageShown)) {
        this->tabBaseGame->Load_Settings(&tmpSettings);
        this->tabLevelGenerator->Load_Settings(&tmpSettings);
        this->tabDifficulty->Load_Settings(&tmpSettings);
        *this->pluginSettings = tmpSettings;
        this->pluginSettings->lastExternalConfigLocation = QFileInfo(configFileLocation).absolutePath();
        if (!messageShown) QMessageBox::information(this->parent, Common_Strings::STRING_LEVEL_HEADED, "Settings loaded successfully!");
    } else {
        if (!messageShown) QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, "Unable to load config file!");
    }
}

void Configure_Settings_Form::on_btnSaveConfig_clicked() {
    //Open a Save File Dialog
    if (this->pluginSettings->lastExternalConfigLocation.isEmpty()) this->pluginSettings->lastExternalConfigLocation = this->applicationLocation;
    QString configFileLocation = QFileDialog::getSaveFileName(this->parent, "Save Location", this->pluginSettings->lastExternalConfigLocation, "Level-Headed Config Files (*.lcfg)");
    if (configFileLocation == nullptr || configFileLocation.isEmpty()) return;

    Plugin_Settings tmpSettings = Plugin_Settings(*this->pluginSettings);
    this->tabBaseGame->Save_Settings(&tmpSettings);
    this->tabLevelGenerator->Save_Settings(&tmpSettings);
    this->tabDifficulty->Save_Settings(&tmpSettings);
    this->pluginSettings->lastExternalConfigLocation = QFileInfo(configFileLocation).absolutePath();
    if (Config_File_Handler(this->parent, this->applicationLocation).Save_Plugin_Settings(&tmpSettings, configFileLocation, false)) {
        QMessageBox::information(this->parent, Common_Strings::STRING_LEVEL_HEADED, "Settings saved successfully!");
    } else {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, "Unable to save config file!");
    }
}

void Configure_Settings_Form::on_comboBaseROM_currentTextChanged(const QString &arg1) {
    if (!this->loading) {
        bool wasPartialSupportEnabled = this->tabBaseGame->Is_Partial_Support_Mode_Enabled();
        bool enablePartialSupportMode = arg1.startsWith(ROM_Filename::STRING_PARTIAL_SUPPORT);
        this->tabBaseGame->Enable_Partial_Support_Mode(enablePartialSupportMode);
        if (wasPartialSupportEnabled && !enablePartialSupportMode) this->tabBaseGame->Use_Random_Settings();
    }
}

void Configure_Settings_Form::on_btnInstallNewROM_clicked() {
    this->loading = true;
    this->tabBaseGame->Install_New_ROM();
    this->loading = false;

    //Only enable partial support after the installation is complete
    this->tabBaseGame->Enable_Partial_Support_Mode(this->ui->comboBaseROM->currentText().startsWith(ROM_Filename::STRING_PARTIAL_SUPPORT));
}

void Configure_Settings_Form::on_btnOutputROMLocation_clicked() {
    QString extension = this->pluginSettings->baseROM.split('.').last().toLower();
    QString extensionFilter = "";
    if (extension == "fds") extensionFilter = "Famicom Images (*.fds)";
    else if (extension == "nes") extensionFilter = "NES ROMs (*.nes)";
    else extensionFilter = "NES ROMs (*.nes *.fds)";

    //Ask the user where they want to save the output ROM
    QString openLocation = QFileInfo(this->ui->leOutputROMLocation->text()).absolutePath();
    if (!QFileInfo::exists(openLocation)) openLocation = this->applicationLocation;
    QString outputROMLocation = QFileDialog::getSaveFileName(this->parent, "Save Location", openLocation, extensionFilter);
    if (outputROMLocation.isEmpty()) return;
    else this->ui->leOutputROMLocation->setText(outputROMLocation);
}

void Configure_Settings_Form::on_btnUseRandomSettings_clicked() {
    this->tabBaseGame->Use_Random_Settings();
}

void Configure_Settings_Form::on_btnUseBasicSettings_clicked() {
    this->tabBaseGame->Use_Basic_Settings();
}

void Configure_Settings_Form::on_btnUseOriginalSettings_clicked() {
    this->tabBaseGame->Use_Original_Settings();
}

void Configure_Settings_Form::on_radioGenerateNewLevels_toggled(bool checked) {
    this->tabLevelGenerator->Enable_New_Level_Options(checked);
}

void Configure_Settings_Form::on_btnClearAllRandomLevelScripts_clicked() {
    this->tabLevelGenerator->Clear_All_Random_Level_Scripts();
}

void Configure_Settings_Form::on_cbRandomNumWorlds_toggled(bool checked) {
    this->tabLevelGenerator->Enable_Random_Number_Of_Worlds(checked);
}

void Configure_Settings_Form::on_sbNumLevelsPerWorld_valueChanged() {
    this->tabLevelGenerator->Update_Worlds();
}

void Configure_Settings_Form::on_sbNumWorlds_valueChanged() {
    this->tabLevelGenerator->Update_Worlds();
}

void Configure_Settings_Form::on_btnNewRandomSeed_clicked() {
    this->ui->leRandomSeed->setText(QString::number(QTime::currentTime().msecsSinceStartOfDay()));
}

void Configure_Settings_Form::on_radioStandardLevelDistribution_toggled(bool checked) {
    this->tabLevelGenerator->Enable_Standard_Level_Distribution_Options(checked);
}

void Configure_Settings_Form::on_cbGodMode_toggled(bool checked) {
    if (checked) {
        this->ui->cbNoPipeIntroCutscenes->setEnabled(false);
        this->ui->cbNoPipeIntroCutscenes->setChecked(true);
    } else {
        this->ui->cbNoPipeIntroCutscenes->setEnabled(true);
        this->ui->cbNoPipeIntroCutscenes->setChecked(this->pluginSettings->noPipeIntroCutscenes);
    }
}

void Configure_Settings_Form::on_btnUseDefaultSettingsLevelDistribution_clicked() {
    this->tabLevelGenerator->Use_Default_Settings();
}

void Configure_Settings_Form::on_comboDifficulty_currentIndexChanged(int index) {
    if (index==0) this->ui->layoutCustomDifficultyWidget->show();
    else this->ui->layoutCustomDifficultyWidget->hide();
}

void Configure_Settings_Form::on_radioStartingLives_toggled(bool checked) {
    if (checked) {
        this->ui->sbLives->show();
        this->ui->cbAlwaysRestartFromCurrentWorldAfterGameOver->show();
    } else {
        this->ui->sbLives->hide();
        this->ui->cbAlwaysRestartFromCurrentWorldAfterGameOver->hide();
    }
}

