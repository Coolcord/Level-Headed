#ifndef REQUIRED_ENEMY_SPAWNS_H
#define REQUIRED_ENEMY_SPAWNS_H

#include "../Common SMB1 Files/Enemy_Item.h"
#include "Extra_Enemy_Args.h"
#include <QQueue>

class Object_Writer;
class Enemy_Writer;
class Pipe_Pointer_Writer;

class Required_Enemy_Spawns
{
public:
    Required_Enemy_Spawns(Object_Writer *object, Enemy_Writer *enemy, Pipe_Pointer_Writer *pipePointer);
    ~Required_Enemy_Spawns();
    bool Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x);
    bool Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x, int y);
    bool Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, Extra_Enemy_Args args, int x);
    bool Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, Extra_Enemy_Args args, int x, int y);
    Extra_Enemy_Args Get_Initialized_Extra_Enemy_Args();
    bool Spawn_Required_Enemy(int lastX);
    bool Is_In_Range_Of_Required_Enemy(int x);
    int Get_Num_Required_Enemy_Spawns();
    int Get_Num_Required_Bytes();
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
        Extra_Enemy_Args args;
    };

    bool Determine_Bytes_Required_For_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x);

    QQueue<Required_Enemy_Spawn> *requiredEnemies;
    Object_Writer *object;
    Enemy_Writer *enemy;
    Pipe_Pointer_Writer *pipePointer;
    int numRequiredBytes;
};

#endif // REQUIRED_ENEMY_SPAWNS_H
