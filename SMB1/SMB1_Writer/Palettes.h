#ifndef PALETTE_H
#define PALETTE_H

#include "Byte_Writer.h"

class Colors;

class Palettes : public Byte_Writer {
public:
    Palettes(QFile *file, Level_Offset *levelOffset);
    ~Palettes();
    bool Randomize_Palettes();

private:
    bool Coin_Palette_Random();
    bool Sky_Palette_Random();

    Colors *colors;
};

#endif // PALETTE_H
