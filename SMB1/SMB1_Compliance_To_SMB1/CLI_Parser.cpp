#include "CLI_Parser.h"
#include "Plugin_Settings.h"
#include <assert.h>
#include <QDebug>
#include <QFileInfo>

CLI_Parser::CLI_Parser(QStringList *args, Plugin_Settings *pluginSettings) {
    assert(args); assert(pluginSettings);
    this->args = args;
    this->pluginSettings = pluginSettings;
    this->helpRequested = false;

    //Populate the Possible Commands
    this->commandMap = new QMap<QString, int>();
    this->commandMap->insert("help", 0);
    this->commandMap->insert("h", 0);
    this->commandMap->insert("?", 0);
    this->commandMap->insert("scripts", 1);
    this->commandMap->insert("s", 1);
}

CLI_Parser::~CLI_Parser() {
    delete this->commandMap;
}

bool CLI_Parser::Parse_Args() {
    for (int i = 3; i < this->args->size(); ++i) {
        QString tmpString = this->args->at(i);
        this->Remove_Command_Identifier_Characters(tmpString);

        QMap<QString, int>::iterator iter = this->commandMap->find(tmpString);
        if (iter == this->commandMap->end()) return this->Show_Syntax_Error();
        switch (iter.value()) {
        default:
            this->helpRequested = true;
            return false;
        case 0: //Show help
            this->helpRequested = true;
            return true;
        case 1: //Use Scripts
            this->pluginSettings->generateNewLevels = false;
            ++i;
            if (i >= this->args->size()) return this->Show_Syntax_Error();
            this->pluginSettings->levelScripts = this->args->at(i);
            break;
        }
    }
    return true;
}

bool CLI_Parser::Was_Help_Requested() {
    return this->helpRequested;
}

void CLI_Parser::Show_Help() {
    QString callCommand = this->args->at(0)+" -i "+this->args->at(2);
    qDebug() << "";

    //Generate a new game
    qDebug().noquote() << callCommand;
    qDebug() << "    Generates a new game using the previous config.";
    qDebug() << "    If no config is found, the default values will be used.";
    qDebug() << "    The previous config file can be changed by running in GUI mode and changing the settings there.";
    qDebug() << "";

    //Compile scripts
    qDebug().noquote() << callCommand << "-s [.lvls archive]";
    qDebug() << "    Generates a game using the specified level scripts and the previous config.";
    qDebug() << "    The .lvls archive should be in the Levels folder.";
    qDebug() << "    The specified .lvls archive should only be the name.";
    qDebug() << "    It should not include the path or the .lvls extension.";
    qDebug() << "";

    //Help
    qDebug().noquote() << callCommand << "--help";
    qDebug() << "    Shows this message";
}

bool CLI_Parser::Show_Syntax_Error() {
    qDebug() << "Syntax Error!";
    this->Show_Help();
    return false;
}

void CLI_Parser::Remove_Command_Identifier_Characters(QString &command) {
    if (command.startsWith("/")) command = command.remove(0, 1);
    else if (command.startsWith("--")) command = command.remove(0, 2);
    else if (command.startsWith("-")) command = command.remove(0, 1);
}
