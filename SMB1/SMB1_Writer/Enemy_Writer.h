#ifndef ENEMY_WRITER_H
#define ENEMY_WRITER_H

#include "Item_Writer.h"

class Enemy_Writer : public Item_Writer
{
public:
    Enemy_Writer(QByteArray *buffer, Header_Writer *headerWriter) : Item_Writer(buffer, headerWriter) {}

    bool Green_Koopa(int x, int y, bool moving);
    bool Red_Koopa(int x, int y);
    bool Buzzy_Beetle(int x, int y);
    bool Hammer_Bro(int x, int y);
    bool Goomba(int x, int y);
    bool Blooper(int x, int y);
    bool Bullet_Bill(int x, int y);
    bool Green_Paratroopa(int x, int y, bool moving, bool leaping);
    bool Red_Paratroopa(int x, int y);
    bool Green_Cheep_Cheep(int x, int y);
    bool Red_Cheep_Cheep(int x, int y);
    bool Podoboo(int x, int y);
    bool Pirana_Plant(int x, int y);
    bool Lakitu(int x, int y);
    bool Spiny(int x, int y);
    bool Bowser_Fire(int x, int y);
    bool Cheep_Cheep_Spawner(int x, int y, bool leaping);
    bool Bullet_Bill_Spawner(int x, int y);
    bool Fire_Bar(int x, int y, bool clockwise, bool fast);
    bool Large_Fire_Bar(int x, int y);
    bool Lift(int x, int y, bool vertical);
    bool Falling_Lift(int x, int y);
    bool Balance_Lift(int x, int y);
    bool Surfing_Lift(int x, int y);
    bool Lift_Spawner(int x, int y, bool up, bool small);
    bool Bowser(int x);
    bool Warp_Zone(int x, int y);
    bool Toad(int x, int y);
    bool Goomba_Group(int x, int y, int num);
    bool Koopa_Group(int x, int y, int num);
    bool Page_Skip(int x, int amount);
    bool Nothing(int x);

private:
    bool Write_Enemy(int x, int y, int enemyByte);
};

#endif // ENEMY_WRITER_H
