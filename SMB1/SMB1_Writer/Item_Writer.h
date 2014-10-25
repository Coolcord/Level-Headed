#ifndef ITEM_WRITER_H
#define ITEM_WRITER_H

#include "Binary_Manipulator.h"

class Header_Writer;
class Room_ID_Handler;

class Item_Writer {
public:
    Item_Writer(QByteArray *buffer, Header_Writer *headerWriter, Room_ID_Handler *roomIDHandler);
    int Get_Current_Page();
    int Get_Current_X();
    int Get_Current_Y();
    bool Write_Coordinates(int x, int y, bool handlePageFlag = true);


protected:
    Item_Writer(const Item_Writer&);
    Item_Writer& operator=(const Item_Writer&);
    bool Is_Safe_To_Write_Item();
    bool Write_Item(int x, int y, int itemByte);
    bool Write_Byte_To_Buffer(int byte);
    int How_Many_Bytes_Left();

    QByteArray *buffer;
    Header_Writer *headerWriter;
    Room_ID_Handler *roomIDHandler;
    int bufferSize;
    int currentPage;
    int currentX;
    int currentY;
    int currentByte;
    bool pageFlag;
};

#endif // ITEM_WRITER_H
