#ifndef HACKS_H
#define HACKS_H

#include "Byte_Writer.h"

class Hacks : public Byte_Writer
{
public:
    Hacks(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Hacks() {}
    bool Add_Luigi_Game();
    bool Always_Autoscroll();
    bool Disable_Intro_Demo();
    bool Enable_Hitting_Underwater_Blocks();
    bool Fast_Enemies(int speed);
    bool Fireballs_Kill_Everything_Onscreen();
    bool Fix_Lakitu_Throw_Arc();
    bool Hard_Mode_Does_Not_Affect_Lift_Size();
    bool Infinite_Lives();
    bool Invincibility();
    bool Moon_Jump();
    bool Replace_Castle_Loop_With_Autoscroll_Object(int overworldSpeed, int undergroundSpeed, int underwaterSpeed, int castleSpeed);
    bool Replace_Fire_Flower_With_Hammer_Suit();
    bool Replace_Mario_With_Luigi();
    bool Set_Starting_Lives(int lives);
    bool Star_Color_Mario();
    bool Start_Underwater_Castle_Brick_On_World(int world);
    bool Taking_Damage_As_Fire_Reverts_To_Super();
    bool Unlimited_Time();
    bool Write_Watermark();
};

#endif // HACKS_H
