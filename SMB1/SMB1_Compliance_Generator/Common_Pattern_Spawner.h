#ifndef COMMON_PATTERN_SPAWNER_H
#define COMMON_PATTERN_SPAWNER_H

#include "Object_Spawner.h"

class Common_Pattern_Spawner : public Object_Spawner
{
public:
    Common_Pattern_Spawner(Object_Writer *object);
    bool Spawn_Common_Pattern();

private:
    Common_Pattern_Spawner(const Common_Pattern_Spawner&);
    Common_Pattern_Spawner& operator=(const Common_Pattern_Spawner&);
    bool Two_Steps_And_Hole();
    bool Pipe_Series();
    bool Platform_Over_Hole();
    bool Vertical_And_Horizontal_Blocks();

    int availableObjects;
};

#endif // COMMON_PATTERN_SPAWNER_H
