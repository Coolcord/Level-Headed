#ifndef ITEM_WRITER_H
#define ITEM_WRITER_H

#include "Item_Type.h"
#include <QTextStream>
#include <QString>

class Item_Writer
{
public:
    Item_Writer(QTextStream *stream, int numBytesLeft);
    int Get_Num_Bytes_Left();
    int Get_Num_Items();
    int Get_Level_Length();

protected:
    bool Write_Item(Item_Type type, int x, const QString &item, bool decrementBytes = true);
    bool Is_Coordinate_Valid(int coordinate);
    bool Is_Safe_To_Write_Item(int numBytes = 2);
    QTextStream *stream;
    int numBytesLeft;
    int numItems;
    int levelLength;
};

#endif // ITEM_WRITER_H
