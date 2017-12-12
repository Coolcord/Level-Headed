#ifndef HACKS_H
#define HACKS_H

#include "Byte_Writer.h"

class Hacks : public Byte_Writer
{
public:
    Hacks(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Hacks() {}
    bool Write_Watermark();
    bool Infinite_Lives();
    bool Set_Starting_Lives(int lives);
    bool Replace_Mario_With_Luigi();
    bool Add_Luigi_Game();
    bool Invincibility();
    bool Moon_Jump();
    bool Star_Color_Mario();
    bool Fireballs_Kill_Everything_Onscreen();
    bool Taking_Damage_As_Fire_Reverts_To_Super();
    bool Fix_Lakitu_Throw_Arc();
    bool Fast_Enemies(int speed);
};

#endif // HACKS_H
