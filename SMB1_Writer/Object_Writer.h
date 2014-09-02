#ifndef OBJECT_WRITER_H
#define OBJECT_WRITER_H

#include "Item_Writer.h"

class Object_Writer : public Item_Writer
{
public:
    bool Question_Block_With_Mushroom(int x, int y);
    bool Question_Block_With_Coin(int x, int y);
    bool Hidden_Block_With_Coin(int x, int y);
private:
    bool Is_Safe_To_Write_Object();
    bool Write_Object(int x, int y, int objectByte);
};

#endif // OBJECT_WRITER_H
