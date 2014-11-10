#include "Main_Window.h"
#include "ui_Main_Window.h"
#include "Plugin_Handler.h"
#include "Interpreter_Interface.h"
#include <QWindow>
#include <QFile>
#include <QPluginLoader>
#include <QDebug>

#include "Common_Strings.h"
#include <QMessageBox>

Main_Window::Main_Window(QWidget *parent) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Main_Window)
{
    ui->setupUi(this);
    this->pluginHandler = new Plugin_Handler(this);
    this->interpreterLoader = NULL;
    this->interpreterPlugin = NULL;
}

Main_Window::~Main_Window() {
    delete ui;
    delete this->interpreterLoader;
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
    if (this->interpreterPlugin) this->interpreterPlugin->Shutdown();
    if (this->interpreterLoader) this->interpreterLoader->unload();
    delete this->interpreterLoader;
    this->interpreterLoader = NULL;
    this->interpreterPlugin = NULL;
    this->ui->comboBaseGame->setCurrentIndex(0);
}

void Main_Window::Enable_Generator() {
    this->ui->lblLevelGenerator->setEnabled(true);
    this->ui->comboLevelGenerator->setEnabled(true);
    this->ui->lblConfigurePlugins->setEnabled(false);
    this->ui->btnConfigureBaseGame->setEnabled(false);
    this->ui->btnConfigureLevelGenerator->setEnabled(false);
    this->ui->btnGenerateGame->setEnabled(false);
    if (this->interpreterPlugin) this->interpreterPlugin->Shutdown();
    if (this->interpreterLoader) this->interpreterLoader->unload();
    delete this->interpreterLoader;
    this->interpreterLoader = NULL;
    this->interpreterPlugin = NULL;
}

void Main_Window::Enable_Buttons() {
    this->ui->lblConfigurePlugins->setEnabled(true);
    this->ui->btnConfigureBaseGame->setEnabled(true);
    this->ui->btnConfigureLevelGenerator->setEnabled(true);
    this->ui->btnGenerateGame->setEnabled(true);
    QString fileLocation = this->pluginHandler->Get_Interpreter_Name(this->ui->comboBaseGame->currentText(),
                                                                          this->ui->comboLevelGenerator->currentText());
    fileLocation = QApplication::applicationDirPath() + "/" + Common_Strings::PLUGINS + "/" +
                         Common_Strings::INTERPRETERS + "/" + fileLocation; //set the full path
    QFile interpreter(fileLocation);
    if (fileLocation.isEmpty() || !interpreter.exists()) {
        QMessageBox::critical(this, Common_Strings::LEVEL_HEADED, Common_Strings::LEVEL_HEADED +
                             " cannot find the interpreter plugin! Make sure that it is in the " +
                              Common_Strings::INTERPRETERS + " folder.", Common_Strings::OK);
        this->Disable_All();
    }
    if (!this->Load_Interpreter(fileLocation)) {
        this->Show_Unable_To_Load_Plugin_Error();
    }
}

bool Main_Window::Populate_Writers() {
    QStringList writerPlugins = this->pluginHandler->Get_Writer_Plugins();
    if (writerPlugins.empty()) return false;
    this->ui->comboBaseGame->clear();
    this->ui->comboBaseGame->addItem(Common_Strings::SELECT_A_PLUGIN);
    this->ui->comboBaseGame->addItems(writerPlugins);
    return true;
}

bool Main_Window::Populate_Generators(const QString &writerPlugin) {
    QStringList generatorPlugins = this->pluginHandler->Get_Generator_Plugins(writerPlugin);
    if (generatorPlugins.empty()) {
        QString type = Common_Strings::GENERATORS.toLower(); type.chop(1);
        QMessageBox::critical(this, Common_Strings::LEVEL_HEADED, Common_Strings::LEVEL_HEADED +
                             " cannot find any compatible " + type + " plugins! Make sure that they are in the " +
                              Common_Strings::GENERATORS + " folder.", Common_Strings::OK);
        return false;
    }
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

void Main_Window::on_btnConfigureBaseGame_clicked(){
    if (!this->interpreterPlugin) this->Show_Unable_To_Load_Plugin_Error();
    this->interpreterPlugin->Configure_Writer();
}

void Main_Window::on_btnConfigureLevelGenerator_clicked(){
    if (!this->interpreterPlugin) this->Show_Unable_To_Load_Plugin_Error();
    this->interpreterPlugin->Configure_Generator();
}

void Main_Window::on_btnGenerateGame_clicked(){
    if (!this->interpreterPlugin) this->Show_Unable_To_Load_Plugin_Error();
    if (this->interpreterPlugin->Run()) {
        qDebug() << "Interpreter ran successfully";
    } else {
        qDebug() << "Something went wrong. Check debug info...";
    }
    //TODO: Add threading?
    //If threading is added, the other components of the UI will need to be locked down until the generation is complete
}

bool Main_Window::Load_Interpreter(const QString &fileLocation) {
    this->interpreterLoader = new QPluginLoader(fileLocation);
    QObject *validPlugin = this->interpreterLoader->instance();
    if (!validPlugin) return false;
    this->interpreterPlugin = qobject_cast<Interpreter_Interface*>(validPlugin);
    if (!this->interpreterPlugin) return false;
    this->interpreterPlugin->Startup(this, QApplication::applicationDirPath());
    return true;
}

void Main_Window::Show_Unable_To_Load_Plugin_Error() {
    QMessageBox::critical(this, Common_Strings::LEVEL_HEADED, Common_Strings::LEVEL_HEADED +
                         " is unable to load the interpreter plugin! The plugin may not be valid.",
                          Common_Strings::OK);
    this->Disable_All();
}
