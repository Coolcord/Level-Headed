#include "Main_Window.h"
#include "Plugin_Handler.h"
#include "Interpreter_Interface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Main_Window w;

    if (!w.Create_Directories()) return 1;
    if (!w.Populate_Writers()) return 1;

    w.show();

    return a.exec();
}

