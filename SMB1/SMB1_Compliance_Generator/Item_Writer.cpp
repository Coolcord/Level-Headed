#include "Item_Writer.h"
#include <assert.h>

Item_Writer::Item_Writer(QTextStream *stream, int numBytesLeft)
{
    assert(stream);
    assert(numBytesLeft >= 0);
    this->stream = stream;
    this->numBytesLeft = numBytesLeft;
}

bool Item_Writer::Write_Item(Item_Type type, const QString &item, bool handleBytes) {
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
    return true;
}

bool Item_Writer::Is_Coordinate_Valid(int coordinate) {
    return (coordinate >= 0x0 && coordinate <= 0xF);
}

bool Item_Writer::Are_Coordinates_Valid(int x, int y) {
    return (this->Is_Coordinate_Valid(x) && this->Is_Coordinate_Valid(y));
}

bool Item_Writer::Is_Safe_To_Write_Item(int numBytes) {
    return (this->numBytesLeft >= numBytes);
}
