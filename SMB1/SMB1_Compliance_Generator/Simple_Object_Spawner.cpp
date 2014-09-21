#include "Simple_Object_Spawner.h"
#include "Object_Writer.h"
#include "Physics.h"
#include <assert.h>
#include <QTime>

Simple_Object_Spawner::Simple_Object_Spawner(Object_Writer *object) : Object_Spawner::Object_Spawner(object) {
    assert(object);
    this->object = object;
}

int Simple_Object_Spawner::Get_Random_Length() {
    return ((qrand() % (Physics::MAX_OBJECT_LENGTH-2))+1);
}

int Simple_Object_Spawner::Get_Random_Hole_Length() {
    return ((qrand() % Physics::RUNNING_JUMP_LENGTH)+1);
}

int Simple_Object_Spawner::Get_Random_Steps_Size() {
    return (qrand() % (Physics::MAX_STEPS_SIZE-Physics::MIN_STEPS_SIZE+1)+Physics::MIN_STEPS_SIZE);
}

bool Simple_Object_Spawner::Spawn_Simple_Object() {
    if (this->object->Get_Num_Objects_Available() == 0) return false;
    int x = this->Get_Random_X(this->object->Get_Last_Object_Length());
    int random = qrand() % this->PROBABILITY_TOTAL_SIMPLE;
    if (random <= PROBABILITY_HORIZONTAL_BRICKS) {
        return this->object->Horizontal_Bricks(x, Physics::BASIC_BLOCK_Y, this->Get_Random_Length());
    } else if (random <= PROBABILITY_HORIZONTAL_BLOCKS) {
        return this->object->Horizontal_Blocks(x, Physics::BASIC_BLOCK_Y, this->Get_Random_Length());
    } else if (random <= PROBABILITY_HORIZONTAL_COINS) {
        return this->object->Horizontal_Coins(x, this->Get_Random_Y(x), this->Get_Random_Length());
    } else if (random <= PROBABILITY_PIPE) {
        int maxY = this->Get_Y_From_Height(Physics::BASIC_JUMP_HEIGHT);
        int y = this->Get_Random_Pipe_Y(x);
        //Prevent creating areas that are impossible to get out of
        if (x > this->object->Get_Last_Object_Length() && y > maxY) y = maxY;
        int height = Physics::GROUND_Y - y + 1;
        assert(height <= 8);
        return this->object->Pipe(x, y, height);
    } else if (random <= PROBABILITY_HOLE) {
        if (this->object->Hole(x, this->Get_Random_Hole_Length(), false)) {
            this->object->Increment_Last_Object_Length(1);
            return true;
        } else {
            return false;
        }
    } else if (random <= PROBABILITY_STEPS) {
        return this->object->Steps(x, this->Get_Random_Steps_Size());
    } else if (random <= PROBABILITY_QUESTION_BLOCK_WITH_COIN) {
        return this->object->Question_Block_With_Coin(x, Physics::BASIC_BLOCK_Y);
    } else if (random <= PROBABILITY_VERTICAL_BLOCKS) {
        int maxY = this->Get_Y_From_Height(Physics::BASIC_JUMP_HEIGHT);
        int y = this->Get_Random_Y(x);
        //Prevent creating areas that are impossible to get out of
        if (x > this->object->Get_Last_Object_Length() && y > maxY) y = maxY;
        int height = Physics::GROUND_Y - y + 1;
        return this->object->Vertical_Blocks(x, y, height);
    } else if (random <= PROBABILITY_QUESTION_BLOCK_WITH_MUSHROOM) {
        return this->object->Question_Block_With_Mushroom(x, Physics::BASIC_BLOCK_Y);
    } else if (random <= PROBABILITY_BRICK_WITH_10_COINS) {
        return this->object->Brick_With_10_Coins(x, Physics::BASIC_BLOCK_Y);
    } else if (random <= PROBABILITY_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS) {
        return this->object->Horizontal_Question_Blocks_With_Coins(x, Physics::BASIC_BLOCK_Y, this->Get_Random_Length());
    } else if (random <= PROBABILITY_BRICK_WITH_MUSHROOM) {
        return this->object->Brick_With_Mushroom(x, Physics::BASIC_BLOCK_Y);
    } else if (random <= PROBABILITY_BRICK_WITH_STAR) {
        return this->object->Brick_With_Star(x, Physics::BASIC_BLOCK_Y);
    } else if (random <= PROBABILITY_BRICK_WITH_1UP) {
        return this->object->Brick_With_1up(x, Physics::BASIC_BLOCK_Y);
    } else if (random <= PROBABILITY_HIDDEN_BLOCK_WITH_1UP) {
        return this->object->Hidden_Block_With_1up(x, Physics::BASIC_BLOCK_Y);
    } else {
        assert(false);
    }
    return false;
}
