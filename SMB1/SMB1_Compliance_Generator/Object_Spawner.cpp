#include "Object_Spawner.h"
#include "Physics.h"
#include <assert.h>
#include <QTime>

Object_Spawner::Object_Spawner(Object_Writer *object) {
    assert(object);
    this->object = object;
}


int Object_Spawner::Get_Random_X() {
    //Aim for a lower value... but allow higher values to be possible
    switch (qrand() % 4) {
    case 0:     return (qrand() % 0x08);
    case 1:     return (qrand() % 0x0A);
    case 2:     return (qrand() % 0x0D);
    case 3:     return (qrand() % 0x10);
    default:    assert(false); return 0;
    }
}

int Object_Spawner::Get_Random_X(int min) {
    int x = this->Get_Random_X();
    x += min;
    if (x > 0x10) x = 0x10;
    return x;
}

int Object_Spawner::Get_Random_Number(int min, int max) {
    if (min == max) return min;
    assert(max > min);
    return ((qrand() % (max+1-min)) + min);
}

int Object_Spawner::Get_Y_From_Height(int height) {
    assert(height > 0);
    return (Physics::GROUND_Y-height)+1;
}

int Object_Spawner::Get_Height_From_Y(int y) {
    assert(y >= 0 && y <= Physics::GROUND_Y);
    return (Physics::GROUND_Y-y)+1;
}

int Object_Spawner::Get_Random_Y(int x) {
    int y = this->object->Get_Current_Y();
    bool up = static_cast<bool>((qrand() % 2));
    if (x > Physics::BASIC_JUMP_HEIGHT) {
        return Physics::GROUND_Y; //drop back to ground level if the x goes out too far
    }
    if (up) {
        y -= qrand() % (Physics::BASIC_JUMP_HEIGHT+1);
        if (y < Physics::HIGHEST_Y) y = Physics::HIGHEST_Y; //make sure y is valid
    } else {
        y += qrand() % (Physics::BASIC_JUMP_HEIGHT+1);
        if (y > Physics::GROUND_Y) y = Physics::GROUND_Y; //make sure y is valid
    }
    return y;
}

int Object_Spawner::Get_Random_Pipe_Y(int x) {
    int y = this->Get_Random_Y(x) - 1;
    if (y < Physics::MAX_PIPE_Y) y = Physics::MAX_PIPE_Y; //make sure y is not too high
    else if (y > Physics::MIN_PIPE_Y) y = Physics::MIN_PIPE_Y; //make sure y is not too low
    return y;
}

bool Object_Spawner::Is_Safe_To_Spawn_Pattern(int objectCost) {
    return ((this->object->Get_Num_Objects_Left() - objectCost) > Physics::MIN_END_OBJECTS);
}

int Object_Spawner::Get_Safe_Random_X() {
    return this->Get_Random_X(this->object->Get_Last_Object_Length());
}
