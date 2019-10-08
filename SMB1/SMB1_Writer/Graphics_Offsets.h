#ifndef GRAPHICS_OFFSETS_H
#define GRAPHICS_OFFSETS_H

#include <QStack>
#include <QMap>

class Graphics_Offsets {
public:
    Graphics_Offsets();
    ~Graphics_Offsets();
    QByteArray *Get_Values_At_Offset(qint64 offset);
    QByteArray *Get_Values_At_Offset_And_Never_Fail(qint64 offset);

    //Sprite Offsets
    QStack<qint64> Get_Air_Bubble_Offsets();
    QStack<qint64> Get_Blooper_Offsets();
    QStack<qint64> Get_Bowser_Offsets();
    QStack<qint64> Get_Bowser_Fire_Offsets();
    QStack<qint64> Get_Brick_Piece_Offsets();
    QStack<qint64> Get_Bullet_Bill_Offsets();
    QStack<qint64> Get_Buzzy_Beetle_Offsets();
    QStack<qint64> Get_Castle_Flag_Offsets();
    QStack<qint64> Get_Cheep_Cheep_Offsets();
    QStack<qint64> Get_Coin_Animation_Offsets();
    QStack<qint64> Get_Explosion_Offsets();
    QStack<qint64> Get_Fireball_Offsets();
    QStack<qint64> Get_Fire_Flower_Offsets();
    QStack<qint64> Get_Flagpole_Flag_Offsets();
    QStack<qint64> Get_Goomba_Offsets();
    QStack<qint64> Get_Hammer_Offsets();
    QStack<qint64> Get_Hammer_Bro_Offsets();
    QStack<qint64> Get_Jump_Spring_Base_Offsets();
    QStack<qint64> Get_Jump_Spring_Animation_Offsets();
    QStack<qint64> Get_Koopa_Offsets();
    QStack<qint64> Get_Lakitu_Offsets();
    QStack<qint64> Get_Lift_Offsets();
    QStack<qint64> Get_Mario_Offsets();
    QStack<qint64> Get_Mushroom_Powerup_Offsets();
    QStack<qint64> Get_One_Up_Font_Offsets();
    QStack<qint64> Get_Peach_Offsets();
    QStack<qint64> Get_Piranha_Plant_Offsets();
    QStack<qint64> Get_Podoboo_Offsets();
    QStack<qint64> Get_Score_Font_Offsets();
    QStack<qint64> Get_Sky_Lift_Offsets();
    QStack<qint64> Get_Spiny_Offsets();
    QStack<qint64> Get_Spiny_Egg_Offsets();
    QStack<qint64> Get_Starman_Offsets();
    QStack<qint64> Get_Toad_Offsets();

    //Background Offsets
    QStack<qint64> Get_Axe_Offsets();
    QStack<qint64> Get_Brick_Block_Offsets();
    QStack<qint64> Get_Brick_Block_Animation_Offsets();
    QStack<qint64> Get_Bowser_Bridge_Offsets();
    QStack<qint64> Get_Bridge_Offsets();
    QStack<qint64> Get_Bullet_Bill_Cannon_Offsets();
    QStack<qint64> Get_Castle_Block_Offsets();
    QStack<qint64> Get_Chain_Offsets();
    QStack<qint64> Get_Cloud_Block_Offsets();
    QStack<qint64> Get_Coin_Offsets();
    QStack<qint64> Get_Coin_Icon_Offsets();
    QStack<qint64> Get_Coral_Offsets();
    QStack<qint64> Get_Flagpole_Offsets();
    QStack<qint64> Get_Font_Offsets();
    QStack<qint64> Get_Mushroom_Platform_Offsets();
    QStack<qint64> Get_Overworld_Block_Offsets();
    QStack<qint64> Get_Pipe_Offsets();
    QStack<qint64> Get_Question_Block_Offsets();
    QStack<qint64> Get_Rope_Offsets();
    QStack<qint64> Get_Selector_Icon_Offsets();
    QStack<qint64> Get_Solid_Block_Offsets();
    QStack<qint64> Get_Tree_Platform_Offsets();
    QStack<qint64> Get_Underwater_Block_Offsets();
    QStack<qint64> Get_Vine_Offsets();
    QStack<qint64> Get_Water_Offsets();

private:
    void Initialize_Tile_Order_Map();
    void Deallocate_Tile_Order_Map();

    QMap<qint64, QByteArray*> *tileOrderMap;
};

#endif // GRAPHICS_OFFSETS_H
