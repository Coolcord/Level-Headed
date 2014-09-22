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
        switch (qrand() % 5) {
        case 0:     return this->Two_Steps_And_Hole();
        case 1:     return this->Pipe_Series();
        case 2:     return this->Platform_Over_Hole();
        case 3:     return this->Vertical_And_Horizontal_Blocks();
        case 4:     return this->Vertical_Blocks();
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
    bool landing = false;
    height = this->Get_Random_Number(2, height);
    if (this->availableObjects > 0) {
        if (qrand() % 2 == 0) {
            assert(this->object->Vertical_Blocks(length, this->Get_Y_From_Height(height), height));
            --this->availableObjects;
            landing = true;
        }
    }

    //Possibly place steps going down
    height = this->Get_Random_Number(Physics::MIN_STEPS_SIZE, height);
    if (this->availableObjects >= height) {
        if (qrand() % 2 == 0) {
            landing = true;
            while (height > 0 && this->availableObjects > 0) {
                assert(this->object->Vertical_Blocks(this->object->Get_Last_Object_Length(), this->Get_Y_From_Height(height), height));
                --this->availableObjects;
                --height;
            }
        }
    }

    if (hole && !landing) this->object->Increment_Last_Object_Length(1); //add a safe spot after the hole
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

bool Common_Pattern_Spawner::Platform_Over_Hole() {
    assert(this->availableObjects >= 2);

    //Add a hole with random length
    int holeLength = this->Get_Random_Number(3, Physics::MAX_OBJECT_LENGTH-1);
    int x = this->Get_Safe_Random_X();
    assert(this->object->Hole(x, holeLength, false));

    //Add a platform to use to cross the hole at a random height
    int height = this->Get_Random_Number(2, Physics::BASIC_JUMP_HEIGHT);
    x = this->Get_Random_Number(0, holeLength/2);
    if (x > Physics::WALKING_JUMP_LENGTH) x = Physics::WALKING_JUMP_LENGTH;

    //Possibly make the platform uniform on both sides
    int length = 0;
    if (qrand() % 2 == 0) { //uniform
        length = holeLength-(2*x);
    } else { //not uniform
        length = this->Get_Random_Number(2, holeLength-x);
        while (holeLength-(x+length) > Physics::RUNNING_JUMP_LENGTH) {
            ++length; //prevent making impossible jumps
        }
    }
    if (length < 2 && holeLength > 3) length = 2; //have at least 2 blocks on the platform
    assert(length+x <= holeLength);

    //Use either bricks or blocks to make the platform
    if (qrand() % 2 == 0) {
        assert(this->object->Horizontal_Blocks(x, this->Get_Y_From_Height(height), length));
    } else {
        assert(this->object->Horizontal_Bricks(x, this->Get_Y_From_Height(height), length));
    }

    //Fix the last object length
    this->object->Increment_Last_Object_Length(holeLength-x);
    return true;
}

bool Common_Pattern_Spawner::Vertical_And_Horizontal_Blocks() {
    assert(this->availableObjects >= 2);

    //Spawn the initial block
    bool vertical = false;
    int x = this->Get_Safe_Random_X();
    int y = this->Get_Safe_Random_Y(x);
    if (qrand() % 2 == 0) {
        assert(this->object->Vertical_Blocks(x, y, this->Get_Height_From_Y(y)));
        vertical = true;
    } else {
        assert(this->object->Horizontal_Blocks(x, y, this->Get_Random_Number(1, 4)));
        vertical = false;
    }
    --this->availableObjects;

    //Continue the pattern up to 7 more times
    for (int i = this->Get_Random_Number(1, 7); i > 0 && this->availableObjects > 0; --i) {
        x = this->object->Get_Last_Object_Length();
        if (vertical) { //alternate between horizontal and vertical blocks
            y = this->object->Get_Current_Y();
            //Possibly go down
            if (qrand() % 2 == 0) y += this->Get_Random_Number(1, 4);
            if (y > Physics::GROUND_Y) y = Physics::GROUND_Y; //don't go too low
            assert(this->object->Horizontal_Blocks(x, y, this->Get_Random_Number(2, Physics::MAX_OBJECT_LENGTH-1)));
            vertical = false;
        } else {
            y = this->object->Get_Current_Y();
            //Possibly go up
            if (qrand() % 2 == 0) y -= this->Get_Random_Number(1, 4);
            if (y < 0) y = 0; //don't go too high
            assert(this->object->Vertical_Blocks(x, y, this->Get_Height_From_Y(y)));
            vertical = true;
        }
        --this->availableObjects;
    }

    return true;
}

bool Common_Pattern_Spawner::Vertical_Blocks() {
    assert(this->availableObjects >= 2);

    //Spawn the first Vertical Block
    int x = this->Get_Safe_Random_X();
    int y = this->Get_Safe_Random_Y(x);
    if (y >= Physics::GROUND_Y-1) y = Physics::GROUND_Y-1;
    assert(this->object->Vertical_Blocks(x, y, this->Get_Height_From_Y(y)));
    --this->availableObjects;


    //Spawn a series of Verical Blocks after
    x = 1;
    for (int i = this->Get_Random_Number(1, 6); i > 0 && this->availableObjects > 0; --i) {
        y = this->object->Get_Current_Y();
        //Possibly change Y
        if (qrand() % 4 != 0) {
            if (qrand() % 2 == 0) {
                y -= this->Get_Random_Number(1, Physics::BASIC_JUMP_HEIGHT);
            } else {
                y += this->Get_Random_Number(1, Physics::BASIC_JUMP_HEIGHT);
            }
        }
        if (y <= 0) y = Physics::HIGHEST_Y;
        if (y >= Physics::GROUND_Y-1) y = Physics::GROUND_Y-1;
        assert(this->object->Vertical_Blocks(x, y, this->Get_Height_From_Y(y)));
        --this->availableObjects;
    }

    return true;
}



