#include "End_Spawner.h"
#include "Physics.h"
#include <assert.h>

End_Spawner::End_Spawner(Object_Writer *object, Castle::Castle castle) : Object_Spawner(object) {
    assert(object);
    this->object = object;
    this->endWritten = false;
    switch (castle) {
    case Castle::NONE:  this->castleObjectCount = 0; break;
    case Castle::SMALL: this->castleObjectCount = 1; break;
    case Castle::BIG:   this->castleObjectCount = 2; break;
    default: assert(false);
    }

    //TODO: Determine how many objects to allocate for the end
    this->endObjectCount = Physics::MIN_END_OBJECTS;
    this->object->Set_End_Object_Count(this->endObjectCount);
}

bool End_Spawner::Is_End_Written() {
    return this->endWritten;
}

bool End_Spawner::Handle_End(int x) {
    if (this->endWritten) return false; //can't write another end
    int numObjectsLeft = this->object->Get_Num_Objects_Left();
    if (numObjectsLeft == 2 + this->castleObjectCount) return this->Shortest_End(x);
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
    if (!this->Spawn_Castle()) return false;
    this->object->Set_Coordinate_Safety(true); //turn back on the safety

    this->endWritten = true;
    return true;
}

bool End_Spawner::Spawn_Castle() {
    switch (this->castleObjectCount) {
    case 0:
        return true; //nothing to do
    case 1:
        return this->object->Castle(Physics::CASTLE_DISTANCE);
    case 2:
        if (!this->object->Change_Background(2, Background::CASTLE_WALL)) return false;
        return this->object->Big_Castle(3);
    default:
        assert(false);
        return false;
    }
}
