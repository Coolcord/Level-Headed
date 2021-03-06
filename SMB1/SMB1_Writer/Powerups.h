#ifndef POWERUPS_H
#define POWERUPS_H

#include "Byte_Writer.h"
#include <QByteArray>

class Graphics;
class Hacks;
class Sequential_Archive_Handler;

class Powerups : public Byte_Writer {
public:
    Powerups(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler, Graphics *graphics, Hacks *hacks);
    bool Is_Primary_Powerup_Fire_Based();
    bool Replace_1UP_With_Poison_Mushroom();
    bool Replace_1UP_With_Poison_Mushroom_If_Not_Fire_Mario();
    bool Replace_1UP_With_Swimming_Mushroom();
    bool Replace_Fire_Flower_With_Bone_Caster();
    bool Replace_Fire_Flower_With_Coin_Flinger();
    bool Replace_Fire_Flower_With_Cutter_Flower();
    bool Replace_Fire_Flower_With_Double_Jump_Star();
    bool Replace_Fire_Flower_With_Fire_Star();
    bool Replace_Fire_Flower_With_Hammer_Suit();
    bool Replace_Fire_Flower_With_Luigis_Fire_Flower();
    bool Replace_Fire_Flower_With_Pink_Fury();
    bool Replace_Fire_Flower_With_Poison_Bubbles();
    bool Replace_Fire_Flower_With_Power_Wand();
    bool Replace_Fire_Flower_With_Pyro_Spark_Wand();
    bool Replace_Fire_Flower_With_Slime_Flower();
    bool Replace_Fire_Flower_With_Snowballs();
    bool Replace_Fire_Flower_With_Spinball_Flower();

private:
    bool Update_Fire_Mario_Skin_Color();
    bool Force_Green_Palettes_To_Be_Green();
    bool Set_Primary_Powerup_Is_Fire_Based(bool isFireBased);

    Graphics *graphics;
    Hacks *hacks;
    Sequential_Archive_Handler *sequentialArchiveHandler;
    bool primaryPowerupIsFireBased;
};

#endif // POWERUPS_H
