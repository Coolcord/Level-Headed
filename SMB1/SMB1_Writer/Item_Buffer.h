#ifndef ITEM_BUFFER_H
#define ITEM_BUFFER_H

#include "Binary_Manipulator.h"

class Header_Writer;
class Room_ID_Handler;

class Item_Buffer {
public:
    Item_Buffer(QByteArray *buffer, Header_Writer *headerWriter, Room_ID_Handler *roomIDHandler);
    int Get_Current_Page();
    int Get_Current_X();
    int Get_Current_Y();
    bool Write_Coordinates(int x, int y, bool handlePageFlag = true);

protected:
    Item_Buffer(const Item_Buffer&);
    Item_Buffer& operator=(const Item_Buffer&);
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

#endif // ITEM_BUFFER_H
