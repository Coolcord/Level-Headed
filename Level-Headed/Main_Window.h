#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "Plugin_Handler.h"
#include <QDialog>

namespace Ui {
class Main_Window;
}

class Main_Window : public QDialog
{
    Q_OBJECT

public:
    explicit Main_Window(QWidget *parent = 0);
    ~Main_Window();
    Plugin_Handler *pluginHandler;

private slots:
    void Disable_All();
    void Enable_Generator();
    void Enable_Buttons();

    void on_comboBaseGame_editTextChanged(const QString &arg1);

private:
    Ui::Main_Window *ui;
};

#endif // MAIN_WINDOW_H
