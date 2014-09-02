#include "Item_Writer.h"
#include <assert.h>

Item_Writer::Item_Writer(QByteArray *buffer) {
    assert(buffer);
    this->buffer = buffer;
    this->bufferSize = this->buffer->size();
    this->currentPage = 0;
    this->currentX = 0;
    this->currentY = 0;
    this->pageFlag = false;
    this->currentByte = 0;
}

int Item_Writer::Get_Current_Page() {
    return this->currentPage;
}

int Item_Writer::Get_Current_X() {
    return this->currentX;
}

int Item_Writer::Get_Current_Y() {
    return this->currentY;
}

bool Item_Writer::Write_Coordinates(int x, int y) {
    if (y < 0x0 || y > 0xF) return false; //make sure the y is valid

    //Handle the x coordinate
    int tmpX = this->currentX + x;
    bool tmpPageFlag = this->pageFlag;
    if (tmpX > 0xF) { //set the page flag if necessary
        if (tmpPageFlag) return false;
        tmpPageFlag = true;
        tmpX -= 0x10;
    }
    if (tmpX > 0xF) return false; //the x coordinate cannot be off the next page

    //Set the member variables
    this->currentX = tmpX;
    this->currentY = y;
    this->pageFlag = tmpPageFlag;

    //TODO: Find a way to write this to the buffer
    QBitArray positionBits(8, false);
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(positionBits, 0, this->currentX); //x
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(positionBits, 4, this->currentY); //y
    int positionByte = Binary_Manipulator::BitArray_To_Hex(positionBits); //get the byte
    return this->Write_Byte_To_Buffer(positionByte);
}

bool Item_Writer::Write_Byte_To_Buffer(int byte) {
    if (this->currentByte >= this->bufferSize) return false; //no more space!
    this->buffer[this->currentByte] = QString(byte).toUtf8().data(); //write the byte
    ++this->currentByte;
    return true;
}

int Item_Writer::How_Many_Bytes_Left() {
    return (this->bufferSize - this->currentByte+1);
}
