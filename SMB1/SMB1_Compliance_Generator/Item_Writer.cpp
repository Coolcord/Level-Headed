#include "Item_Writer.h"
#include <assert.h>

Item_Writer::Item_Writer(QTextStream *stream, int numBytesLeft)
{
    assert(stream);
    assert(numBytesLeft >= 0);
    this->stream = stream;
    this->numItems = 0;
    this->levelLength = 0;
    this->numBytesLeft = numBytesLeft;
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

bool Item_Writer::Write_Item(Item_Type type, int x, const QString &item, bool handleBytes) {
    if (!this->Is_Coordinate_Valid(x)) return false;
    if (handleBytes && !this->Is_Safe_To_Write_Item()) return false;
    QString line = "";
    switch (type) {
    case OBJECT:        line += "O: "; break;
    case ENEMY:         line += "E: "; break;
    case PIPE_POINTER:  line += "P: "; break;
    default:            return false;
    }
    line += item + "\n"; //only use Unix endlines
    *(this->stream) << line;
    if (handleBytes) this->numBytesLeft -= 2;
    ++this->numItems;
    this->levelLength += x;
    return true;
}

bool Item_Writer::Is_Coordinate_Valid(int coordinate) {
    return (coordinate >= 0x0 && coordinate <= 0xF);
}

bool Item_Writer::Is_Safe_To_Write_Item(int numBytes) {
    return (this->numBytesLeft >= numBytes);
}
