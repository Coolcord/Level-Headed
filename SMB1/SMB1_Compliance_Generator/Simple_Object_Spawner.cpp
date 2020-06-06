#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "Simple_Object_Spawner.h"
#include "Object_Buffer.h"
#include "Physics.h"
#include <assert.h>
#include <QTime>

Simple_Object_Spawner::Simple_Object_Spawner(Object_Buffer *object, Level_Type::Level_Type levelType) : Object_Spawner(object) {
    this->levelType = levelType;
    switch (this->levelType) {
    case Level_Type::STANDARD_OVERWORLD:
    case Level_Type::UNDERWATER:
    case Level_Type::ISLAND:
    case Level_Type::BRIDGE:
    case Level_Type::CASTLE:
        this->minY = Physics::HIGHEST_Y; break;
    case Level_Type::UNDERGROUND:
        this->minY = 4; break;
    default:
        assert(false);
    }
}

int Simple_Object_Spawner::Get_Random_Length() {
    return Random::Get_Instance().Get_Num(Physics::MAX_OBJECT_LENGTH-3)+1;
}

int Simple_Object_Spawner::Get_Random_Hole_Length() {
    return Random::Get_Instance().Get_Num(Physics::RUNNING_JUMP_LENGTH-2)+1;
}

int Simple_Object_Spawner::Get_Random_Steps_Size() {
    return Random::Get_Instance().Get_Num(Physics::MIN_STEPS_SIZE, Physics::MAX_STEPS_SIZE-2);
}

void Simple_Object_Spawner::Fill_With_Question_Blocks(int y, int length) {
    int lastObjectX = 0;
    int numObjects = 0;
    for (int i = 0; i < length; ++i) {
        if (this->object->Get_Num_Objects_Available() > 0 && numObjects < length-1) {
            int random = Random::Get_Instance().Get_Num(QUESTION_TOTAL-1);
            if (random <= QUESTION_COIN) continue;
            else if (random <= QUESTION_MUSHROOM) {
                if (this->object->Question_Block_With_Coin(i, y)) {
                    lastObjectX = i;
                    ++numObjects;
                }
            } else assert(false);
        }
    }
    if (numObjects > 0) this->object->Increment_Last_Object_Length(length-lastObjectX);
}

void Simple_Object_Spawner::Fill_With_Question_Blocks_And_Bricks(int y, int length) {
    int lastObjectX = 0;
    int numObjects = 0;
    for (int i = 0; i < length; ++i) {
        if (this->object->Get_Num_Objects_Available() > 0 && numObjects < length-1) {
            int random = Random::Get_Instance().Get_Num(BRICK_TOTAL-1);
            if (random <= BRICK_NOTHING) continue;
            else if (random <= BRICK_QUESTION_COIN) {
                if (this->object->Question_Block_With_Coin(i, y)) {
                    lastObjectX = i;
                    ++numObjects;
                }
            } else if (random <= BRICK_QUESTION_MUSHROOM) {
                if (this->object->Question_Block_With_Coin(i, y)) {
                    lastObjectX = i;
                    ++numObjects;
                }
            } else assert(false);
        }
    }
    if (numObjects > 0) this->object->Increment_Last_Object_Length(length-lastObjectX);
}

bool Simple_Object_Spawner::Spawn_Simple_Object(int x) {
    if (this->object->Get_Num_Objects_Available() == 0) return false;
    int random = Random::Get_Instance().Get_Num(PROBABILITY_TOTAL_SIMPLE);
    if (random <= PROBABILITY_HORIZONTAL_BRICKS) {
        if (this->object->Horizontal_Bricks(x, Physics::BASIC_BLOCK_Y, this->Get_Random_Length())) {
            int length = this->object->Get_Last_Object_Length();
            if (length > 1) this->Fill_With_Question_Blocks_And_Bricks(Physics::BASIC_BLOCK_Y, length);
            this->object->Increment_Last_Object_Length(1);
            return true;
        } else {
            return false;
        }
    } else if (random <= PROBABILITY_HORIZONTAL_BLOCKS) {
        if (this->object->Horizontal_Blocks(x, Physics::BASIC_BLOCK_Y, this->Get_Random_Length())) {
            this->object->Increment_Last_Object_Length(1);
            return true;
        } else {
            return false;
        }
    } else if (random <= PROBABILITY_HORIZONTAL_COINS) {
        int length = this->Get_Random_Length();
        if (length < 3) length = 3; //spawn at least 3 coins
        int y = this->Get_Random_Number(this->Get_Y_From_Height(Physics::BASIC_JUMP_HEIGHT+1), Physics::GROUND_Y-2);
        return this->object->Horizontal_Coins(x, y, length);
    } else if (random <= PROBABILITY_PIPE) {
        int y = this->Get_Random_Pipe_Y(x);
        if (y < this->minY) y = this->minY;
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
        int y = this->Get_Safe_Random_Y(x);
        if (y < this->minY) y = this->minY;
        int height = Physics::GROUND_Y - y + 1;
        return this->object->Vertical_Blocks(x, y, height);
    } else if (random <= PROBABILITY_QUESTION_BLOCK_WITH_MUSHROOM) {
        return this->object->Question_Block_With_Coin(x, Physics::BASIC_BLOCK_Y);
    } else if (random <= PROBABILITY_BRICK_WITH_10_COINS) {
        return this->object->Horizontal_Bricks(x, Physics::BASIC_BLOCK_Y, 1);
    } else if (random <= PROBABILITY_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS) {
        if (this->object->Horizontal_Question_Blocks_With_Coins(x, Physics::BASIC_BLOCK_Y, this->Get_Random_Length())) {
            this->object->Increment_Last_Object_Length(1);
            return true;
        } else {
            return false;
        }
    } else if (random <= PROBABILITY_BRICK_WITH_MUSHROOM) {
        return this->object->Horizontal_Bricks(x, Physics::BASIC_BLOCK_Y, 1);
    } else if (random <= PROBABILITY_BRICK_WITH_STAR) {
        return this->object->Horizontal_Bricks(x, Physics::BASIC_BLOCK_Y, 1);
    } else if (random <= PROBABILITY_BRICK_WITH_1UP) {
        return this->object->Horizontal_Bricks(x, Physics::BASIC_BLOCK_Y, 1);
    } else {
        assert(false);
    }
    return false;
}
