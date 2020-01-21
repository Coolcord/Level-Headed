#ifndef MIDPOINT_HANDLER_H
#define MIDPOINT_HANDLER_H

#include "../Common_SMB1_Files/Level_Type.h"

class Object_Buffer;
class Continuous_Enemies_Spawner;
class SMB1_Compliance_Generator_Arguments;

class Midpoint_Handler {
public:
    Midpoint_Handler(Object_Buffer *object, Continuous_Enemies_Spawner *continuousEnemiesSpawner, SMB1_Compliance_Generator_Arguments *args, Level_Type::Level_Type levelType);
    void Handle_Midpoint(int &x);
    bool Is_Midpoint_Written();
    int Get_Midpoint();

private:
    bool Increment_Past_Standard_Overworld_Midpoint(int &x, int &page);
    bool Increment_Past_Island_Midpoint(int &x, int &page);
    Object_Buffer *object;
    Continuous_Enemies_Spawner *continuousEnemiesSpawner;
    SMB1_Compliance_Generator_Arguments *args;
    Level_Type::Level_Type levelType;
    bool midpointWritten;
    int midpoint;
};

#endif // MIDPOINT_HANDLER_H
