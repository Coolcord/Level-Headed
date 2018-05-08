#include "Configure_Difficulty_Form.h"
#include "ui_Configure_Difficulty_Form.h"
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
    this->ui->sbHammerTime->setValue(this->difficultySettings->hammerTime);
    this->ui->sbUnderwaterCheepCheeps->setValue(this->difficultySettings->underwaterCheepCheeps);
    this->ui->sbFlyingCheepCheeps->setValue(this->difficultySettings->flyingCheepCheeps);
}

void Configure_Difficulty_Form::Save_Settings() {
    this->difficultySettings->save = true;
    this->difficultySettings->hammerTime = this->ui->sbHammerTime->value();
    this->difficultySettings->underwaterCheepCheeps = this->ui->sbUnderwaterCheepCheeps->value();
    this->difficultySettings->flyingCheepCheeps = this->ui->sbFlyingCheepCheeps->value();
}
