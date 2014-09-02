#include "Object_Writer.h"

bool Object_Writer::Is_Safe_To_Write_Object() {
    return (this->How_Many_Bytes_Left() >= 2);
}

bool Object_Writer::Write_Object(int x, int y, int objectByte) {
    if (!this->Is_Safe_To_Write_Object()) return false;
    if (!this->Write_Coordinates(x, y)) return false;
    return this->Write_Byte_To_Buffer(objectByte);
}

bool Object_Writer::Question_Block_With_Mushroom(int x, int y) {
    return this->Write_Object(x, y, 0x0);
}

bool Object_Writer::Question_Block_With_Coin(int x, int y) {
    return this->Write_Object(x, y, 0x1);
}
