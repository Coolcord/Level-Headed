#ifndef ITEM_WRITER_H
#define ITEM_WRITER_H

#include <QFile>

class Item_Writer
{
public:
    Item_Writer(QFile *file);

protected:
    QFile *file;
};

#endif // ITEM_WRITER_H
