#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDialog>
#include <QString>
#include <QPluginLoader>

class Plugin_Handler;
class Interpreter_Interface;

namespace Ui {
class Main_Window;
}

class Main_Window : public QDialog {
    Q_OBJECT

public:
    explicit Main_Window(QWidget *parent = 0);
    ~Main_Window();
    bool Create_Directories();
    bool Populate_Writers();
    bool Populate_Generators(const QString &writerPlugin);
    bool Load_Previous_Plugins();

private slots:
    void Disable_All();
    void Enable_Generator();
    void Enable_Buttons();
    void on_comboBaseGame_currentIndexChanged(const QString &arg1);
    void on_comboLevelGenerator_currentIndexChanged(const QString &arg1);
    void on_btnConfigureSettings_clicked();
    void on_btnGenerateGame_clicked();
    void on_Main_Window_finished(int result);

private:
    bool Load_Interpreter(const QString &fileLocation);
    void Show_Unable_To_Load_Plugin_Error();

    Ui::Main_Window *ui;
    Plugin_Handler *pluginHandler;
    QPluginLoader *interpreterLoader;
    Interpreter_Interface *interpreterPlugin;
};

#endif // MAIN_WINDOW_H
