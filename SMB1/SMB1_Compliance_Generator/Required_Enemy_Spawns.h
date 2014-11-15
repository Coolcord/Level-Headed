#ifndef REQUIRED_ENEMY_SPAWNS_H
#define REQUIRED_ENEMY_SPAWNS_H

#include "../Common SMB1 Files/Enemy_Item.h"
#include <QQueue>

class Object_Writer;
class Enemy_Writer;

class Required_Enemy_Spawns
{
public:
    Required_Enemy_Spawns(Object_Writer *object, Enemy_Writer *enemy);
    ~Required_Enemy_Spawns();
    bool Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x, int y);
    int Get_Num_Required_Enemy_Spawns();
    int Get_Required_Bytes();
    Enemy_Item::Enemy_Item Get_Enemy();
    int Get_X();
    int Get_Y();
    bool Mark_Enemy_As_Spawned();

private:
    struct Required_Enemy_Spawn {
        Enemy_Item::Enemy_Item enemy;
        int x;
        int y;
        int numRequiredBytes;
    };

    bool Determine_Bytes_Required_For_Required_Enemy_Spawn(int x);

    QQueue<Required_Enemy_Spawn> *requiredEnemies;
    Object_Writer *object;
    Enemy_Writer *enemy;
    int numRequiredBytes;
};

#endif // REQUIRED_ENEMY_SPAWNS_H
