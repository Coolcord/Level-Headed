#ifndef MIDPOINT_HANDLER_H
#define MIDPOINT_HANDLER_H

#include "../Common_SMB1_Files/Level_Type.h"
#include "SMB1_Compliance_Generator_Arguments.h"

class Object_Writer;
class Continuous_Enemies_Spawner;

class Midpoint_Handler
{
public:
    Midpoint_Handler(Object_Writer *object, Continuous_Enemies_Spawner *continuousEnemiesSpawner, SMB1_Compliance_Generator_Arguments *args);
    void Handle_Midpoint(int &x);
    bool Is_Midpoint_Written();
    int Get_Midpoint();

private:
    bool Increment_Past_Standard_Overworld_Midpoint(int &x, int &page);
    bool Increment_Past_Island_Midpoint(int &x, int &page);
    Object_Writer *object;
    bool midpointWritten;
    int midpoint;
    SMB1_Compliance_Generator_Arguments *args;
    Continuous_Enemies_Spawner *continuousEnemiesSpawner;
};

#endif // MIDPOINT_HANDLER_H
