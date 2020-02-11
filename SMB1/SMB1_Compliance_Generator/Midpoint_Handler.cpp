#include "Midpoint_Handler.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "Continuous_Enemies_Spawner.h"
#include "Object_Buffer.h"
#include "Physics.h"
#include "SMB1_Compliance_Generator_Arguments.h"
#include <QDebug>
#include <assert.h>

Midpoint_Handler::Midpoint_Handler(Object_Buffer *object, Continuous_Enemies_Spawner *continuousEnemiesSpawner, SMB1_Compliance_Generator_Arguments *args, Level_Type::Level_Type levelType) {
    assert(object);
    assert(continuousEnemiesSpawner);
    assert(args);
    this->object = object;
    this->continuousEnemiesSpawner = continuousEnemiesSpawner;
    this->args = args;
    this->levelType = levelType;
    this->midpointWritten = false;
    this->midpoint = 0;
}

void Midpoint_Handler::Handle_Midpoint(int &x) {
    if (this->midpointWritten) return; //the midpoint was already written
    if (!this->object->Is_Midpoint_Ready()) return; //midpoint is not ready to be written yet

    //Auto Scrolling levels can't have midpoints
    if (this->object->Was_Auto_Scroll_Used() || !this->args->useMidpoints) {
        this->midpoint = 0;
        this->midpointWritten = true;
        return;
    }

    //Get the page so that it can be fixed if necessary
    int page = this->object->Get_Current_Page();

    //Handle according to the level type
    if (x < this->object->Get_Last_Object_Length()) x = this->object->Get_Last_Object_Length();
    if (this->object->Get_Num_Objects_Available() == 0) return; //midpoint may not be necessary
    switch (this->levelType) {
    case Level_Type::UNDERGROUND:
    case Level_Type::UNDERWATER:
    case Level_Type::STANDARD_OVERWORLD:
        if (!this->Increment_Past_Standard_Overworld_Midpoint(x, page)) return; //unable to increment at this time
        break;
    case Level_Type::CASTLE:
        this->midpoint = 0;
        this->midpointWritten = true;
        return;
    case Level_Type::ISLAND:
    case Level_Type::BRIDGE:
        if (!this->Increment_Past_Island_Midpoint(x, page)) return; //unable to increment at this time
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
    if (this->midpointWritten) return this->midpoint;
    else return 0;
}

bool Midpoint_Handler::Increment_Past_Standard_Overworld_Midpoint(int &x, int &page) {
    //Absolute coordinates 0x2 - 0x4 must be clear
    //Increment to 0x5 to fix
    int absoluteX = this->object->Get_Page_Relative_Absolute_X(x);
    if (absoluteX < 0x3) {
        if (x+(5-absoluteX) > 0x10) return false;
        else x += (5-absoluteX);
        if (this->object->Will_Page_Flag_Be_Tripped(x)) ++page;
    } else {
        if (x+(0x15-absoluteX) > 0x10) return false;
        else x += (0x15-absoluteX);
        assert(this->object->Will_Page_Flag_Be_Tripped(x));
        ++page;
    }
    Enemy_Item::Enemy_Item enemyItem = this->continuousEnemiesSpawner->Create_Midpoint_Continuous_Enemies_Spawner(x);
    if (enemyItem == Enemy_Item::CHEEP_CHEEP_SPAWNER || enemyItem == Enemy_Item::BULLET_BILL_SPAWNER) x = 0;
    return true;
}

bool Midpoint_Handler::Increment_Past_Island_Midpoint(int &x, int &page) {
    int lastX = x;
    int requiredObjects = 1;
    if (this->levelType == Level_Type::BRIDGE) ++requiredObjects;
    if (this->object->Get_Num_Objects_Available() < requiredObjects) return false;

    //Absolute coordinates 0x2 - 0x4 must be clear
    //Increment to 0x5 to fix
    int absoluteX = this->object->Get_Page_Relative_Absolute_X(x);
    if (absoluteX < 0x3) {
        //Place an island to ensure that the midpoint will be safe
        if (this->object->Will_Page_Flag_Be_Tripped(x)) ++page;
        if (!this->object->Island(x, Physics::GROUND_Y+1, Random::Get_Instance().Get_Num(2)+(6-absoluteX))) return false;
        x = this->object->Get_Last_Object_Length();
        this->continuousEnemiesSpawner->Create_Midpoint_Continuous_Enemies_Spawner(0);
        return true;
    }

    //Place some kind of object to push the x over to the midpoint
    if (absoluteX < 0xA && this->object->Get_Num_Objects_Available() > requiredObjects+1) {
        //Try to randomize the distance
        int tmpX = x+Random::Get_Instance().Get_Num(3)+1;
        if (tmpX > 0x10) tmpX = x;
        absoluteX = this->object->Get_Page_Relative_Absolute_X(tmpX);
        //Determine the length
        int length = (0xB-absoluteX)+Random::Get_Instance().Get_Num(2);
        if (length < 3) length = 3;
        if (length > 7) length = 7;
        //Determine a y value
        int y = 0;
        switch (Random::Get_Instance().Get_Num(2)) {
        case 0:     y = 7; break;
        case 1:     y = 8; break;
        case 2:     y = Physics::GROUND_Y; break;
        default:    assert(false); return false;
        }
        if (this->levelType == Level_Type::BRIDGE) {
            //Spawn the bridge by itself
            if (this->object->Will_Page_Flag_Be_Tripped(tmpX)) ++page;
            if (!this->object->Bridge(tmpX, y, length)) return false;
        } else { //spawn a tree
            if (this->object->Will_Page_Flag_Be_Tripped(tmpX)) ++page;
            if (!this->object->Island(tmpX, y, length)) return false;
        }
        x = this->object->Get_Last_Object_Length();
        lastX = x;
        absoluteX = this->object->Get_Page_Relative_Absolute_X(x);
    }

    //Attempt to spawn an island at absoluteX 0x0
    if (absoluteX >= 0xA) {
        ++page;
        int length = 0x15-absoluteX;
        assert(x+(0x10-absoluteX) <= 0x10);
        assert(this->object->Island(x+(0x10-absoluteX), Physics::GROUND_Y+1, length));
        this->continuousEnemiesSpawner->Create_Midpoint_Continuous_Enemies_Spawner(0);
        x = length;
        return true;
    }
    x = lastX;
    return false;
}
