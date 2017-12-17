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
    this->ui->btnGenerateGame->setText(Common_Strings::STRING_GENERATE_A_GAME);
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
    fileLocation = QApplication::applicationDirPath() + "/" + Common_Strings::STRING_PLUGINS + "/" +
                         Common_Strings::STRING_INTERPRETERS + "/" + fileLocation; //set the full path
    QFile interpreter(fileLocation);
    if (fileLocation.isEmpty() || !interpreter.exists()) {
        QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED, Common_Strings::STRING_LEVEL_HEADED +
                             " cannot find the interpreter plugin! Make sure that it is in the " +
                              Common_Strings::STRING_INTERPRETERS + " folder.", Common_Strings::STRING_OK);
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
    this->ui->comboBaseGame->addItem(Common_Strings::STRING_SELECT_A_PLUGIN);
    this->ui->comboBaseGame->addItems(writerPlugins);
    return true;
}

bool Main_Window::Populate_Generators(const QString &writerPlugin) {
    QStringList generatorPlugins = this->pluginHandler->Get_Generator_Plugins(writerPlugin);
    if (generatorPlugins.empty()) {
        QString type = Common_Strings::STRING_GENERATORS.toLower().trimmed();
        QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED, Common_Strings::STRING_LEVEL_HEADED +
                             " cannot find any compatible " + type + " plugins! Make sure that they are in the " +
                              Common_Strings::STRING_GENERATORS + " folder.", Common_Strings::STRING_OK);
        return false;
    }
    this->ui->comboLevelGenerator->clear();
    this->ui->comboLevelGenerator->addItem(Common_Strings::STRING_SELECT_A_PLUGIN);
    this->ui->comboLevelGenerator->addItems(generatorPlugins);
    return true;
}

bool Main_Window::Load_Previous_Plugins() {
    QString writerPlugin = QString();
    QString generatorPlugin = QString();
    if (!this->pluginHandler->Get_Previously_Loaded_Plugins(writerPlugin, generatorPlugin)) return false;
    this->ui->comboBaseGame->setCurrentText(writerPlugin);
    this->ui->comboLevelGenerator->setCurrentText(generatorPlugin);
    return true;
}

void Main_Window::on_comboBaseGame_currentIndexChanged(const QString &arg1) {
    if (arg1 == NULL || arg1.isEmpty() || arg1 == Common_Strings::STRING_SELECT_A_PLUGIN) {
        this->Disable_All();
        return;
    } else if (this->Populate_Generators(arg1)){
        this->Enable_Generator();
        return;
    }
}

void Main_Window::on_comboLevelGenerator_currentIndexChanged(const QString &arg1) {
    if (arg1 == NULL || arg1.isEmpty() || arg1 == Common_Strings::STRING_SELECT_A_PLUGIN) {
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
        this->ui->btnGenerateGame->setText(Common_Strings::STRING_GENERATE_ANOTHER);
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
    QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED, Common_Strings::STRING_LEVEL_HEADED +
                         " is unable to load the interpreter plugin! The plugin may not be valid.",
                          Common_Strings::STRING_OK);
    this->Disable_All();
}

void Main_Window::on_Main_Window_finished(int result) {
    if (result == 0) {
        //The output of this really doesn't matter at this point
        this->pluginHandler->Save_Currently_Loaded_Plugins(this->ui->comboBaseGame->currentText(), this->ui->comboLevelGenerator->currentText());
    }
    delete this->pluginHandler;
    this->pluginHandler = NULL;
}
