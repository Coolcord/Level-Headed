#ifndef END_SPAWNER_H
#define END_SPAWNER_H

#include "../Common SMB1 Files/Castle.h"
#include "Object_Spawner.h"

class End_Spawner : public Object_Spawner
{
public:
    End_Spawner(Object_Writer *object, Castle::Castle castle);
    bool Is_End_Written();
    bool Handle_End(int x);

private:
    End_Spawner(const End_Spawner&);
    End_Spawner& operator=(const End_Spawner&);
    bool Shortest_End(int x);
    bool Spawn_Castle();

    int castleObjectCount;
    int endObjectCount;
    bool endWritten;
};

#endif // END_SPAWNER_H
