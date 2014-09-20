#ifndef ITEM_HANDLER_H
#define ITEM_HANDLER_H

#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include <QString>

class Item_Handler
{
public:
    Item_Handler(SMB1_Writer_Interface *writerPlugin);

protected:
    bool Parse_Num(const QString &value, int &num);

    SMB1_Writer_Interface *writerPlugin;
};

#endif // ITEM_HANDLER_H
