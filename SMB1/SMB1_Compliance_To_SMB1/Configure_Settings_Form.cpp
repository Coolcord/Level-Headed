#include "Configure_Settings_Form.h"
#include "ui_Configure_Settings_Form.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
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

Configure_Settings_Form::Configure_Settings_Form(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Plugin_Settings *pluginSettings) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Configure_Settings_Form)
{
    assert(parent); assert(writerPlugin); assert(pluginSettings);
    this->parent = parent;
    this->applicationLocation = applicationLocation;
    this->pluginSettings = pluginSettings;
    this->writerPlugin = writerPlugin;
    ui->setupUi(this);
    this->tabBaseGame = new Tab_Base_Game(this, applicationLocation, writerPlugin, this->ui, pluginSettings);
    this->tabDifficulty = new Tab_Difficulty(this, applicationLocation, writerPlugin, this->ui, pluginSettings);
    this->tabLevelGenerator = new Tab_Level_Generator(this, applicationLocation, writerPlugin, this->ui, pluginSettings);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->Load_Settings();
}

Configure_Settings_Form::~Configure_Settings_Form() {
    delete this->tabBaseGame;
    delete this->tabDifficulty;
    delete this->tabLevelGenerator;
    delete ui;
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

void Configure_Settings_Form::on_btnInstallNewROM_clicked() {
    this->tabBaseGame->Install_New_ROM();
}

void Configure_Settings_Form::on_btnOutputROMLocation_clicked() {
    QString extension = this->pluginSettings->baseROM.split('.').last().toLower();
    QString extensionFilter = "";
    if (extension == "fds") extensionFilter = "Famicom Images (*.fds)";
    else if (extension == "nes") extensionFilter = "NES ROMs (*.nes)";
    else extensionFilter = "NES ROMs (*.nes *.fds)";

    //Ask the user where they want to save the output ROM
    QString openLocation = QFileInfo(this->ui->leOutputROMLocation->text()).absolutePath();
    if (!QFileInfo(openLocation).exists()) openLocation = this->applicationLocation;
    QString outputROMLocation = QFileDialog::getSaveFileName(this->parent, "Save Location", openLocation, extensionFilter);
    if (outputROMLocation.isEmpty()) return;
    else this->ui->leOutputROMLocation->setText(outputROMLocation);
}

void Configure_Settings_Form::on_buttonBox_clicked(QAbstractButton *button) {
    assert(button);
    if (!this->ui->buttonBox->buttons().contains(button)) return; //ignore other buttons
    if (this->ui->buttonBox->buttonRole(button) != QDialogButtonBox::AcceptRole) {
        this->close();
        return;
    }
    if (!this->tabLevelGenerator->At_Least_One_Very_Common_Selected()) {
        QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED,
                              "At least one level type must have a \"" + STRING_VERY_COMMON + "\" chance!",
                              Common_Strings::STRING_OK);
        return;
    }
    if (this->ui->comboLevelScripts->currentText() == STRING_NO_LEVEL_SCRIPTS_FOUND) {
        QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED,
                              "No level scripts could be found! Try generating new levels.",
                              Common_Strings::STRING_OK);
        return;
    }

    this->Save_Settings();
    this->close();
}

void Configure_Settings_Form::on_radioGenerateNewLevels_toggled(bool checked) {
    this->tabLevelGenerator->Enable_New_Level_Options(checked);
}

void Configure_Settings_Form::on_btnClearAllRandomLevelScripts_clicked() {
    this->tabLevelGenerator->Clear_All_Random_Level_Scripts();
}

void Configure_Settings_Form::on_cbRandomNumWorlds_clicked(bool checked) {
    this->tabLevelGenerator->Enable_Random_Number_Of_Worlds(checked);
}

void Configure_Settings_Form::on_sbNumLevelsPerWorld_valueChanged() {
    this->tabLevelGenerator->Update_Worlds();
}

void Configure_Settings_Form::on_sbNumWorlds_valueChanged() {
    this->tabLevelGenerator->Update_Worlds();
}

void Configure_Settings_Form::on_btnNewRandomSeed_clicked() {
    this->ui->sbRandomSeed->setValue(QTime::currentTime().msecsSinceStartOfDay());
}

void Configure_Settings_Form::on_cbInfiniteLives_toggled(bool checked) {
    this->ui->lblLives->setEnabled(!checked);
    if (checked) this->ui->sbLives->clear();
    else this->ui->sbLives->setValue(this->pluginSettings->numLives);
    this->ui->sbLives->setEnabled(!checked);
}

void Configure_Settings_Form::on_comboDifficulty_currentIndexChanged(int index) {
    if (index==0) this->ui->layoutCustomDifficultyWidget->show();
    else this->ui->layoutCustomDifficultyWidget->hide();
}
