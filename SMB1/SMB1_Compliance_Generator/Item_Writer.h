#ifndef ITEM_WRITER_H
#define ITEM_WRITER_H

#include "Item_Type.h"
#include <QTextStream>
#include <QString>

class Item_Writer
{
public:
    Item_Writer(QTextStream *stream);

protected:
    bool Write_Item(Item_Type type, const QString &item);
    bool Is_Coordinate_Valid(int coordinate);
    bool Are_Coordinates_Valid(int x, int y);
    QTextStream *stream;
};

#endif // ITEM_WRITER_H
