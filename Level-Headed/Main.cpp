#include "Main_Window.h"
#include "CLI_Passthrough.h"
#include "Plugin_Handler.h"
#include "Interpreter_Interface.h"
#include "../Common_Files/Random.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    #ifdef Q_OS_WIN32
    a.setStyle("fusion"); //use fusion theme on Windows
    #endif
    Main_Window w(nullptr, &a);

    Random::Get_Instance().Seed(QString::number(QTime::currentTime().msecsSinceStartOfDay()), 1);
    if (!w.Create_Directories()) return 1;
    if (!w.Populate_Writers()) return 1;

    CLI_Passthrough cliPassthrough(argc, argv, QApplication::applicationDirPath());
    if (cliPassthrough.Was_Command_Line_Mode_Requested()) { //Run in Command Line Mode
        return static_cast<int>(cliPassthrough.Run_Commands()) == 1;
    } else { //Run in GUI Mode
        w.show();
        w.Load_Previous_Plugins();
        w.Check_For_Updates();
        return a.exec();
    }
}

