#ifndef PALETTE_H
#define PALETTE_H

#include "Byte_Writer.h"

class Palette : public Byte_Writer {
public:
    Palette(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Palette() {}

    bool Palette_Test();
};

#endif // PALETTE_H
