#ifndef OBJECT_SPAWNER_H
#define OBJECT_SPAWNER_H

#include "Object_Buffer.h"
#include "../Common_SMB1_Files/Level_Type.h"

class Object_Spawner {
public:
    Object_Spawner(Object_Buffer *object);

protected:
    Object_Spawner(const Object_Spawner&);
    Object_Spawner& operator=(const Object_Spawner&);
    int Get_Random_Number(int min, int max);
    int Get_Y_From_Height(int height);
    int Get_Height_From_Y(int y);
    int Get_Safe_Random_Y(int x);
    int Get_Random_Pipe_Y(int x);
    Object_Buffer *object;
};

#endif // OBJECT_SPAWNER_H
