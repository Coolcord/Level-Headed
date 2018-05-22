#include "Configure_Difficulty_Form.h"
#include "ui_Configure_Difficulty_Form.h"
#include "Difficulty_Configurations.h"
#include <assert.h>

Configure_Difficulty_Form::Configure_Difficulty_Form(QWidget *parent, Difficulty_Settings *difficultySettings, const QString &location) :
    QDialog(parent),
    ui(new Ui::Configure_Difficulty_Form) {
    ui->setupUi(this);
    assert(difficultySettings);
    this->parent = parent;
    this->difficultySettings = difficultySettings;
    this->difficultySettings->save = false;
    this->applicationLocation = location;
    this->Load_Settings();
}

Configure_Difficulty_Form::~Configure_Difficulty_Form() {
    delete ui;
}

void Configure_Difficulty_Form::on_buttonBox_clicked(QAbstractButton *button) {
    assert(button);
    if (!this->ui->buttonBox->buttons().contains(button)) return; //ignore other buttons
    if (this->ui->buttonBox->buttonRole(button) != QDialogButtonBox::AcceptRole) {
        this->close();
        return;
    }

    this->Save_Settings();
    this->close();
}

void Configure_Difficulty_Form::Load_Settings() {
    this->ui->sbBulletTime->setValue(this->difficultySettings->bulletTime);
    this->ui->sbHammerTime->setValue(this->difficultySettings->hammerTime);
    this->ui->sbBridgeFlyingCheepCheeps->setValue(this->difficultySettings->bridgeFlyingCheepCheeps);
    this->ui->sbBridgeLakitus->setValue(this->difficultySettings->bridgeLakitus);
    this->ui->sbBridgeOffscreenBulletBills->setValue(this->difficultySettings->bridgeOffscreenBulletBills);
    this->ui->sbCastleFlyingCheepCheeps->setValue(this->difficultySettings->castleFlyingCheepCheeps);
    this->ui->sbCastleLakitus->setValue(this->difficultySettings->castleLakitus);
    this->ui->sbCastleOffscreenBulletBills->setValue(this->difficultySettings->castleOffscreenBulletBills);
    this->ui->sbIslandFlyingCheepCheeps->setValue(this->difficultySettings->islandFlyingCheepCheeps);
    this->ui->sbIslandLakitus->setValue(this->difficultySettings->islandLakitus);
    this->ui->sbIslandOffscreenBulletBills->setValue(this->difficultySettings->islandOffscreenBulletBills);
    this->ui->sbUndergroundFlyingCheepCheeps->setValue(this->difficultySettings->undergroundFlyingCheepCheeps);
    this->ui->sbUndergroundLakitus->setValue(this->difficultySettings->undergroundLakitus);
    this->ui->sbUndergroundOffscreenBulletBills->setValue(this->difficultySettings->undergroundOffscreenBulletBills);
    this->ui->sbUnderwaterBloopers->setValue(this->difficultySettings->underwaterBloopers);
    this->ui->sbUnderwaterFlyingCheepCheeps->setValue(this->difficultySettings->underwaterFlyingCheepCheeps);
    this->ui->sbUnderwaterLakitus->setValue(this->difficultySettings->underwaterLakitus);
    this->ui->sbUnderwaterSwimmingCheepCheeps->setValue(this->difficultySettings->underwaterSwimmingCheepCheeps);
    this->ui->sbStandardOverworldFlyingCheepCheeps->setValue(this->difficultySettings->standardOverworldFlyingCheepCheeps);
    this->ui->sbStandardOverworldLakitus->setValue(this->difficultySettings->standardOverworldLakitus);
    this->ui->sbStandardOverworldOffscreenBulletBills->setValue(this->difficultySettings->standardOverworldOffscreenBulletBills);
    this->ui->cbNoEnemies->setChecked(this->difficultySettings->noEnemies);
    this->ui->sbHammerTimeIntensity->setValue(this->difficultySettings->hammerTimeIntensity);
    this->ui->sbLakituSpawnChancePerLevel->setValue(this->difficultySettings->lakituSpawnChancePerLevel);
    this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->setChecked(this->difficultySettings->disableAllOtherEnemiesWhenALakituSpawns);
    this->ui->comboSpawnerPriority->setCurrentIndex(this->difficultySettings->spawnerPriority);
}

void Configure_Difficulty_Form::Save_Settings() {
    this->difficultySettings->save = true;
    this->difficultySettings->bulletTime = this->ui->sbBulletTime->value();
    this->difficultySettings->hammerTime = this->ui->sbHammerTime->value();
    this->difficultySettings->bridgeFlyingCheepCheeps = this->ui->sbBridgeFlyingCheepCheeps->value();
    this->difficultySettings->bridgeLakitus = this->ui->sbBridgeLakitus->value();
    this->difficultySettings->bridgeOffscreenBulletBills = this->ui->sbBridgeOffscreenBulletBills->value();
    this->difficultySettings->castleFlyingCheepCheeps = this->ui->sbCastleFlyingCheepCheeps->value();
    this->difficultySettings->castleLakitus = this->ui->sbCastleLakitus->value();
    this->difficultySettings->castleOffscreenBulletBills = this->ui->sbCastleOffscreenBulletBills->value();
    this->difficultySettings->islandFlyingCheepCheeps = this->ui->sbIslandFlyingCheepCheeps->value();
    this->difficultySettings->islandLakitus = this->ui->sbIslandLakitus->value();
    this->difficultySettings->islandOffscreenBulletBills = this->ui->sbBridgeOffscreenBulletBills->value();
    this->difficultySettings->undergroundFlyingCheepCheeps = this->ui->sbBridgeFlyingCheepCheeps->value();
    this->difficultySettings->undergroundLakitus = this->ui->sbUndergroundLakitus->value();
    this->difficultySettings->undergroundOffscreenBulletBills = this->ui->sbUndergroundOffscreenBulletBills->value();
    this->difficultySettings->underwaterBloopers = this->ui->sbUnderwaterBloopers->value();
    this->difficultySettings->underwaterFlyingCheepCheeps = this->ui->sbUnderwaterFlyingCheepCheeps->value();
    this->difficultySettings->underwaterLakitus = this->ui->sbUnderwaterLakitus->value();
    this->difficultySettings->underwaterSwimmingCheepCheeps = this->ui->sbUnderwaterSwimmingCheepCheeps->value();
    this->difficultySettings->standardOverworldFlyingCheepCheeps = this->ui->sbStandardOverworldFlyingCheepCheeps->value();
    this->difficultySettings->standardOverworldLakitus = this->ui->sbStandardOverworldLakitus->value();
    this->difficultySettings->standardOverworldOffscreenBulletBills = this->ui->sbStandardOverworldOffscreenBulletBills->value();
    this->difficultySettings->noEnemies = this->ui->cbNoEnemies->isChecked();
    this->difficultySettings->hammerTimeIntensity = this->ui->sbHammerTimeIntensity->value();
    this->difficultySettings->lakituSpawnChancePerLevel = this->ui->sbLakituSpawnChancePerLevel->value();
    this->difficultySettings->disableAllOtherEnemiesWhenALakituSpawns = this->ui->cbDisableAllOtherEnemiesWhenALakituSpawns->isChecked();
    this->difficultySettings->spawnerPriority = this->ui->comboSpawnerPriority->currentIndex();
}
