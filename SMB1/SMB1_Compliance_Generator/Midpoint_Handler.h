#ifndef MIDPOINT_HANDLER_H
#define MIDPOINT_HANDLER_H

#include "../Common SMB1 Files/Level_Type.h"

class Object_Writer;

class Midpoint_Handler
{
public:
    Midpoint_Handler(Object_Writer *object);
    void Set_Level_Type(Level_Type::Level_Type type);
    void Handle_Midpoint(int &x);
    bool Is_Midpoint_Written();
    int Get_Midpoint();

private:
    bool Increment_Past_Standard_Overworld_Midpoint(int &x, int &page);
    bool Increment_Past_Island_Midpoint(int &x, int &page);
    Object_Writer *object;
    Level_Type::Level_Type levelType;
    bool midpointWritten;
    int midpoint;
};

#endif // MIDPOINT_HANDLER_H
