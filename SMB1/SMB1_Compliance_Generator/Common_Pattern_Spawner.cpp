#include "Common_Pattern_Spawner.h"
#include "Physics.h"
#include <QTime>
#include <assert.h>
#include <QDebug>

Common_Pattern_Spawner::Common_Pattern_Spawner(Object_Writer *object) : Object_Spawner(object) {
    this->availableObjects = 0;
}

bool Common_Pattern_Spawner::Spawn_Common_Pattern() {
    this->availableObjects = this->object->Get_Num_Objects_Available();

    //Min Requirement of 3
    if (availableObjects >= 3) {
        switch (qrand() % 2) {
        case 0:     return this->Two_Steps_And_Hole();
        case 1:     return this->Pipe_Series();
        default:    return false;
        }
    }

    return false;
}

bool Common_Pattern_Spawner::Two_Steps_And_Hole() {
    assert(this->availableObjects >= 3);

    //Spawn the steps going up
    int x = this->Get_Safe_Random_X();
    assert(x > 0);
    int height = this->Get_Random_Number(2, Physics::MAX_STEPS_SIZE-2);
    assert(this->object->Steps(x, height));
    --this->availableObjects;

    //Possibly extend the top platform by one
    bool extendedTop = false;
    if (qrand() % 2 == 0) {
        extendedTop = true;
        assert(this->object->Vertical_Blocks(this->object->Get_Last_Object_Length(), this->Get_Y_From_Height(height), height));
        --this->availableObjects;
    }

    //Possibly place a hole
    bool hole = false;
    if (height > Physics::BASIC_JUMP_HEIGHT || qrand() % 2 == 0) {
        int length = 0;
        if (extendedTop) length = this->Get_Random_Number(Physics::WALKING_JUMP_LENGTH, Physics::RUNNING_JUMP_LENGTH);
        else length = this->Get_Random_Number(2, Physics::WALKING_JUMP_LENGTH);
        assert(this->object->Hole(this->object->Get_Last_Object_Length(), length, false));
        hole = true;
        --this->availableObjects;
    }

    //Add a space between the steps if there is no hole
    int length = this->object->Get_Last_Object_Length();
    if (!hole && height <= Physics::BASIC_JUMP_HEIGHT) {
        length = this->Get_Random_Number(this->object->Get_Last_Object_Length(), Physics::WALKING_JUMP_LENGTH);
    }

    //Possibly place vertical blocks to land on
    height = this->Get_Random_Number(2, height);
    if (this->availableObjects > 0) {
        if (qrand() % 2 == 0) {
            assert(this->object->Vertical_Blocks(length, this->Get_Y_From_Height(height), height));
            --this->availableObjects;
        }
    }

    //Possibly place steps going down
    height = this->Get_Random_Number(Physics::MIN_STEPS_SIZE, height);
    if (this->availableObjects >= height) {
        if (qrand() % 2 == 0) {
            while (height > 0 && this->availableObjects > 0) {
                assert(this->object->Vertical_Blocks(this->object->Get_Last_Object_Length(), this->Get_Y_From_Height(height), height));
                --this->availableObjects;
                --height;
            }
        }
    }

    return true;
}

bool Common_Pattern_Spawner::Pipe_Series() {
    assert(this->availableObjects >= 3);

    //Possibly add space between each pipe
    bool noSpace = false;
    if (qrand() % 2 == 0) noSpace = true;

    //Spawn in all the pipes
    int x = this->Get_Safe_Random_X();
    int y = this->Get_Random_Pipe_Y(x);
    assert(this->object->Pipe(x, y, this->Get_Height_From_Y(y)));
    --this->availableObjects;
    for (int i = this->Get_Random_Number(2, 5); this->availableObjects > 0 && i > 0; --i) {
        if (noSpace) x = Physics::PIPE_LENGTH;
        else x = this->Get_Random_Number(this->object->Get_Last_Object_Length(), 6);
        assert(x >= 2);
        int y = this->Get_Random_Pipe_Y(x);
        assert(this->object->Pipe(x, y, this->Get_Height_From_Y(y)));
        --this->availableObjects;
    }

    return true;
}

