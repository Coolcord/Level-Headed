#include "Configure_Base_Form.h"
#include "ui_Configure_Base_Form.h"
#include <assert.h>

Configure_Writer_Form::Configure_Writer_Form(QWidget *parent, Base_Game_Settings *baseGameSettings, SMB1_Writer_Interface *writerPlugin) :
    QDialog(parent),
    ui(new Ui::Configure_Writer_Form) {
    assert(baseGameSettings);
    assert(writerPlugin);
    this->baseGameSettings = baseGameSettings;
    this->writerPlugin = writerPlugin;
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    //Setup the UI
    ui->setupUi(this);
    this->Populate_Installed_ROMs();
    this->ui->cbDuplicateLevels->setChecked(this->baseGameSettings->noDuplicates);
    this->ui->sbNumWorlds->setValue(this->baseGameSettings->numWorlds);
    if (this->baseGameSettings->noDuplicates) this->ui->sbNumWorlds->setMaximum(7);
}

Configure_Writer_Form::~Configure_Writer_Form() {
    delete this->ui;
}

void Configure_Writer_Form::on_buttonBox_clicked(QAbstractButton *button) {
    assert(button);
    this->close();
}

void Configure_Writer_Form::on_sbNumWorlds_valueChanged(int arg1) {
    this->baseGameSettings->numWorlds = arg1;
}

void Configure_Writer_Form::on_cbDuplicateLevels_toggled(bool checked) {
    this->baseGameSettings->noDuplicates = checked;
    if (!checked) {
        this->ui->sbNumWorlds->setMaximum(8);
    } else if (checked && this->baseGameSettings->numWorlds > 7) {
        this->ui->sbNumWorlds->setMaximum(7);
        this->baseGameSettings->numWorlds = 7;
    }
}

void Configure_Writer_Form::on_comboBaseROM_currentIndexChanged(const QString &arg1) {
    this->baseGameSettings->baseROM = arg1;
}

void Configure_Writer_Form::Populate_Installed_ROMs() {
    this->ui->comboBaseROM->clear();
    QStringList installedROMs = this->writerPlugin->Get_Installed_ROMs();
    QString tmpBaseROM = this->baseGameSettings->baseROM;
    this->ui->comboBaseROM->addItems(installedROMs);
    if (tmpBaseROM != "" && installedROMs.contains(tmpBaseROM)) {
        this->ui->comboBaseROM->setCurrentText(tmpBaseROM);
    } else {
        this->ui->comboBaseROM->setCurrentIndex(0);
    }
    this->baseGameSettings->baseROM = this->ui->comboBaseROM->currentText();
}


void Configure_Writer_Form::on_btnInstallNewROM_clicked() {
    if (!this->writerPlugin->Install_ROM().isEmpty()) {
        this->Populate_Installed_ROMs();
    }
}
