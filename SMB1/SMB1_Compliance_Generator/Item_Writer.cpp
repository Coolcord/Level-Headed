#include "Item_Writer.h"
#include <assert.h>

Item_Writer::Item_Writer(QTextStream *stream)
{
    assert(stream);
    this->stream = stream;
}

bool Item_Writer::Write_Item(Item_Type type, const QString &item) {
    QString line = "";
    switch (type) {
    case OBJECT:        line += "O: "; break;
    case ENEMY:         line += "E: "; break;
    case PIPE_POINTER:  line += "P: "; break;
    default:            return false;
    }
    line += item + "\n"; //only use Unix endlines
    *(this->stream) << line;
}

bool Item_Writer::Is_Coordinate_Valid(int coordinate) {
    return (coordinate >= 0x0 && coordinate <= 0xF);
}

bool Item_Writer::Are_Coordinates_Valid(int x, int y) {
    return (this->Is_Coordinate_Valid(x) && this->Is_Coordinate_Valid(y));
}
