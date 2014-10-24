#include "End_Spawner.h"
#include "Physics.h"
#include <assert.h>

End_Spawner::End_Spawner(Object_Writer *object) : Object_Spawner(object) {
    assert(object);
    this->object = object;
    this->endWritten = false;
}

bool End_Spawner::Is_End_Written() {
    return this->endWritten;
}

bool End_Spawner::Handle_End(int x) {
    if (this->endWritten) return false; //can't write another end
    int numObjectsLeft = this->object->Get_Num_Objects_Left();
    if (numObjectsLeft == 3) return this->Shortest_End(x);
    return true;
}

bool End_Spawner::Shortest_End(int x) {
    if (this->object->Get_Num_Objects_Left() < Physics::MIN_END_OBJECTS) return false;

    //Spawn the end of the level
    int absoluteX = this->object->Get_Absolute_X(x);

    //Handle the problem case of spawning at the edge of a page
    if (absoluteX == 0xF) {
        if (x < 0x10) ++x;
        else if (x > this->object->Get_Last_Object_Length()+1) --x;
        else ++x;
    }

    //Write the End Pattern
    this->object->Set_Coordinate_Safety(false); //turn off the safety check, since absolue value is confirmed first
    if (!this->object->End_Steps(x)) return false;
    if (!this->object->Flagpole(Physics::END_STAIRS_LENGTH+Physics::FLAGPOLE_DISTANCE)) return false;
    if (!this->object->Castle(Physics::CASTLE_DISTANCE)) return false;
    this->object->Set_Coordinate_Safety(true); //turn back on the safety

    this->endWritten = true;
    return true;
}
