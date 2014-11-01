#ifndef END_SPAWNER_H
#define END_SPAWNER_H

#include "../Common SMB1 Files/Castle.h"
#include "../Common SMB1 Files/Level_Type.h"
#include "End_Pattern.h"
#include "SMB1_Compliance_Generator_Arguments.h"
#include "Object_Spawner.h"

class End_Spawner : public Object_Spawner
{
public:
    End_Spawner(Object_Writer *object, SMB1_Compliance_Generator_Arguments *args);
    bool Is_End_Written();
    bool Handle_End(int x);

private:
    End_Spawner(const End_Spawner&);
    End_Spawner& operator=(const End_Spawner&);
    void Determine_End();
    bool Determine_Standard_Overworld_End();
    bool Determine_Underground_End();
    bool Determine_Underwater_End();
    bool Determine_Castle_End();
    bool Determine_Bridge_End();
    bool Determine_Island_End();
    bool Shortest_End(int x);
    bool Shortest_With_Brick_End(int x);
    bool One_Block_Bridge_Island_End(int x);
    bool Spawn_Castle();

    SMB1_Compliance_Generator_Arguments *args;
    End_Pattern::End_Pattern endPattern;
    int castleObjectCount;
    int endObjectCount;
    bool endWritten;
};

#endif // END_SPAWNER_H
