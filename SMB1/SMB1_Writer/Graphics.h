#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Byte_Writer.h"

class Graphics : public Byte_Writer
{
public:
    Graphics(QFile *file, Level_Offset *levelOffset);
    ~Graphics() {}
    bool Dream_Mario_Bros();
    bool Lost_Levels();
    bool Paradyce();
    bool Pocket_Edition();
    bool Super_Duper_Mario_Bros();
    bool Super_Mario_Bros_DX();
    bool Super_Mario_MCB();
    bool Super_Mario_S();
    bool Is_Axe_Rope_Broken();

private:
    bool brokenAxeRope;
};

#endif // GRAPHICS_H
