#include "Main_Window.h"
#include "Plugin_Handler.h"
#include "Interpreter_Interface.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[]) {
    QApplication::setStyle("plastique");
    QApplication a(argc, argv);
    Main_Window w;

    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    if (!w.Create_Directories()) return 1;
    if (!w.Populate_Writers()) return 1;

    w.show();
    w.Load_Previous_Plugins();

    return a.exec();
}

