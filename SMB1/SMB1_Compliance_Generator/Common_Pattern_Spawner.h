#ifndef COMMON_PATTERN_SPAWNER_H
#define COMMON_PATTERN_SPAWNER_H

#include "Object_Spawner.h"
#include "../Common_SMB1_Files/Level_Type.h"

class Common_Pattern_Spawner : public Object_Spawner
{
public:
    Common_Pattern_Spawner(Object_Writer *object, Level_Type::Level_Type levelType);
    bool Spawn_Common_Pattern(int x);

private:
    Common_Pattern_Spawner(const Common_Pattern_Spawner&);
    Common_Pattern_Spawner& operator=(const Common_Pattern_Spawner&);
    bool Two_Steps_And_Hole(int x);
    bool Pipe_Series(int x);
    bool Platform_Over_Hole(int x);
    bool Vertical_And_Horizontal_Blocks(int x);
    bool Vertical_Blocks(int x);

    Level_Type::Level_Type levelType;
    int availableObjects;
    int minY;
};

#endif // COMMON_PATTERN_SPAWNER_H
