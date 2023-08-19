#ifndef END_SPAWNER_H
#define END_SPAWNER_H

#include "../Common_SMB1_Files/Castle.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include "End_Pattern.h"
#include "SMB1_Compliance_Generator_Arguments.h"
#include "Object_Spawner.h"

class Enemy_Buffer;
class Required_Enemy_Spawns;
class Pipe_Pointer_Buffer;

class End_Spawner : public Object_Spawner {
public:
    End_Spawner(Object_Buffer *objects, Enemy_Buffer *enemies, Pipe_Pointer_Buffer *pipePointers, SMB1_Compliance_Generator_Arguments *args, Required_Enemy_Spawns *requiredEnemySpawns, bool useAutoScroll);
    bool Is_End_Written();
    bool Handle_End(int x);
    bool Handle_End(int x, bool forceWrite);

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
    void Handle_Auto_Scroll();
    bool Shortest_End(int x);
    bool Shortest_End(int x, bool cancelSpawner);
    bool Shortest_With_Brick_End(int x);
    bool Shortest_Castle(int x);
    bool One_Block_Bridge_End(int x);
    bool Simple_Underground_End(int x);
    bool Spawn_Castle();

    Enemy_Buffer *enemies;
    Pipe_Pointer_Buffer *pipePointers;
    SMB1_Compliance_Generator_Arguments *args;
    Required_Enemy_Spawns *requiredEnemySpawns;
    End_Pattern::End_Pattern endPattern;
    int castleObjectCount;
    int endObjectCount;
    bool endWritten;
    bool useAutoScroll;
};

#endif // END_SPAWNER_H
