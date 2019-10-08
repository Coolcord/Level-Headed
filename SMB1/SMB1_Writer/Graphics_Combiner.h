#ifndef GRAPHICS_COMBINER_H
#define GRAPHICS_COMBINER_H

#include "Byte_Writer.h"
#include <QByteArray>
#include <QFile>
#include <QMap>
#include <QStack>

class Graphics;
class Graphics_Offsets;
class Sequential_Archive_Handler;

class Graphics_Combiner : public Byte_Writer {
public:
    Graphics_Combiner(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler, Graphics *graphics);
    ~Graphics_Combiner();
    QString Get_Brick_Patch_Name();
    QString Get_Mario_Patch_Name();

    bool Combine_All_Except_Mario();

    //Sprites
    bool Combine_Air_Bubble();
    bool Combine_Blooper();
    bool Combine_Bowser();
    bool Combine_Bowser_Fire();
    bool Combine_Brick_Piece();
    bool Combine_Bullet_Bill();
    bool Combine_Buzzy_Beetle();
    bool Combine_Castle_Flag();
    bool Combine_Cheep_Cheep();
    bool Combine_Coin_Animation();
    bool Combine_Explosion();
    bool Combine_Fireball();
    bool Combine_Fire_Flower();
    bool Combine_Flagpole_Flag();
    bool Combine_Goomba();
    bool Combine_Hammer();
    bool Combine_Hammer_Bro();
    bool Combine_Jump_Spring();
    bool Combine_Koopa();
    bool Combine_Lakitu();
    bool Combine_Lift();
    bool Combine_Mario();
    bool Combine_Mushroom_Powerup();
    bool Combine_One_Up_Font();
    bool Combine_Peach();
    bool Combine_Piranha_Plant();
    bool Combine_Podoboo();
    bool Combine_Score_Font();
    bool Combine_Sky_Lift();
    bool Combine_Spiny();
    bool Combine_Spiny_Egg();
    bool Combine_Starman();
    bool Combine_Toad();

    //Background
    bool Combine_Axe();
    bool Combine_Brick_Block();
    bool Combine_Bowser_Bridge();
    bool Combine_Bridge();
    bool Combine_Bullet_Bill_Cannon();
    bool Combine_Castle_Block();
    bool Combine_Chain();
    bool Combine_Cloud_Block();
    bool Combine_Coin();
    bool Combine_Coin_Icon();
    bool Combine_Coral();
    bool Combine_Flagpole();
    bool Combine_Font();
    bool Combine_Mushroom_Platform();
    bool Combine_Overworld_Block();
    bool Combine_Pipe();
    bool Combine_Question_Block();
    bool Combine_Rope();
    bool Combine_Selector_Icon();
    bool Combine_Solid_Block();
    bool Combine_Tree_Platform();
    bool Combine_Underwater_Block();
    bool Combine_Vine();
    bool Combine_Water();

private:
    bool Does_Graphics_Pack_Use_New_Tiles(QStack<qint64> offsets, bool sprite);
    bool Is_Tile_Blank(char tileID, bool sprite);

    QString marioPatchName;
    QString brickPatchName;
    Graphics *graphics;
    Graphics_Offsets *graphicsOffsets;
    Sequential_Archive_Handler *sequentialArchiveHandler;
};

#endif // GRAPHICS_COMBINER_H
