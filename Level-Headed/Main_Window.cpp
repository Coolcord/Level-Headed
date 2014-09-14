#include "Main_Window.h"
#include "ui_Main_Window.h"
#include <QWindow>

Main_Window::Main_Window(QWidget *parent) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Main_Window)
{
    ui->setupUi(this);
    this->pluginHandler = new Plugin_Handler();
}

Main_Window::~Main_Window() {
    delete ui;
}

void Main_Window::Disable_All() {
    this->ui->lblLevelGenerator->setEnabled(false);
    this->ui->comboLevelGenerator->clear();
    this->ui->comboLevelGenerator->setEnabled(false);
    this->ui->lblConfigurePlugins->setEnabled(false);
    this->ui->btnConfigureLevelGenerator->setEnabled(false);
    this->ui->btnConfigureBaseGame->setEnabled(false);
    this->ui->btnGenerateGame->setEnabled(false);
}

void Main_Window::Enable_Generator() {
    this->ui->lblLevelGenerator->setEnabled(true);
    this->ui->comboLevelGenerator->setEnabled(true);
    this->ui->lblConfigurePlugins->setEnabled(false);
    this->ui->btnConfigureBaseGame->setEnabled(false);
    this->ui->btnConfigureLevelGenerator->setEnabled(false);
    this->ui->btnGenerateGame->setEnabled(false);
}

void Main_Window::Enable_Buttons() {
    this->ui->lblConfigurePlugins->setEnabled(true);
    this->ui->btnConfigureBaseGame->setEnabled(true);
    this->ui->btnConfigureLevelGenerator->setEnabled(true);
    this->ui->btnGenerateGame->setEnabled(true);
}

void Main_Window::on_comboBaseGame_editTextChanged(const QString &arg1) {
    if (arg1 == NULL || arg1.isEmpty()) {
        this->Disable_All();
        return;
    } else {
        this->Enable_Generator();

        return;
    }
}
