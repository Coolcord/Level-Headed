#ifndef HACKS_H
#define HACKS_H

#include "Byte_Writer.h"

class Sequential_Archive_Handler;

class Hacks : public Byte_Writer {
public:
    Hacks(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler);
    ~Hacks() {}
    bool Was_Castle_Loop_Replaced_With_Autoscroll_Object();
    bool Add_Luigi_Game();
    bool Always_Autoscroll();
    bool Black_Piranha_Plants();
    bool Bouncy_Spiny_Eggs();
    bool Disable_Intro_Demo();
    bool Enable_Hitting_Underwater_Blocks();
    bool Enable_Piranha_Plants_On_First_Level();
    bool Enable_Walking_Hammer_Bros(int difficulty);
    bool Fireballs_Kill_Everything_Onscreen();
    bool Fix_Lakitu_Throw_Arc();
    bool Fix_Life_Counter_Bugs();
    bool Hard_Mode_Does_Not_Affect_Lift_Size();
    bool Infinite_Lives();
    bool Invincibility();
    bool Moon_Jump();
    bool Permadeath();
    bool Real_Time();
    bool Red_Piranha_Plants();
    bool Remove_Vertical_Object_Limit();
    bool Replace_1UP_With_Poison_Mushroom();
    bool Replace_1UP_With_Poison_Mushroom_If_Not_Fire_Mario();
    bool Replace_1UP_With_Swimming_Mushroom();
    bool Replace_Castle_Loop_With_Autoscroll_Object();
    bool Replace_Castle_Loop_With_Fire_Bros();
    bool Replace_Mario_With_Luigi();
    bool Set_Basic_Enemy_Speed(int speed);
    bool Set_Bullet_Bill_Speed(int speed);
    bool Set_Number_Of_Worlds(int value);
    bool Set_Lakitu_Respawn_Speed(int value);
    bool Set_Starting_Lives(int lives);
    void Set_Hammer_Suit_Active(bool isHammerSuitActive);
    bool Speedy_Objects_And_Enemies();
    bool Start_Underwater_Castle_Brick_On_World(int world);
    bool Start_With_Fire_Flower_On_Room_Change();
    bool Taking_Damage_As_Fire_Reverts_To_Super();
    bool Unlimited_Time();
    bool Write_Watermark();

private:
    bool Convert_Difficulty_To_World(int difficulty, int numWorlds, int &world);
    bool Enable_Walking_Hammer_Bros_In_World(int world);

    Sequential_Archive_Handler *sequentialArchiveHandler;
    int difficultyWalkingHammerBros;
    bool isHammerSuitActive;
    bool wasCastleLoopReplacedWithAutoScrollObject;
};

#endif // HACKS_H
