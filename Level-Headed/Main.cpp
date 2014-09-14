#include "Main_Window.h"
#include "Plugin_Handler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Main_Window w;

    Plugin_Handler *pluginHandler = new Plugin_Handler();
    if (!pluginHandler->Create_Directories()) {
        delete pluginHandler;
        return 1;
    }

    w.show();

    delete pluginHandler;
    return a.exec();
}

