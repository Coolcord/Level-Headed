#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Byte_Writer.h"

class Graphics : public Byte_Writer
{
public:
    Graphics(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Graphics() {}
    bool Lost_Levels();
    bool Super_Mario_MCB();
    bool Pocket_Edition();
    bool Super_Mario_Bros_DX();
    bool Super_Mario_S();
};

#endif // GRAPHICS_H
