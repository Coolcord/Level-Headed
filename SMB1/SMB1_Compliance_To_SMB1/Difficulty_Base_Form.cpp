#include "Difficulty_Base_Form.h"
#include "ui_Difficulty_Base_Form.h"

Difficulty_Base_Form::Difficulty_Base_Form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Difficulty_Base_Form)
{
    ui->setupUi(this);
}

Difficulty_Base_Form::~Difficulty_Base_Form()
{
    delete ui;
}

void Difficulty_Base_Form::on_buttonBox_clicked(QAbstractButton *button) {
    //TODO: Write this...
}
