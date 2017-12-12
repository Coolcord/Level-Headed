#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Byte_Writer.h"

class Graphics : public Byte_Writer
{
public:
    Graphics(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Graphics() {}
    bool Pocket_Edition();
};

#endif // GRAPHICS_H
