#include "Main_Window.h"
#include "ui_Main_Window.h"
#include "Plugin_Handler.h"
#include "Interpreter_Interface.h"
#include "Update_Dialog.h"
#include "Update_Thread.h"
#include "Common_Strings.h"
#include "../Common_Files/Version.h"
#include "../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h"
#include <QWindow>
#include <QFile>
#include <QPluginLoader>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QUrl>

Main_Window::Main_Window(QWidget *parent, QApplication *application) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Main_Window)
{
    assert(application);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    this->setWindowTitle(Common_Strings::STRING_LEVEL_HEADED+" "+Version::VERSION);
    this->ui->btnGenerateGame->setText(Common_Strings::STRING_GENERATE_A_GAME);
    this->readableConfigFile = new Readable_Config_File();
    this->pluginHandler = new Plugin_Handler(this, this->readableConfigFile);
    this->interpreterLoader = nullptr;
    this->interpreterPlugin = nullptr;
    this->updateThread = new Update_Thread(this, application, this->readableConfigFile, Version::VERSION_NUMBER, QApplication::applicationDirPath()+"/"+Common_Strings::STRING_PLUGINS+"/Git/bin/git");
    connect(this->updateThread, SIGNAL(Update_Available(const QString&, const QString&)), this, SLOT(on_Update_Available(const QString&, const QString&)));
}

Main_Window::~Main_Window() {
    delete ui;
    this->Shutdown();
}

void Main_Window::Check_For_Updates() {
    this->updateThread->start();
}

bool Main_Window::Create_Directories() {
    return this->pluginHandler->Create_Directories();
}

void Main_Window::Disable_All() {
    this->ui->lblLevelGenerator->setEnabled(false);
    this->ui->comboLevelGenerator->clear();
    this->ui->comboLevelGenerator->setEnabled(false);
    this->ui->btnConfigureSettings->setEnabled(false);
    this->ui->btnGenerateGame->setEnabled(false);
    if (this->interpreterPlugin) this->interpreterPlugin->Shutdown();
    if (this->interpreterLoader) this->interpreterLoader->unload();
    delete this->interpreterLoader;
    this->interpreterLoader = nullptr;
    this->interpreterPlugin = nullptr;
    this->ui->comboBaseGame->setCurrentIndex(0);
}

void Main_Window::Enable_Generator() {
    this->ui->lblLevelGenerator->setEnabled(true);
    this->ui->comboLevelGenerator->setEnabled(true);
    this->ui->btnConfigureSettings->setEnabled(false);
    this->ui->btnGenerateGame->setEnabled(false);
    if (this->interpreterPlugin) this->interpreterPlugin->Shutdown();
    if (this->interpreterLoader) this->interpreterLoader->unload();
    delete this->interpreterLoader;
    this->interpreterLoader = nullptr;
    this->interpreterPlugin = nullptr;
}

void Main_Window::Enable_Buttons() {
    this->ui->btnConfigureSettings->setEnabled(true);
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

void Main_Window::Shutdown() {
    if (this->interpreterPlugin) this->interpreterPlugin->Shutdown();
    if (this->interpreterLoader) this->interpreterLoader->unload();
    delete this->interpreterLoader;
    this->interpreterLoader = nullptr;
    this->interpreterPlugin = nullptr;
    delete this->pluginHandler;
    this->pluginHandler = nullptr;
    if (this->updateThread) this->updateThread->terminate();
    delete this->updateThread;
    this->updateThread = nullptr;
    delete this->readableConfigFile;
    this->readableConfigFile = nullptr;
}

void Main_Window::on_comboBaseGame_currentIndexChanged(const QString &arg1) {
    if (arg1 == nullptr || arg1.isEmpty() || arg1 == Common_Strings::STRING_SELECT_A_PLUGIN) {
        this->Disable_All();
        return;
    } else if (this->Populate_Generators(arg1)){
        this->Enable_Generator();
        return;
    }
}

void Main_Window::on_comboLevelGenerator_currentIndexChanged(const QString &arg1) {
    if (arg1 == nullptr || arg1.isEmpty() || arg1 == Common_Strings::STRING_SELECT_A_PLUGIN) {
        this->Enable_Generator();
        return;
    } else {
        this->Enable_Buttons();
        return;
    }
}

void Main_Window::on_btnConfigureSettings_clicked() {
    if (!this->interpreterPlugin) this->Show_Unable_To_Load_Plugin_Error();
    this->interpreterPlugin->Configure_Settings();
}

void Main_Window::on_btnGenerateGame_clicked(){
    this->ui->btnGenerateGame->setText(Common_Strings::STRING_GENERATING);
    this->ui->btnGenerateGame->repaint();
    if (!this->interpreterPlugin) this->Show_Unable_To_Load_Plugin_Error();
    if (this->interpreterPlugin->Run()) {
        qDebug() << "Interpreter ran successfully";
    } else {
        qDebug() << "Something went wrong. Check debug info...";
    }
    this->ui->btnGenerateGame->setText(Common_Strings::STRING_GENERATE_A_GAME);
}

bool Main_Window::Load_Interpreter(const QString &fileLocation) {
    this->interpreterLoader = new QPluginLoader(fileLocation);
    QObject *validPlugin = this->interpreterLoader->instance();
    if (!validPlugin) return false;
    this->interpreterPlugin = qobject_cast<Interpreter_Interface*>(validPlugin);
    if (!this->interpreterPlugin) return false;
    this->interpreterPlugin->Startup(this, QApplication::applicationDirPath(), QStringList());
    return true;
}

void Main_Window::Show_Unable_To_Load_Plugin_Error() {
    QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED, Common_Strings::STRING_LEVEL_HEADED +
                         " is unable to load the interpreter plugin! The plugin may not be valid.",
                          Common_Strings::STRING_OK);
    this->Disable_All();
}

void Main_Window::on_Main_Window_finished() {
    this->pluginHandler->Save_Currently_Loaded_Plugins(this->ui->comboBaseGame->currentText(), this->ui->comboLevelGenerator->currentText());
    this->Shutdown();
}

void Main_Window::on_Update_Available(const QString &newVersion, const QString &updatePage) {
    Update_Dialog(this, this->readableConfigFile, newVersion, updatePage).exec();
}
