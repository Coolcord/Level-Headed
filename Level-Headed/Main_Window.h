#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "Update_Checker.h"
#include <QApplication>
#include <QDialog>
#include <QString>
#include <QPluginLoader>

class Plugin_Handler;
class Interpreter_Interface;
class Readable_Config_File;

namespace Ui {
class Main_Window;
}

class Main_Window : public QDialog {
    Q_OBJECT

public:
    Main_Window(QWidget *parent, QApplication *application);
    ~Main_Window();
    void Check_For_Updates();
    bool Create_Directories();
    bool Populate_Writers();
    bool Populate_Generators(const QString &writerPlugin);
    bool Load_Previous_Plugins();
    void Shutdown();

private slots:
    void Disable_All();
    void Enable_Generator();
    void Enable_Buttons();
    void on_comboBaseGame_currentIndexChanged(const QString &arg1);
    void on_comboLevelGenerator_currentIndexChanged(const QString &arg1);
    void on_btnConfigureSettings_clicked();
    void on_btnGenerateGame_clicked();
    void on_Main_Window_finished();
    void on_Update_Available(const QString &newVersion, const QString &updatePage);

private:
    bool Load_Interpreter(const QString &fileLocation);
    void Show_Unable_To_Load_Plugin_Error();

    Ui::Main_Window *ui;
    Update_Checker *updateThread;
    Readable_Config_File *readableConfigFile;
    Plugin_Handler *pluginHandler;
    QPluginLoader *interpreterLoader;
    Interpreter_Interface *interpreterPlugin;
};

#endif // MAIN_WINDOW_H
