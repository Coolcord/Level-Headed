#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDialog>

class Plugin_Handler;

namespace Ui {
class Main_Window;
}

class Main_Window : public QDialog
{
    Q_OBJECT

public:
    explicit Main_Window(QWidget *parent = 0);
    ~Main_Window();
    bool Create_Directories();
    bool Populate_Writers();
    bool Populate_Generators(const QString &writerPlugin);

private slots:
    void Disable_All();
    void Enable_Generator();
    void Enable_Buttons();

    void on_comboBaseGame_currentIndexChanged(const QString &arg1);

    void on_comboLevelGenerator_currentIndexChanged(const QString &arg1);

private:
    Ui::Main_Window *ui;
    Plugin_Handler *pluginHandler;
};

#endif // MAIN_WINDOW_H
