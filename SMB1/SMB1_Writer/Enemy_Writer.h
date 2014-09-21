#ifndef ENEMY_WRITER_H
#define ENEMY_WRITER_H

#include "Item_Writer.h"

class Enemy_Writer : public Item_Writer
{
public:
    Enemy_Writer(QByteArray *buffer, Header_Writer *headerWriter) : Item_Writer(buffer, headerWriter) {}
    bool Fill_Buffer();

    bool Green_Koopa(int x, int y, bool moving = true, bool onlyHardMode = false);
    bool Red_Koopa(int x, int y, bool onlyHardMode = false);
    bool Buzzy_Beetle(int x, int y, bool onlyHardMode = false);
    bool Hammer_Bro(int x, int y, bool onlyHardMode = false);
    bool Goomba(int x, int y, bool onlyHardMode = false);
    bool Blooper(int x, int y, bool onlyHardMode = false);
    bool Bullet_Bill(int x, int y, bool onlyHardMode = false);
    bool Green_Paratroopa(int x, int y, bool moving = true, bool leaping = true, bool onlyHardMode = false);
    bool Red_Paratroopa(int x, int y, bool onlyHardMode = false);
    bool Green_Cheep_Cheep(int x, int y, bool onlyHardMode = false);
    bool Red_Cheep_Cheep(int x, int y, bool onlyHardMode = false);
    bool Podoboo(int x, int y, bool onlyHardMode = false);
    bool Pirana_Plant(int x, int y, bool onlyHardMode = false);
    bool Lakitu(int x, int y, bool onlyHardMode = false);
    bool Spiny(int x, int y, bool onlyHardMode = false);
    bool Bowser_Fire_Spawner(int x, int y, bool onlyHardMode = false);
    bool Cheep_Cheep_Spawner(int x, int y, bool leaping = false, bool onlyHardMode = false);
    bool Bullet_Bill_Spawner(int x, int y, bool onlyHardMode = false);
    bool Fire_Bar(int x, int y, bool clockwise = true, bool fast = false, bool onlyHardMode = false);
    bool Large_Fire_Bar(int x, int y, bool onlyHardMode = false);
    bool Lift(int x, int y, bool vertical = true, bool onlyHardMode = false);
    bool Falling_Lift(int x, int y, bool onlyHardMode = false);
    bool Balance_Lift(int x, int y, bool onlyHardMode = false);
    bool Surfing_Lift(int x, int y, bool onlyHardMode = false);
    bool Lift_Spawner(int x, int y, bool up = true, bool small = false, bool onlyHardMode = false);
    bool Bowser(int x, bool onlyHardMode = false);
    bool Warp_Zone(int x);
    bool Toad(int x, bool onlyHardMode = false);
    bool Goomba_Group(int x, int y, int num, bool onlyHardMode = false);
    bool Koopa_Group(int x, int y, int num, bool onlyHardMode = false);
    bool Page_Change(int x, int page);
    bool Pipe_Pointer(int x, int room, int page);
    bool Nothing(int x);

private:
    Enemy_Writer(const Enemy_Writer&);
    Enemy_Writer& operator=(const Enemy_Writer&);
    bool Write_Enemy(int x, int y, int enemyByte, bool onlyHardMode);
};

#endif // ENEMY_WRITER_H
