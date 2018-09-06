#include "Main_Window.h"
#include "CLI_Passthrough.h"
#include "Plugin_Handler.h"
#include "Interpreter_Interface.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[]) {
    //Use Plastique Theme on Windows
    #ifdef Q_OS_WIN32
    QApplication::setStyle("plastique");
    #endif

    QApplication a(argc, argv);
    Main_Window w;

    qsrand(static_cast<unsigned int>(QTime::currentTime().msecsSinceStartOfDay()));
    if (!w.Create_Directories()) return 1;
    if (!w.Populate_Writers()) return 1;

    CLI_Passthrough cliPassthrough(argc, argv);
    if (cliPassthrough.Was_Command_Line_Mode_Requested()) { //Run in Command Line Mode
        return static_cast<int>(cliPassthrough.Run_Commands()) == 1;
    } else { //Run in GUI Mode
        w.show();
        w.Load_Previous_Plugins();
        return a.exec();
    }
}

