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
    void Set_Current_Y(int value);
    int Get_Absolute_X(int x);
    int Get_Current_Page();
    void Set_Coordinate_Safety(bool value);
    bool Will_Page_Flag_Be_Tripped(int x);

protected:
    Item_Writer(const Item_Writer&);
    Item_Writer& operator=(const Item_Writer&);
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
    bool coordinateSafety;
};

#endif // ITEM_WRITER_H
