#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <QStringList>

class Plugin_Settings;

class CLI_Parser {
public:
    CLI_Parser(QStringList *args, Plugin_Settings *pluginSettings);
    ~CLI_Parser();
    bool Parse_Args();
    bool Was_Help_Requested();
    void Show_Help();

private:
    bool Show_Syntax_Error();
    void Remove_Command_Identifier_Characters(QString &command);

    Plugin_Settings *pluginSettings;
    QStringList *args;
    bool helpRequested;
    QMap<QString, int> *commandMap;
};

#endif // CLI_PARSER_H
