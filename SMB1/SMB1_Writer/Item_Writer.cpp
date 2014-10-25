#include "Item_Writer.h"
#include "Header_Writer.h"
#include "Room_ID_Handler.h"
#include <assert.h>
#include <QDebug>

Item_Writer::Item_Writer(QByteArray *buffer, Header_Writer *headerWriter, Room_ID_Handler *roomIDHandler) {
    assert(buffer);
    assert(headerWriter);
    assert(roomIDHandler);
    this->buffer = buffer;
    this->headerWriter = headerWriter;
    this->roomIDHandler = roomIDHandler;
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

bool Item_Writer::Is_Safe_To_Write_Item() {
    return (this->How_Many_Bytes_Left() >= 2);
}

bool Item_Writer::Write_Item(int x, int y, int itemByte) {
    if (!this->Is_Safe_To_Write_Item()) return false;
    if (!this->Write_Coordinates(x, y)) return false;
    if (this->pageFlag) {
        this->pageFlag = false;
        itemByte += 128; //set the page flag in the byte
        assert(itemByte <= 0xFF);
    }
    return this->Write_Byte_To_Buffer(itemByte);
}

bool Item_Writer::Write_Coordinates(int x, int y, bool handlePageFlag) {
    if (y < 0x0 || y > 0xF) return false; //make sure the y is valid

    //Handle the x coordinate
    int tmpX = this->currentX + x;
    bool tmpPageFlag = this->pageFlag;
    if (handlePageFlag && tmpX > 0xF) { //set the page flag if necessary
        if (tmpPageFlag) return false;
        tmpPageFlag = true;
        tmpX -= 0x10;
    }
    if (tmpX > 0xF) return false; //the x coordinate cannot be off the next page

    //Set the member variables
    this->currentX = tmpX;
    this->currentY = y;
    if (handlePageFlag) this->pageFlag = tmpPageFlag;

    QBitArray positionBits(8, false);
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(positionBits, 0, this->currentX); //x
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(positionBits, 4, this->currentY); //y
    int positionByte = Binary_Manipulator::BitArray_To_Hex(positionBits); //get the byte
    return this->Write_Byte_To_Buffer(positionByte);
}

bool Item_Writer::Write_Byte_To_Buffer(int byte) {
    if (this->currentByte >= this->bufferSize) return false; //no more space!
    this->buffer->data()[this->currentByte] = static_cast<char>(byte); //write the byte
    ++this->currentByte;
    return true;
}

int Item_Writer::How_Many_Bytes_Left() {
    return (this->bufferSize - this->currentByte);
}
