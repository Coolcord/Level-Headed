#ifndef REQUIRED_ENEMY_SPAWNS_H
#define REQUIRED_ENEMY_SPAWNS_H

#include "../Common_SMB1_Files/Enemy_Item.h"
#include "SMB1_Compliance_Generator_Arguments.h"
#include "Extra_Enemy_Args.h"
#include <QQueue>

class Buffer_Data;
class Object_Buffer;
class Enemy_Buffer;
class Pipe_Pointer_Buffer;

class Required_Enemy_Spawns {
public:
    Required_Enemy_Spawns(Object_Buffer *objects, Enemy_Buffer *enemies, Pipe_Pointer_Buffer *pipePointer, SMB1_Compliance_Generator_Arguments *args);
    ~Required_Enemy_Spawns();
    bool Add_Required_Enemy_Spawn(const Buffer_Data &data);
    bool Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemies, int x);
    bool Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemies, int x, int y);
    bool Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemies, Extra_Enemy_Args args, int x);
    bool Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemies, Extra_Enemy_Args args, int x, int y);
    bool Is_Safe_To_Add_Required_Enemy_Spawn(int x);
    int Get_Num_Bytes_Left();
    int Get_Num_End_Bytes();
    bool Set_Num_End_Bytes(int value);
    Extra_Enemy_Args Get_Initialized_Extra_Enemy_Args();
    bool Spawn_Required_Enemy(int &lastX);
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
        bool disableCoordinateSafety;
        Extra_Enemy_Args args;
    };

    bool Determine_Bytes_Required_For_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemies, bool &disableSafety, int x);

    SMB1_Compliance_Generator_Arguments *args;
    QQueue<Required_Enemy_Spawn> *requiredEnemies;
    Object_Buffer *objects;
    Enemy_Buffer *enemies;
    Pipe_Pointer_Buffer *pipePointer;
    int numRequiredBytes;
    int numEndBytes;
};

#endif // REQUIRED_ENEMY_SPAWNS_H
