#include "Main_Window.h"
#include "ui_Main_Window.h"
#include "Plugin_Handler.h"
#include <QWindow>

#include "Common_Strings.h"
#include <QMessageBox>

Main_Window::Main_Window(QWidget *parent) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Main_Window)
{
    ui->setupUi(this);
    this->pluginHandler = new Plugin_Handler(this);
}

Main_Window::~Main_Window() {
    delete ui;
}

bool Main_Window::Create_Directories() {
    return this->pluginHandler->Create_Directories();
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

bool Main_Window::Populate_Writers() {
    QStringList writerPlugins = this->pluginHandler->Get_Writer_Plugins();
    if (writerPlugins.empty()) return false; //TODO: Throw an error here
    this->ui->comboBaseGame->clear();
    this->ui->comboBaseGame->addItem(Common_Strings::SELECT_A_PLUGIN);
    this->ui->comboBaseGame->addItems(writerPlugins);
    return true;
}

bool Main_Window::Populate_Generators(const QString &writerPlugin) {
    QStringList generatorPlugins = this->pluginHandler->Get_Generator_Plugins(writerPlugin);
    if (generatorPlugins.empty()) return false; //TODO: Throw an error here
    this->ui->comboLevelGenerator->clear();
    this->ui->comboLevelGenerator->addItem(Common_Strings::SELECT_A_PLUGIN);
    this->ui->comboLevelGenerator->addItems(generatorPlugins);
    return true;
}

void Main_Window::on_comboBaseGame_currentIndexChanged(const QString &arg1) {
    if (arg1 == NULL || arg1.isEmpty() || arg1 == Common_Strings::SELECT_A_PLUGIN) {
        this->Disable_All();
        return;
    } else if (this->Populate_Generators(arg1)){
        this->Enable_Generator();
        return;
    }
}

void Main_Window::on_comboLevelGenerator_currentIndexChanged(const QString &arg1) {
    if (arg1 == NULL || arg1.isEmpty() || arg1 == Common_Strings::SELECT_A_PLUGIN) {
        this->Enable_Generator();
        return;
    } else {
        this->Enable_Buttons();
        return;
    }
}
