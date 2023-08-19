#include "Pipe_Pointer_Buffer.h"
#include "Enemy_Buffer.h"
#include "Object_Buffer.h"
#include "Required_Enemy_Spawns.h"
#include "../Common_SMB1_Files/Enemy_Item_String.h"
#include "../Common_SMB1_Files/Level.h"
#include "../Common_SMB1_Files/Object_Item_String.h"
#include "Physics.h"
#include <assert.h>
#include <QString>

Pipe_Pointer_Buffer::Pipe_Pointer_Buffer(Object_Buffer *objects, Enemy_Buffer *enemies, Required_Enemy_Spawns *requiredEnemySpawns) {
    assert(objects);
    assert(enemies);
    assert(requiredEnemySpawns);
    this->objects = objects;
    this->enemies = enemies;
    this->requiredEnemySpawns = requiredEnemySpawns;
}

bool Pipe_Pointer_Buffer::Pipe_Pointer(int x, Level::Level level, int world, int page) {
    if (this->enemies->Get_Num_Bytes_Left() < 3) return false;
    Extra_Enemy_Args args = this->requiredEnemySpawns->Get_Initialized_Extra_Enemy_Args();
    args.level = level; args.world = world; args.page = page;
    return this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::PIPE_POINTER, args, x);
}

bool Pipe_Pointer_Buffer::Enterable_Pipe(int x, int y, int height, Level::Level level, int world, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (height < 1 || height > 16) return false;
    if (!this->Pipe_Pointer(x, level, world, page)) return false;
    return this->objects->Enterable_Pipe_Without_Pointer(x, y, height);
}

bool Pipe_Pointer_Buffer::Reverse_L_Pipe(int x, Level::Level level, int world, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (!this->Pipe_Pointer(x, level, world, page)) return false;
    return this->objects->Reverse_L_Pipe_Without_Pointer(x);
}

bool Pipe_Pointer_Buffer::Tall_Reverse_L_Pipe(int x, int yPlacement, Level::Level level, int world, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (yPlacement < 0x3 || yPlacement > 0xA) return false;
    if (!this->Pipe_Pointer(x, level, world, page)) return false;
    return this->objects->Tall_Reverse_L_Pipe_Without_Pointer(x, yPlacement);
}

bool Pipe_Pointer_Buffer::Underwater_Sideways_Pipe(int x, int y, Level::Level level, int world, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (!this->Pipe_Pointer(x, level, world, page)) return false;
    return this->objects->Underwater_Sideways_Pipe_Without_Pointer(x, y);
}

bool Pipe_Pointer_Buffer::Brick_With_Vine(int x, int y, Level::Level level, int world, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (!this->Pipe_Pointer(x, level, world, page)) return false;
    return this->objects->Brick_With_Vine_Without_Pointer(x, y);
}

bool Pipe_Pointer_Buffer::Is_Safe_To_Write_Pipe_Pointer() {
    if (this->objects->Get_Num_Bytes_Left() < 2) return false;
    return (this->enemies->Get_Num_Bytes_Left() >= 3);
}
