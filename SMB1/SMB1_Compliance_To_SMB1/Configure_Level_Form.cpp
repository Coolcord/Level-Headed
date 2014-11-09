#include "Configure_Level_Form.h"
#include "ui_Configure_Level_Form.h"
#include "../../Level-Headed/Common_Strings.h"
#include <QMessageBox>
#include <assert.h>

Configure_Level_Form::Configure_Level_Form(QWidget *parent, Plugin_Settings *pluginSettings) :
    QDialog(parent),
    ui(new Ui::Configure_Level_Form) {
    assert(pluginSettings);
    this->pluginSettings = pluginSettings;
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

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
    //Save the settings
    this->pluginSettings->generateNewLevels = this->ui->cbGenerateNewLevels->isChecked();
    this->pluginSettings->levelScripts = this->ui->comboLevelScripts->currentText();
    this->pluginSettings->standardOverworldChance = this->ui->comboStandardOverworld->currentText();
    this->pluginSettings->undergroundChance = this->ui->comboUnderground->currentText();
    this->pluginSettings->underwaterChance = this->ui->comboUnderwater->currentText();
    this->pluginSettings->bridgeChance = this->ui->comboBridge->currentText();
    this->pluginSettings->islandChance = this->ui->comboIsland->currentText();
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
    }
    else {
        this->Clear_Chance_ComboBoxes();
        this->Populate_Level_Scripts_ComboBox();
    }
}

void Configure_Level_Form::on_cbGenerateNewLevels_toggled(bool checked) {
    this->Enable_New_Level_ComboBoxes(checked);
}
