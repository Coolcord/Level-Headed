#ifndef ITEM_WRITER_H
#define ITEM_WRITER_H

#include "Binary_Manipulator.h"

class Header_Writer;

class Item_Writer {
public:
    Item_Writer(QByteArray *buffer, Header_Writer *headerWriter);
    int Get_Current_Page();
    int Get_Current_X();
    int Get_Current_Y();
    bool Write_Coordinates(int x, int y);


protected:
    bool Is_Safe_To_Write_Item();
    bool Write_Item(int x, int y, int itemByte);
    bool Write_Byte_To_Buffer(int byte);
    int How_Many_Bytes_Left();

    QByteArray *buffer;
    Header_Writer *headerWriter;
    int bufferSize;
    int currentPage;
    int currentX;
    int currentY;
    int currentByte;
    bool pageFlag;
};

#endif // ITEM_WRITER_H
