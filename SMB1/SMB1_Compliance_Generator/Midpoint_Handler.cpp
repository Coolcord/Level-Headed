#include "Midpoint_Handler.h"
#include "Object_Writer.h"
#include <assert.h>

Midpoint_Handler::Midpoint_Handler(Object_Writer *object) {
    assert(object);
    this->object = object;
    this->midpointWritten = false;
    this->midpoint = 0;
}

void Midpoint_Handler::Handle_Midpoint(int &x) {
    if (this->midpointWritten) return; //the midpoint was already written
    if (!this->object->Is_Midpoint_Ready()) return; //midpoint is not ready to be written yet
    if (!this->Increment_Past_Midpoint(x)) return; //unable to increment at this time

    this->midpoint = this->object->Get_Current_Page();
    if (this->midpoint > 0xF) this->midpoint = 0xF; //the midpoint must be able to fit into a nibble
    this->midpointWritten = true;
}

bool Midpoint_Handler::Is_Midpoint_Written() {
    return this->midpointWritten;
}

int Midpoint_Handler::Get_Midpoint() {
    return this->midpoint;
}

bool Midpoint_Handler::Increment_Past_Midpoint(int &x) {
    //Absolute coordinates 0x3 and 0x4 must be clear
    //Increment to 0x5 to fix
    int absoluteX = this->object->Get_Absolute_X(x);
    switch (absoluteX) {
    case 0x0:
    case 0x1:
    case 0x2:
        if (x+(5-absoluteX) > 0x10) return false;
        else x += (5-absoluteX);
        return true;
    case 0x3:
    case 0x4:
        return false; //unable to place a midpoint at this time
    case 0x5:
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
        if (x+(0x15-absoluteX) > 0x10) return false;
        else x += (0x15-absoluteX);
        return true;
    default:
        assert(false);
        return false;
    }
}
