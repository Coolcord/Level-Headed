#include "Object_Spawner.h"
#include "Physics.h"
#include <assert.h>
#include <QTime>

Object_Spawner::Object_Spawner(Object_Writer *object) {
    assert(object);
    this->object = object;
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

int Object_Spawner::Get_Safe_Random_Y(int x) {
    int y = this->object->Get_Current_Y();
    bool up = static_cast<bool>((qrand() % 2));
    if (x > Physics::BASIC_JUMP_HEIGHT) {
        return this->Get_Random_Number(this->Get_Y_From_Height(Physics::BASIC_JUMP_HEIGHT), Physics::GROUND_Y); //drop back to ground level if the x goes out too far
    }
    if (up) {
        y -= qrand() % (Physics::BASIC_JUMP_HEIGHT+1);
        if (y < Physics::HIGHEST_Y) y = Physics::HIGHEST_Y; //make sure y is valid
    } else {
        y += qrand() % (Physics::BASIC_JUMP_HEIGHT+1);
        if (y > Physics::GROUND_Y) y = Physics::GROUND_Y; //make sure y is valid
    }

    //Prevent creating unescapable areas
    int maxY = this->Get_Y_From_Height(Physics::BASIC_JUMP_HEIGHT);
    if ((!this->object->Was_Last_Object_A_Platform()) || (x > this->object->Get_Last_Object_Length() && y < maxY)) {
        y = maxY;
    }
    return y;
}

int Object_Spawner::Get_Random_Pipe_Y(int x) {
    int y = this->Get_Safe_Random_Y(x);
    if (y == Physics::GROUND_Y) { //try to get a better value
        y = this->Get_Random_Number(this->Get_Y_From_Height(Physics::BASIC_JUMP_HEIGHT), Physics::MIN_PIPE_Y);
    }
    if (y < Physics::MAX_PIPE_Y) y = Physics::MAX_PIPE_Y; //make sure y is not too high
    else if (y > Physics::MIN_PIPE_Y) y = Physics::MIN_PIPE_Y; //make sure y is not too low
    return y;
}
