#ifndef PLUGIN_HANDLER_H
#define PLUGIN_HANDLER_H

#include "Main_Window.h"
#include <QWidget>

class Main_Window;

class Plugin_Handler {
public:
    Plugin_Handler(Main_Window *window);
    ~Plugin_Handler() {}
    bool Create_Directories();
    QStringList Get_Writer_Plugins();
    QStringList Get_Generator_Plugins(const QString &writerPlugin);
    QString Get_Interpreter_Name(QString writerPlugin, QString generatorPlugin);
    bool Save_Currently_Loaded_Plugins(const QString &writerPlugin, const QString &generatorPlugin);
    bool Get_Previously_Loaded_Plugins(QString &writerPlugin, QString &generatorPlugin);

private:
    void Show_Read_Write_Error();
    QStringList Get_Plugins_From_Folder(const QString &folder);

    Main_Window *window;
    QWidget *widget;
};

#endif // PLUGIN_HANDLER_H
