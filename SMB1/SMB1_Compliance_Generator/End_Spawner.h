#ifndef END_SPAWNER_H
#define END_SPAWNER_H

#include "Object_Spawner.h"

class End_Spawner : public Object_Spawner
{
public:
    End_Spawner(Object_Writer *object);
    bool Is_End_Written();
    bool Handle_End(int x);

private:
    End_Spawner(const End_Spawner&);
    End_Spawner& operator=(const End_Spawner&);
    bool Shortest_End(int x);

    bool endWritten;

};

#endif // END_SPAWNER_H
