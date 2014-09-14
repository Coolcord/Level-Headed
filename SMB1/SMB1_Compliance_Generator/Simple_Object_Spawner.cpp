#include "Simple_Object_Spawner.h"
#include "Object_Writer.h"
#include "Physics.h"
#include <assert.h>
#include <QTime>

Simple_Object_Spawner::Simple_Object_Spawner(Object_Writer *object) : Spawner() {
    assert(object);
    this->object = object;
}

int Simple_Object_Spawner::Get_Random_Y() {
    int y = this->object->Get_Current_Y();
    bool up = static_cast<bool>((qrand() % 2));
    if (up) {
        y -= qrand() % (Physics::BASIC_JUMP_HEIGHT+1);
        if (y < Physics::HIGHEST_Y) y = Physics::HIGHEST_Y; //make sure y is valid
    } else {
        y += qrand() % (Physics::BASIC_JUMP_HEIGHT+1);
        if (y > Physics::GROUND_Y) y = Physics::GROUND_Y; //make sure y is valid
    }
    return y;
}

int Simple_Object_Spawner::Get_Random_Pipe_Y() {
    int y = this->Get_Random_Y() - 1;
    if (y < Physics::HIGHEST_Y) y = Physics::HIGHEST_Y; //make sure y is valid
    return y;
}

int Simple_Object_Spawner::Get_Random_Length() {
    return ((qrand() % Physics::MAX_OBJECT_LENGTH)+1);
}

int Simple_Object_Spawner::Get_Random_Hole_Length() {
    return ((qrand() % Physics::RUNNING_JUMP_LENGTH)+1);
}

int Simple_Object_Spawner::Get_Random_Steps_Size() {
    return (((qrand() % (Physics::MIN_STEPS_SIZE+Physics::MAX_STEPS_SIZE))+1)-Physics::MIN_STEPS_SIZE);
}

bool Simple_Object_Spawner::Spawn_Simple_Object() {
    int x = this->Get_Random_X(this->object->Get_Last_Object_Length());
    int random = qrand() % this->PROBABILITY_TOTAL_SIMPLE;
    if (random <= PROBABILITY_HORIZONTAL_BRICKS) {
        return this->object->Horizontal_Bricks(x, Physics::BASIC_BLOCK_Y, this->Get_Random_Length());
    } else if (PROBABILITY_HORIZONTAL_BLOCKS) {
        return this->object->Horizontal_Blocks(x, Physics::BASIC_BLOCK_Y, this->Get_Random_Length());
    } else if (PROBABILITY_HORIZONTAL_COINS) {
        return this->object->Horizontal_Coins(x, this->Get_Random_Y(), this->Get_Random_Length());
    } else if (PROBABILITY_PIPE) {
        int y = this->Get_Random_Pipe_Y();
        int height = Physics::GROUND_Y - y;
        return this->object->Pipe(x, y, height);
    } else if (PROBABILITY_HOLE) {
        return this->object->Hole(x, this->Get_Random_Hole_Length(), false);
    } else if (PROBABILITY_STEPS) {
        return this->object->Steps(x, this->Get_Random_Steps_Size());
    } else if (PROBABILITY_QUESTION_BLOCK_WITH_COIN) {
        return this->object->Question_Block_With_Coin(x, Physics::BASIC_BLOCK_Y);
    } else if (PROBABILITY_VERTICAL_BLOCKS) {
        int y = this->Get_Random_Y();
        int height = Physics::GROUND_Y - y;
        return this->object->Vertical_Blocks(x, y, height);
    } else if (PROBABILITY_QUESTION_BLOCK_WITH_MUSHROOM) {
        return this->object->Question_Block_With_Mushroom(x, Physics::BASIC_BLOCK_Y);
    } else if (PROBABILITY_BRICK_WITH_10_COINS) {
        return this->object->Brick_With_10_Coins(x, Physics::BASIC_BLOCK_Y);
    } else if (PROBABILITY_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS) {
        return this->object->Horizontal_Question_Blocks_With_Coins(x, Physics::BASIC_BLOCK_Y, this->Get_Random_Length());
    } else if (PROBABILITY_BRICK_WITH_MUSHROOM) {
        return this->object->Brick_With_Mushroom(x, Physics::BASIC_BLOCK_Y);
    } else if (PROBABILITY_BRICK_WITH_STAR) {
        return this->object->Brick_With_Star(x, Physics::BASIC_BLOCK_Y);
    } else if (PROBABILITY_BRICK_WITH_1UP) {
        return this->object->Brick_With_1up(x, Physics::BASIC_BLOCK_Y);
    } else if (PROBABILITY_HIDDEN_BLOCK_WITH_1UP) {
        return this->object->Hidden_Block_With_1up(x, Physics::BASIC_BLOCK_Y);
    } else {
        assert(false);
    }
    return false;
}
