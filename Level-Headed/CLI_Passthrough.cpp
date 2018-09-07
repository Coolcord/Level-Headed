#include "CLI_Passthrough.h"
#include "Common_Strings.h"
#include "Interpreter_Interface.h"
#include <QDebug>
#include <QPluginLoader>

CLI_Passthrough::CLI_Passthrough(int argc, char *argv[], const QString &applicationLocation) {
    this->applicationLocation = applicationLocation;
    this->commandLineModeRequested = false;
    this->showHelpMessage = false;
    this->args = new QStringList();
    for (int i = 0; i < argc; ++i) this->args->push_back(QString(argv[i]));

    //Determine if we are running in command line mode
    if (this->args->size() > 1) {
        QString tmpString = this->args->at(1).toLower();
        this->Remove_Command_Identifier_Characters(tmpString);
        this->args->replace(1, tmpString);
        this->commandLineModeRequested = tmpString == "i";
        this->showHelpMessage = tmpString == "help" || tmpString == "h" || tmpString == "?";
    }
}

CLI_Passthrough::~CLI_Passthrough() {
    delete this->args;
}

bool CLI_Passthrough::Run_Commands() {
    if (this->showHelpMessage) {
        this->Show_Help();
        return true;
    }
    if (!this->commandLineModeRequested) return false;
    if (this->args->size() < 3) {
        this->Show_Invalid_Number_Of_Arguments_Message();
        return false;
    }

    //Load the requested plugin
    QString interpreterFileLocation = this->applicationLocation + "/" + Common_Strings::STRING_PLUGINS + "/" +
            Common_Strings::STRING_INTERPRETERS + "/" + this->args->at(2) + Common_Strings::STRING_PLUGIN_EXTENSION;
    QPluginLoader *interpreterLoader = new QPluginLoader(interpreterFileLocation);
    QObject *validPlugin = interpreterLoader->instance();
    if (!validPlugin) {
        this->Show_Unable_To_Load_Plugin_Message();
        return false;
    }
    Interpreter_Interface *interpreterPlugin = qobject_cast<Interpreter_Interface*>(validPlugin);
    if (!interpreterPlugin) {
        this->Show_Unable_To_Load_Plugin_Message();
        return false;
    }

    //Run in CLI Mode
    interpreterPlugin->Startup(nullptr, this->applicationLocation, *this->args);
    bool success = interpreterPlugin->Run_CLI();

    //Unload the Interpreter Plugin
    if (interpreterPlugin) interpreterPlugin->Shutdown();
    if (interpreterLoader) interpreterLoader->unload();
    delete interpreterLoader;
    interpreterLoader = nullptr;
    interpreterPlugin = nullptr;
    return success;
}

bool CLI_Passthrough::Was_Command_Line_Mode_Requested() {
    return this->commandLineModeRequested || this->showHelpMessage;
}

void CLI_Passthrough::Show_Help() {
    qDebug() << "";

    //Basic Commands
    qDebug().noquote() << this->args->at(0) << "-i [Interpreter Plugin] [Plugin Args]";
    qDebug() << "    For normal usage";
    qDebug() << "";

    //Basic Commands
    qDebug().noquote() << this->args->at(0) << "-i [Interpreter Plugin] --help";
    qDebug() << "    Shows plugin specific arguments and commands";
    qDebug() << "";

    //Help
    qDebug().noquote() << this->args->at(0) << "--help";
    qDebug() << "    Shows this message";
}

void CLI_Passthrough::Show_Invalid_Number_Of_Arguments_Message() {
    qDebug() << "Invalid number of arguments";
}

void CLI_Passthrough::Show_Unable_To_Load_Plugin_Message() {
    qDebug() << "Unable to load "+this->args->at(2)+Common_Strings::STRING_PLUGIN_EXTENSION+"!";
}

void CLI_Passthrough::Remove_Command_Identifier_Characters(QString &command) {
    if (command.startsWith("/")) command = command.remove(0, 1);
    else if (command.startsWith("--")) command = command.remove(0, 2);
    else if (command.startsWith("-")) command = command.remove(0, 1);
}
