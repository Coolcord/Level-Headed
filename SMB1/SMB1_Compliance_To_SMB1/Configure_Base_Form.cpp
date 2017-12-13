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
    this->Load_Settings();
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
    this->Save_Settings();
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

void Configure_Base_Form::Load_Settings() {
    if (!this->pluginSettings->baseROM.isEmpty()) this->ui->comboBaseROM->setCurrentText(this->pluginSettings->baseROM);
    this->ui->comboMusic->setCurrentIndex(this->pluginSettings->music);
    this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    this->ui->sbLives->setValue(this->pluginSettings->numLives);
    this->ui->cbInfiniteLives->setChecked(this->pluginSettings->infiniteLives);
    this->ui->cbGodMode->setChecked(this->pluginSettings->godMode);
    this->ui->cb2PlayerGame->setChecked(!this->pluginSettings->addLuigiGame);
    this->ui->cbRevertToSuperMario->setCheckState(this->pluginSettings->superMarioOnDamage);
    //this->ui->cbLakituThrowArc->setCheckState(this->pluginSettings->lakituThrowArc);
    this->ui->comboEnemySpeed->setCurrentIndex(this->pluginSettings->enemySpeed-1);
}

void Configure_Base_Form::Save_Settings() {
    QString baseROM = this->ui->comboBaseROM->currentText();
    if (!baseROM.isEmpty() && baseROM != STRING_NO_ROMS_INSTALLED) this->pluginSettings->baseROM = baseROM;
    this->pluginSettings->infiniteLives = this->ui->cbInfiniteLives->isChecked();
    this->pluginSettings->music = this->ui->comboMusic->currentIndex();
    this->pluginSettings->graphics = this->ui->comboGraphics->currentIndex();
    if (this->pluginSettings->infiniteLives) this->pluginSettings->numLives = 1;
    else this->pluginSettings->numLives = this->ui->sbLives->value();
    this->pluginSettings->godMode = this->ui->cbGodMode->isChecked();
    this->pluginSettings->addLuigiGame = !this->ui->cb2PlayerGame->isChecked();
    this->pluginSettings->superMarioOnDamage = this->ui->cbRevertToSuperMario->checkState();
    //this->pluginSettings->lakituThrowArc = this->ui->cbLakituThrowArc->checkState();
    this->pluginSettings->enemySpeed = this->ui->comboEnemySpeed->currentIndex()+1;
    this->close();
}

void Configure_Base_Form::on_btnInstallNewROM_clicked() {
    if (!this->writerPlugin->Install_ROM().isEmpty()) {
        this->Populate_Installed_ROMs();
    }
}

void Configure_Base_Form::on_cbInfiniteLives_toggled(bool checked) {
    this->ui->lblLives->setEnabled(!checked);
    if (checked) this->ui->sbLives->clear();
    else this->ui->sbLives->setValue(this->pluginSettings->numLives);
    this->ui->sbLives->setEnabled(!checked);
}
