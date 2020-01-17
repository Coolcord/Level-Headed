#include "Item_Writer.h"
#include "../../Level-Headed/Common_Strings.h"
#include "Physics.h"
#include <assert.h>
#include <QDebug>

Item_Writer::Item_Writer(int numBytesLeft) {
    assert(numBytesLeft >= 0);
    this->buffer = new Text_Insertion_Buffer();
    this->numItems = 0;
    this->levelLength = 0;
    this->numBytesLeft = numBytesLeft;
    this->currentPage = 0;
    this->currentX = 0;
    this->currentY = Physics::GROUND_Y+1;
    this->coordinateSafety = true;
}

int Item_Writer::Get_Num_Bytes_Left() {
    return this->numBytesLeft;
}

int Item_Writer::Get_Num_Items() {
    return this->numItems;
}

int Item_Writer::Get_Level_Length() {
    return this->levelLength;
}

int Item_Writer::Get_Current_Y() {
    return this->currentY;
}

void Item_Writer::Set_Current_Y(int value) {
    this->currentY = value;
}

int Item_Writer::Get_Absolute_X(int x) {
    return ((this->currentX+x)%0x10);
}

int Item_Writer::Get_Current_Page() {
    return this->currentPage;
}

void Item_Writer::Set_Coordinate_Safety(bool value) {
    this->coordinateSafety = value;
}

bool Item_Writer::Will_Page_Flag_Be_Tripped(int x) {
    int simulatedCurrentX = this->currentX + x;
    return (simulatedCurrentX > 0xF);
}

bool Item_Writer::Write_Buffer_To_File(QFile *file) {
    return this->buffer->Write_To_File(file);
}

Text_Insertion_Buffer *Item_Writer::Get_Buffer() {
    return this->buffer;
}

bool Item_Writer::Write_Item(int x, const QString &item) {
    assert(!this->coordinateSafety || this->Is_Coordinate_Valid(x));
    assert(this->Is_Safe_To_Write_Item());
    this->buffer->Insert_At_End(item);

    //Keep track of the level stats
    this->numBytesLeft -= 2;
    this->levelLength += x;
    this->currentX += x;
    if (this->currentX > 0xF) {
        this->currentX %= 0x10;
        ++this->currentPage;
    }
    ++this->numItems;
    return true;
}

bool Item_Writer::Is_Byte_Valid(int byte) {
    return (byte >= 0x00 && byte <= 0xFF);
}

bool Item_Writer::Is_Safe_To_Write_Item(int numBytes) {
    return (this->numBytesLeft >= numBytes);
}

bool Item_Writer::Handle_Level_Length_On_Page_Change(int page) {
    if (this->currentPage >= page) {
        qDebug() << "Current Page: " << this->currentPage;
        qDebug() << "Page attempting to change to: " << page;
        return false;
    }
    this->currentPage = page;
    this->currentX = 0;
    this->levelLength = page*(0x10);
    return true;
}
