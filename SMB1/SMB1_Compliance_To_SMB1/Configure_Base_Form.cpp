#include "Configure_Base_Form.h"
#include "ui_Configure_Base_Form.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
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
    this->Populate_Graphics_Packs();
    this->Populate_Music_Packs();
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

void Configure_Base_Form::Populate_Graphics_Packs() {
    QStringList packsWithExtension = this->writerPlugin->Graphics_Get_Graphic_Packs();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboGraphics->addItems(packs);
}

void Configure_Base_Form::Populate_Music_Packs() {
    QStringList packsWithExtension = this->writerPlugin->Music_Get_Music_Packs();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboMusic->addItems(packs);
}

void Configure_Base_Form::Load_Settings() {
    if (!this->pluginSettings->baseROM.isEmpty()) this->ui->comboBaseROM->setCurrentText(this->pluginSettings->baseROM);
    this->ui->leOutputROMLocation->setText(this->pluginSettings->outputROMLocation);
    this->ui->cbOverwriteOutputROM->setChecked(this->pluginSettings->overwriteOuputROM);
    if (this->pluginSettings->music < this->ui->comboMusic->count()) this->ui->comboMusic->setCurrentIndex(this->pluginSettings->music);
    else this->ui->comboMusic->setCurrentIndex(2); //use original music
    this->ui->cbCombineWithOtherMusicPacks->setChecked(this->pluginSettings->combineMusicWithOtherPacks);
    this->ui->comboTone->setCurrentIndex(this->pluginSettings->toneColor);
    if (this->pluginSettings->graphics < this->ui->comboGraphics->count()) this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    else this->ui->comboGraphics->setCurrentIndex(1); //use original graphics
    this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    this->ui->sbLives->setValue(this->pluginSettings->numLives);
    this->ui->cbInfiniteLives->setChecked(this->pluginSettings->infiniteLives);
    this->ui->cbGodMode->setChecked(this->pluginSettings->godMode);
    this->ui->cb2PlayerGame->setChecked(!this->pluginSettings->addLuigiGame);
    this->ui->cbRevertToSuperMario->setCheckState(this->pluginSettings->superMarioOnDamage);
    this->ui->cbLakituThrowArc->setCheckState(this->pluginSettings->lakituThrowArc);
    this->ui->cbAutoscroll->setCheckState(this->pluginSettings->autoscroll);
    this->ui->cbReplaceFireFlowerWithHammerSuit->setCheckState(this->pluginSettings->replaceFireFlowerWithHammerSuit);
    this->ui->comboEnemySpeed->setCurrentIndex(this->pluginSettings->enemySpeed-1);
}

void Configure_Base_Form::Save_Settings() {
    QString baseROM = this->ui->comboBaseROM->currentText();
    if (!baseROM.isEmpty() && baseROM != STRING_NO_ROMS_INSTALLED) this->pluginSettings->baseROM = baseROM;
    if (QFileInfo(this->ui->leOutputROMLocation->text()).absoluteDir().exists()) this->pluginSettings->outputROMLocation = this->ui->leOutputROMLocation->text();
    this->pluginSettings->overwriteOuputROM = this->ui->cbOverwriteOutputROM->isChecked();
    this->pluginSettings->infiniteLives = this->ui->cbInfiniteLives->isChecked();
    this->pluginSettings->music = this->ui->comboMusic->currentIndex();
    this->pluginSettings->combineMusicWithOtherPacks = this->ui->cbCombineWithOtherMusicPacks->isChecked();
    this->pluginSettings->toneColor = this->ui->comboTone->currentIndex();
    this->pluginSettings->graphics = this->ui->comboGraphics->currentIndex();
    if (this->pluginSettings->infiniteLives) this->pluginSettings->numLives = 1;
    else this->pluginSettings->numLives = this->ui->sbLives->value();
    this->pluginSettings->godMode = this->ui->cbGodMode->isChecked();
    this->pluginSettings->addLuigiGame = !this->ui->cb2PlayerGame->isChecked();
    this->pluginSettings->superMarioOnDamage = this->ui->cbRevertToSuperMario->checkState();
    this->pluginSettings->lakituThrowArc = this->ui->cbLakituThrowArc->checkState();
    this->pluginSettings->autoscroll = this->ui->cbAutoscroll->checkState();
    this->pluginSettings->replaceFireFlowerWithHammerSuit = this->ui->cbReplaceFireFlowerWithHammerSuit->checkState();
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

void Configure_Base_Form::on_btnOutputROMLocation_clicked() {
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
