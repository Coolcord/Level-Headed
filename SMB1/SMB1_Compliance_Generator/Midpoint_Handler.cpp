#include "Midpoint_Handler.h"
#include "../../Common_Files/Random.h"
#include "Continuous_Enemies_Spawner.h"
#include "Object_Writer.h"
#include "Physics.h"
#include "Difficulty.h"
#include <QDebug>
#include <assert.h>

Midpoint_Handler::Midpoint_Handler(Object_Writer *object, Continuous_Enemies_Spawner *continuousEnemiesSpawner, SMB1_Compliance_Generator_Arguments *args) {
    assert(object);
    assert(continuousEnemiesSpawner);
    assert(args);
    assert(args->difficulty >= Difficulty::DIFFICULTY_MIN && args->difficulty <= Difficulty::DIFFICULTY_MAX);
    this->args = args;
    this->object = object;
    this->continuousEnemiesSpawner = continuousEnemiesSpawner;
    this->midpointWritten = false;
    this->midpoint = 0;
}

void Midpoint_Handler::Handle_Midpoint(int &x) {
    if (this->midpointWritten) return; //the midpoint was already written
    if (!this->object->Is_Midpoint_Ready()) return; //midpoint is not ready to be written yet

    //Get the page so that it can be fixed if necessary
    int page = this->object->Get_Current_Page();

    //Handle according to the level type
    int tmpX = x;
    if (x < this->object->Get_Last_Object_Length()) x = this->object->Get_Last_Object_Length();
    if (this->object->Get_Num_Objects_Available() == 0) return; //midpoint may not be necessary
    switch (this->args->levelType) {
    case Level_Type::UNDERGROUND:
    case Level_Type::UNDERWATER:
    case Level_Type::STANDARD_OVERWORLD:
        if (!this->Increment_Past_Standard_Overworld_Midpoint(x, page)) {
            x = tmpX;
            return; //unable to increment at this time
        }
        break;
    case Level_Type::CASTLE:
        this->midpoint = 0;
        this->midpointWritten = true;
        return;
    case Level_Type::ISLAND:
    case Level_Type::BRIDGE:
        if (!this->Increment_Past_Island_Midpoint(x, page)) {
            x = tmpX;
            return;
        }
        break;
    default: assert(false); return;
    }

    //Set the midpoint
    this->midpoint = page;
    if (this->midpoint > 0xF) this->midpoint = 0x0; //the midpoint must be able to fit into a nibble
    this->midpointWritten = true;
}

bool Midpoint_Handler::Is_Midpoint_Written() {
    return this->midpointWritten;
}

int Midpoint_Handler::Get_Midpoint() {
    //Midpoints have been scrapped due to limitations in the engine.
    //Worlds with more than 4 levels end up breaking due to how the game counts levels.
    //For example, level 1-1 and level 1-5 have the same midpoint.
    //With this in mind, midpoints will always be set to 0 from now on.
    return 0;
    //return this->midpoint;
}

bool Midpoint_Handler::Increment_Past_Standard_Overworld_Midpoint(int &x, int &page) {
    //Absolute coordinates 0x3 and 0x4 must be clear
    //Increment to 0x5 to fix
    int absoluteX = this->object->Get_Absolute_X(x);
    if (absoluteX < 0x4) {
        if (x+(5-absoluteX) > 0x10) return false;
        else x += (5-absoluteX);
        if (this->object->Will_Page_Flag_Be_Tripped(x)) ++page;
    } else {
        if (x+(0x15-absoluteX) > 0x10) return false;
        else x += (0x15-absoluteX);
        assert(this->object->Will_Page_Flag_Be_Tripped(x));
        ++page;
    }

    this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(0);
    x = 1;
    return true;
}

bool Midpoint_Handler::Increment_Past_Island_Midpoint(int &x, int &page) {
    int requiredObjects = 1;
    if (this->args->levelType == Level_Type::BRIDGE) ++requiredObjects;
    if (this->object->Get_Num_Objects_Available() < requiredObjects) return false;

    //Absolute coordinates 0x3 and 0x4 must be clear
    //Increment to 0x5 to fix
    int absoluteX = this->object->Get_Absolute_X(x);
    if (absoluteX < 0x4) {
        //Place an island to ensure that the midpoint will be safe
        if (this->object->Will_Page_Flag_Be_Tripped(x)) ++page;
        if (!this->object->Island(x, Physics::GROUND_Y+1, Random::Get_Num(2)+(6-absoluteX))) return false;
        x = this->object->Get_Last_Object_Length()+1;
        this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(0);
        return true;
    }

    //Place some kind of object to push the x over to the midpoint
    if (absoluteX < 0xA && this->object->Get_Num_Objects_Available() > requiredObjects+1) {
        //Try to randomize the distance
        int tmpX = x+Random::Get_Num(3)+1;
        if (tmpX > 0x10) tmpX = x;
        absoluteX = this->object->Get_Absolute_X(tmpX);
        //Determine the length
        int length = (0xB-absoluteX)+Random::Get_Num(2);
        if (length < 3) length = 3;
        if (length > 7) length = 7;
        //Determine a y value
        int y = 0;
        switch (Random::Get_Num(2)) {
        case 0:     y = 7; break;
        case 1:     y = 8; break;
        case 2:     y = Physics::GROUND_Y; break;
        default:    assert(false); return false;
        }
        if (this->args->levelType == Level_Type::BRIDGE) {
            //Spawn the bridge by itself
            if (!this->object->Bridge(tmpX, y, length)) return false;
        } else { //spawn a tree
            if (!this->object->Island(tmpX, y, length)) return false;
        }
        x = this->object->Get_Last_Object_Length();
        absoluteX = this->object->Get_Absolute_X(x);
    }

    //Attempt to spawn an island at absoluteX 0x0
    if (absoluteX >= 0xA) {
        ++page;
        int length = 0x15-absoluteX;
        if (x+(0x10-absoluteX) > 0x10) return false;
        if (!this->object->Island(x+(0x10-absoluteX), Physics::GROUND_Y+1, length)) return false;
        this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(0);
        x = length;
        return true;
    }
    return true;
}
