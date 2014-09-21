#include "Pipe_Pointer_Writer.h"
#include "Enemy_Writer.h"
#include "Object_Writer.h"
#include "../Common SMB1 Files/Enemy_Item_String.h"
#include "../Common SMB1 Files/Object_Item_String.h"
#include "Physics.h"
#include <assert.h>
#include <QString>

Pipe_Pointer_Writer::Pipe_Pointer_Writer(Object_Writer *object, Enemy_Writer *enemy) {
    assert(object);
    assert(enemy);
    this->object = object;
    this->enemy = enemy;
}

bool Pipe_Pointer_Writer::Is_Safe_To_Write_Pipe_Pointer() {
    if (this->object->Get_Num_Bytes_Left() < 2) return false;
    return (this->enemy->Get_Num_Bytes_Left() >= 3);
}

bool Pipe_Pointer_Writer::Pipe_Pointer(int x, int room, int page) {
    if (this->enemy->Get_Num_Bytes_Left() < 3) return false;
    if (!this->enemy->Is_Byte_Valid(room) || !this->enemy->Is_Byte_Valid(page)) return false;
    if (!this->enemy->Is_Coordinate_Valid(x)) return false;
    --this->enemy->numBytesLeft;
    return this->enemy->Write_Item(PIPE_POINTER, x, QString(Enemy_Item::STRING_PIPE_POINTER+" "+QString::number(room)+" "+QString::number(page)));
}

bool Pipe_Pointer_Writer::Enterable_Pipe(int x, int y, int height, int room, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (height < 1 || height > 16) return false;
    if (!this->Pipe_Pointer(x, room, page)) return false;
    return this->object->Write_Object(x, y, Object_Item::STRING_ENTERABLE_PIPE, QString::number(height), Physics::PIPE_LENGTH, true);
}

bool Pipe_Pointer_Writer::Reverse_L_Pipe(int x, int room, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (!this->Pipe_Pointer(x, room, page)) return false;
    return this->object->Write_Object(x, Object_Item::STRING_REVERSE_L_PIPE, Physics::REVERSE_L_PIPE_LENGTH);
}

bool Pipe_Pointer_Writer::Tall_Reverse_L_Pipe(int x, int yPlacement, int room, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (yPlacement < 0x1 || yPlacement > 0xA) return false;
    if (!this->Pipe_Pointer(x, room, page)) return false;
    return this->object->Write_Object(x, yPlacement, Object_Item::STRING_TALL_REVERSE_L_PIPE, Physics::REVERSE_L_PIPE_LENGTH, false);
}

bool Pipe_Pointer_Writer::Underwater_Sideways_Pipe(int x, int y, int room, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (!this->Pipe_Pointer(x, room, page)) return false;
    return this->object->Write_Object(x, y, Object_Item::STRING_UNDERWATER_SIDEWAYS_PIPE, Physics::MIN_OBJECT_LENGTH, true);
}

bool Pipe_Pointer_Writer::Brick_With_Vine(int x, int y, int room, int page) {
    if (!this->Is_Safe_To_Write_Pipe_Pointer()) return false;
    if (!this->Pipe_Pointer(x, room, page)) return false;
    return this->object->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_VINE, Physics::MIN_OBJECT_LENGTH, true);
}
