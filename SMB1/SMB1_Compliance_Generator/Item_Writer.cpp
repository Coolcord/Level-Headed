#include "Item_Writer.h"
#include "Physics.h"
#include <assert.h>

Item_Writer::Item_Writer(QTextStream *stream, int numBytesLeft) {
    assert(stream);
    assert(numBytesLeft >= 0);
    this->stream = stream;
    this->numItems = 0;
    this->levelLength = 0;
    this->numBytesLeft = numBytesLeft;
    this->currentPage = 1;
    this->currentX = 1;
    this->currentY = Physics::GROUND_Y;
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

int Item_Writer::Get_Absolute_X(int x) {
    return ((this->currentX+x)%0x10);
}

void Item_Writer::Set_Coordinate_Safety(bool value) {
    this->coordinateSafety = value;
}

bool Item_Writer::Write_Item(Item_Type type, int x, const QString &item) {
    if (this->coordinateSafety && !this->Is_Coordinate_Valid(x))
        return false;
    if (!this->Is_Safe_To_Write_Item())
        return false;
    QString line = "";
    switch (type) {
    case OBJECT:        line += "O: "; break;
    case ENEMY:         line += "E: "; break;
    case PIPE_POINTER:  line += "P: "; break;
    default:            return false;
    }
    line += item + "\n"; //only use Unix endlines
    *(this->stream) << line;
    if (this->stream->status() != QTextStream::Ok) return false;

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

bool Item_Writer::Is_Coordinate_Valid(int coordinate) {
    return (coordinate >= 0x0 && coordinate <= 0x10);
}

bool Item_Writer::Is_Byte_Valid(int byte) {
    return (byte >= 0x00 && byte <= 0xFF);
}

bool Item_Writer::Is_Safe_To_Write_Item(int numBytes) {
    return (this->numBytesLeft >= numBytes);
}

bool Item_Writer::Handle_Level_Length_On_Page_Change(int page) {
    if (this->currentPage >= page) return false;
    this->currentPage = page;
    this->currentX = 0;
    this->levelLength = page*(0x10);
    return true;
}
