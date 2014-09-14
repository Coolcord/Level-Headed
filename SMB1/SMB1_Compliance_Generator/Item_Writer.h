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
    int Get_Current_Y();

protected:
    bool Write_Item(Item_Type type, int x, const QString &item);
    bool Is_Coordinate_Valid(int coordinate);
    bool Is_Byte_Valid(int byte);
    bool Is_Safe_To_Write_Item(int numBytes = 2);
    bool Handle_Level_Length_On_Page_Change(int page);
    QTextStream *stream;
    int numBytesLeft;
    int numItems;
    int levelLength;
    int currentX;
    int currentY;
    int currentPage;
};

#endif // ITEM_WRITER_H
