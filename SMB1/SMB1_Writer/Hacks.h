#ifndef HACKS_H
#define HACKS_H

#include "Byte_Writer.h"

class Graphics;
class Midpoint_Writer;
class Sequential_Archive_Handler;
class Text;
class Powerups;

class Hacks : public Byte_Writer {
public:
    Hacks(QFile *file, Level_Offset *levelOffset, Midpoint_Writer *midpointWriter, Sequential_Archive_Handler *sequentialArchiveHandler, Text *text);
    ~Hacks() {}
    void Set_Graphics(Graphics *graphics);
    void Set_Powerups(Powerups *powerups);
    bool Was_Castle_Loop_Replaced_With_Autoscroll_Object();
    bool Was_Luigi_Game_Added();
    bool Was_Vertical_Object_Limit_Removed();
    bool Add_Luigi_Game();
    bool Always_Autoscroll();
    bool Always_Force_Continue_Code();
    bool Black_Piranha_Plants();
    bool Destroy_Bowser_Bridge_Randomly();
    bool Disable_Intro_Demo();
    bool Disable_One_Up_Coin_Limits();
    bool Enable_Hitting_Underwater_Blocks();
    bool Enable_Piranha_Plants_On_First_Level();
    bool Enable_Walking_Hammer_Bros(int difficulty);
    bool European_Blooper_Swim_Height();
    bool Fast_Leaping_Paratroopas();
    bool Fireballs_Kill_Everything_Onscreen();
    bool Fire_Bowser_Throws_A_Ludicrous_Amount_Of_Fireballs();
    bool Fire_Flower_As_Small_Mario_Upgrades_To_Fire_Mario();
    bool Fix_Lakitu_Throw_Arc();
    bool Fix_Life_Counter_Bugs();
    bool Hammer_Bros_Never_Jump();
    bool Hard_Mode_Does_Not_Affect_Lift_Size();
    bool Infinite_Lives();
    bool Invincibility();
    bool Mario_Stands_Still_At_Intro_Demo();
    bool Moon_Jump();
    bool Only_Jump_At_Intro_Demo();
    bool Permadeath();
    bool Random_Group_Enemy_Goomba(bool allowHammerBros);
    bool Random_Group_Enemy_Koopa(bool allowHammerBros);
    bool Random_Intro_Demo();
    bool Random_True_Bowser_Characters();
    bool Real_Time();
    bool Red_Piranha_Plants();
    bool Remove_Vertical_Object_Limit();
    bool Replace_1UP_With_Poison_Mushroom();
    bool Replace_1UP_With_Poison_Mushroom_If_Not_Fire_Mario();
    bool Replace_1UP_With_Swimming_Mushroom();
    bool Replace_Castle_Loop_With_Autoscroll_Object();
    bool Replace_Castle_Loop_With_Fire_Bros();
    bool Replace_Castle_Loop_With_Start_With_Fire_Flower();
    bool Replace_Castle_Loop_With_Top_Of_Flagpole_Gives_1UP();
    bool Replace_Mario_With_Luigi();
    bool Set_Basic_Enemy_Speed(int speed);
    bool Set_Bowser_Bridge_Destruction_Speed(int speed);
    bool Set_Bowser_Flame_Frequency(int frequency);
    bool Set_Brick_Break_Animation_Bounce_Height(int lowerHeight, int upperHeight);
    bool Set_Bullet_Bill_Firing_Rate(int rate);
    bool Set_Bullet_Bill_Speed(int speed);
    bool Set_Coin_Animation_Bounce_Height(int height);
    bool Set_Death_Animation_Jump_Height(int height);
    bool Set_Enemy_Group_Size(int value);
    bool Set_Enemy_Revival_Speed(int speed);
    bool Set_Firebar_Length(int length);
    bool Set_Flying_Cheep_Cheep_Jump_Height(int height);
    bool Set_Long_Firebar_Length(int length);
    bool Set_Maximum_Number_Of_Pirahna_Plants(int value);
    bool Set_Number_Of_Worlds(int value);
    bool Set_Number_Of_Levels_Per_World(int value);
    bool Set_Lakitu_Respawn_Speed(int value);
    bool Set_Starting_Lives(int lives);
    bool Set_Surfing_Lift_Speed(int speed);
    bool Set_Hammer_Bros_Throw_Rate(int easyRate, int hardRate);
    void Set_Hammer_Suit_Active(bool isHammerSuitActive);
    bool Slow_Leaping_Paratroopas();
    bool Speedy_Objects_And_Enemies();
    bool Spiny_Eggs_Do_Not_Break();
    bool Spiny_Eggs_Bouncy();
    bool Spiny_Eggs_Chase_Mario();
    bool Spiny_Eggs_Explode_Into_Flames(); //must be applied AFTER the powerups!
    bool Spiny_Eggs_No_Eggs();
    bool Start_With_Fire_Flower_On_Room_Change();
    bool Taking_Damage_As_Fire_Reverts_To_Super();
    bool Unlimited_Time();
    bool Write_Watermark();

private:
    bool Convert_Difficulty_To_World(int difficulty, int numWorlds, int &world);
    bool Enable_Walking_Hammer_Bros_In_World(int world);
    bool Increase_Leaping_Paratroopa_Speed(int amount);
    bool Increase_Spiny_Egg_Speed(int amount);
    bool Skip_Lives_Screen();
    bool Random_Enemy_Group(qint64 offset, bool allowHammerBros);


    Graphics *graphics;
    Powerups *powerups;
    Midpoint_Writer *midpointWriter;
    Sequential_Archive_Handler *sequentialArchiveHandler;
    Text *text;
    int difficultyWalkingHammerBros;
    int spinyEggSpeedCap;
    bool skipLivesScreen;
    bool isHammerSuitActive;
    bool permadeath;
    bool wasCastleLoopReplacedWithAutoScrollObject;
    bool wasCastleLoopReplacedWithFireBros;
    bool wasCastleLoopReplacedWithFlagpole1UP;
    bool wasCastleLoopReplacedWithFireFlower;
    bool wasVerticalObjectLimitRemoved;
};

#endif // HACKS_H
