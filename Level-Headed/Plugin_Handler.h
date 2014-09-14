#ifndef PLUGIN_HANDLER_H
#define PLUGIN_HANDLER_H

#include <QWidget>

class Plugin_Handler
{
public:
    Plugin_Handler();
    bool Create_Directories();
    bool Get_Plugins();


private:
    void Show_Read_Write_Error();

    QWidget *parent;
};

#endif // PLUGIN_HANDLER_H
