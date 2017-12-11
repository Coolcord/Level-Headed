#ifndef HACKS_HANDLER_H
#define HACKS_HANDLER_H

#include "Plugin_Settings.h"

class SMB1_Writer_Interface;

class Hacks_Handler
{
public:
    Hacks_Handler(SMB1_Writer_Interface *writerPlugin, Plugin_Settings *pluginSettings);
    ~Hacks_Handler();
    bool Write_Hacks();

private:
    bool Handle_Lives();
    bool Handle_God_Mode();
    bool Handle_Play_As_Luigi();
    bool Handle_Lakitu_Throw_Arc();
    bool Get_Bool_From_CheckState(Qt::CheckState checkState);

    SMB1_Writer_Interface *writerPlugin;
    Plugin_Settings *pluginSettings;
};

#endif // HACKS_HANDLER_H
