#ifndef ENEMY_BUFFER_H
#define ENEMY_BUFFER_H

#include "Item_Buffer.h"
#include "../Common_SMB1_Files/Enemy_Item.h"
#include "../Common_SMB1_Files/Level.h"
#include <QFile>
#include <QLinkedList>
#include <QString>

class Buffer_Data;

class Enemy_Buffer : public Item_Buffer {
public:
    Enemy_Buffer(int numBytesLeft);
    ~Enemy_Buffer();
    bool Write_Buffer_To_File(QFile *file);
    bool Get_First_Enemy();
    void Set_First_Enemy(bool value);
    bool Is_Lakitu_Active();
    bool Was_Lakitu_Spawned();

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
    bool Podoboo(int x, bool onlyHardMode = false);
    bool Piranha_Plant(int x, int y, bool onlyHardMode = false);
    bool Lakitu(int x, int y, bool onlyHardMode = false);
    bool Spiny(int x, int y, bool onlyHardMode = false);
    bool Bowser_Fire_Spawner(int x, bool onlyHardMode = false);
    bool Swimming_Cheep_Cheep_Spawner(int x, bool leaping = false, bool onlyHardMode = false);
    bool Bullet_Bill_Spawner(int x, bool onlyHardMode = false);
    bool Fire_Bar(int x, int y, bool clockwise = true, bool fast = false, bool onlyHardMode = false);
    bool Large_Fire_Bar(int x, int y, bool onlyHardMode = false);
    bool Lift(int x, int y, bool vertical = true, bool onlyHardMode = false);
    bool Falling_Lift(int x, int y, bool onlyHardMode = false);
    bool Balance_Lift(int x, int y, bool onlyHardMode = false);
    bool Surfing_Lift(int x, int y, bool onlyHardMode = false);
    bool Lift_Spawner(int x, int y, bool up = true, bool small = false, bool onlyHardMode = false);
    bool Bowser(int x, bool onlyHardMode = false);
    bool Warp_Zone(int x, bool onlyHardMode = false);
    bool Toad(int x, bool onlyHardMode = false);
    bool Goomba_Group(int x, int y, int num, bool onlyHardMode = false);
    bool Koopa_Group(int x, int y, int num, bool onlyHardMode = false);
    bool Page_Change(int page);

private:
    Enemy_Buffer(const Enemy_Buffer&);
    Enemy_Buffer& operator=(const Enemy_Buffer&);
    bool Write_Enemy(int page);
    bool Write_Enemy(int x, Level::Level level, int world, int page);
    bool Write_Enemy(Enemy_Item::Enemy_Item enemyItem, Buffer_Data &args, int x);
    bool Write_Enemy(Enemy_Item::Enemy_Item enemyItem, Buffer_Data &args, int x, int y);
    bool Is_Coordinate_Valid(int coordinate);
    QString Get_String_From_Enemy_Item(Enemy_Item::Enemy_Item enemyItem);
    QString Get_String_From_Level(Level::Level level);

    bool firstEnemy;
    bool lakituIsActive;
    bool wasLakituSpawned;

    friend class Pipe_Pointer_Buffer;
};

#endif // ENEMY_BUFFER_H
