#include "Configure_Base_Form.h"
#include "ui_Configure_Base_Form.h"
#include <assert.h>

Configure_Base_Form::Configure_Base_Form(QWidget *parent, Plugin_Settings *pluginSettings, SMB1_Writer_Interface *writerPlugin, const QString &location) :
    QDialog(parent),
    ui(new Ui::Configure_Base_Form) {
    assert(pluginSettings);
    assert(writerPlugin);
    this->parent = parent;
    this->applicationLocation = location;
    this->pluginSettings = pluginSettings;
    this->writerPlugin = writerPlugin;
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    //Setup the UI
    ui->setupUi(this);
    this->Populate_Installed_ROMs();
    this->ui->sbNumWorlds->setValue(this->pluginSettings->numWorlds);
    this->ui->sbNumLevelsPerWorld->setValue(this->pluginSettings->numLevelsPerWorld);
    this->Fix_Max_Worlds(this->pluginSettings->numLevelsPerWorld);
}

Configure_Base_Form::~Configure_Base_Form() {
    delete this->ui;
}

void Configure_Base_Form::on_buttonBox_clicked(QAbstractButton *button) {
    assert(button);
    if (!this->ui->buttonBox->buttons().contains(button)) return; //ignore other buttons
    if (this->ui->buttonBox->buttonRole(button) != QDialogButtonBox::AcceptRole) {
        this->close();
        return;
    }
    QString baseROM = this->ui->comboBaseROM->currentText();
    if (!baseROM.isEmpty() && baseROM != STRING_NO_ROMS_INSTALLED) {
        this->pluginSettings->baseROM = baseROM;
    }
    this->pluginSettings->numWorlds = this->ui->sbNumWorlds->value();
    this->pluginSettings->numLevelsPerWorld = this->ui->sbNumLevelsPerWorld->value();
    this->close();
}

void Configure_Base_Form::Populate_Installed_ROMs() {
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

void Configure_Base_Form::Fix_Max_Worlds(int numLevelsPerWorld) {
    switch (numLevelsPerWorld) {
    default:
        this->ui->sbNumWorlds->setMaximum(7); //TODO: Change this to 8 once item sending is implemented
        break;
    case 4:
        this->ui->sbNumWorlds->setMaximum(7); //TODO: Change this once item sending is implemented
        break;
    case 5:
        this->ui->sbNumWorlds->setMaximum(5);
        break;
    case 6:
        this->ui->sbNumWorlds->setMaximum(4);
        break;
    case 7:
    case 8:
        this->ui->sbNumWorlds->setMaximum(3);
        break;
    }
}

void Configure_Base_Form::on_btnInstallNewROM_clicked() {
    if (!this->writerPlugin->Install_ROM().isEmpty()) {
        this->Populate_Installed_ROMs();
    }
}

void Configure_Base_Form::on_sbNumLevelsPerWorld_valueChanged(int arg1) {
    this->Fix_Max_Worlds(arg1);
}
