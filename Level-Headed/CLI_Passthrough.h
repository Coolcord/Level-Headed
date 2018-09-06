#ifndef CLI_PASSTHROUGH_H
#define CLI_PASSTHROUGH_H

#include <QStringList>

class CLI_Passthrough {
public:
    CLI_Passthrough(int argc, char *argv[], const QString &applicationLocation);
    ~CLI_Passthrough();
    bool Run_Commands();
    bool Was_Command_Line_Mode_Requested();

private:
    void Show_Help();
    void Show_Invalid_Number_Of_Arguments_Message();
    void Show_Unable_To_Load_Plugin_Message();
    void Remove_Command_Identifier_Characters(QString &command);

    QString applicationLocation;
    QStringList *args;
    bool commandLineModeRequested;
    bool showHelpMessage;
};

#endif // CLI_PASSTHROUGH_H
