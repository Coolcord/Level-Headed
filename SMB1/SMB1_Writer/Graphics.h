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
    bool Paradyce_SMB1();
    bool Paradyce_SMB2();
    bool Pocket_Edition();
    bool Super_Duper_Mario_Bros();
    bool Super_Mario_Bros_DX_v2();
    bool Super_Mario_Bros_DX_v3_4();
    bool Super_Mario_MCB();
    bool Super_Mario_S();
    bool Is_Axe_Rope_Broken();

private:
    bool brokenAxeRope;
};

#endif // GRAPHICS_H
