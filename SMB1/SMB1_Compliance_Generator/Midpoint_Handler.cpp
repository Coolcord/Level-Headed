#include "Midpoint_Handler.h"
#include "Object_Writer.h"
#include "Physics.h"
#include <QDebug>
#include <assert.h>

Midpoint_Handler::Midpoint_Handler(Object_Writer *object) {
    assert(object);
    this->object = object;
    this->midpointWritten = false;
    this->midpoint = 0;
    this->levelType = Level_Type::STANDARD_OVERWORLD;
}

void Midpoint_Handler::Set_Level_Type(Level_Type::Level_Type type) {
    this->levelType = type;
}

void Midpoint_Handler::Handle_Midpoint(int &x) {
    if (this->midpointWritten) return; //the midpoint was already written
    if (!this->object->Is_Midpoint_Ready()) return; //midpoint is not ready to be written yet

    //Get the page so that it can be fixed if necessary
    int page = this->object->Get_Current_Page();

    //Handle according to the level type
    switch (this->levelType) {
    case Level_Type::UNDERGROUND:
    case Level_Type::UNDERWATER:
    case Level_Type::STANDARD_OVERWORLD:
        if (!this->Increment_Past_Standard_Overworld_Midpoint(x, page)) return; //unable to increment at this time
        break;
    case Level_Type::CASTLE:
        assert(false); //TODO: Implement this...
        break;
    case Level_Type::ISLAND:
    case Level_Type::BRIDGE:
        if (!this->Increment_Past_Island_Midpoint(x, page)) return;
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
    return this->midpoint;
}

bool Midpoint_Handler::Increment_Past_Standard_Overworld_Midpoint(int &x, int &page) {
    //Absolute coordinates 0x3 and 0x4 must be clear
    //Increment to 0x5 to fix
    int absoluteX = this->object->Get_Absolute_X(x);
    if (absoluteX < 0x4) {
        if (x+(5-absoluteX) > 0x10) return false;
        else x += (5-absoluteX);
        if (this->object->Will_Page_Flag_Be_Tripped(x)) ++page;
        return true;
    } else {
        if (x+(0x15-absoluteX) > 0x10) return false;
        else x += (0x15-absoluteX);
        assert(this->object->Will_Page_Flag_Be_Tripped(x));
        ++page;
        return true;
    }
}

bool Midpoint_Handler::Increment_Past_Island_Midpoint(int &x, int &page) {
    //Absolute coordinates 0x3 and 0x4 must be clear
    //Increment to 0x5 to fix
    int absoluteX = this->object->Get_Absolute_X(x);
    if (absoluteX < 0x4) {
        //Place an island to ensure that the midpoint will be safe
        if (this->object->Will_Page_Flag_Be_Tripped(x)) ++page;
        if (!this->object->Island(x, Physics::GROUND_Y+1, (qrand()%3)+(6-absoluteX))) return false;
        x = this->object->Get_Last_Object_Length();
        return true;
    }

    //Place some kind of object to push the x over to the midpoint
    if (absoluteX < 0xA) {
        //Try to randomize the distance
        int tmpX = x+(qrand()%4)+1;
        if (tmpX > 0x10) tmpX = x;
        absoluteX = this->object->Get_Absolute_X(tmpX);
        //Determine the length
        int length = (0xB-absoluteX)+(qrand()%3);
        if (length < 3) length = 3;
        if (length > 7) length = 7;
        //Determine a y value
        int y = 0;
        switch (qrand()%3) {
        case 0:     y = 7; break;
        case 1:     y = 8; break;
        case 2:     y = Physics::GROUND_Y; break;
        default:    assert(false); return false;
        }
        if (this->levelType == Level_Type::BRIDGE) {
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
        if (this->levelType == Level_Type::BRIDGE) {
            if (!this->object->Flying_Cheep_Cheep_Spawner(0)) return false;
        }
        x = length;
        return true;
    }
    return true;
}
