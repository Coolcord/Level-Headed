#include "Item_Writer.h"
#include <assert.h>

Item_Writer::Item_Writer(QFile *file)
{
    assert(file);
    this->file = file;
}
