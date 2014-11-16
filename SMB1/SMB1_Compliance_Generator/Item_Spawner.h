#ifndef ITEM_SPAWNER_H
#define ITEM_SPAWNER_H

#include "../Common SMB1 Files/Level_Type.h"

class Object_Writer;

class Item_Spawner
{
public:
    Item_Spawner(Object_Writer *object, Level_Type::Level_Type levelType);
    int Spawn_Random_Item(int currentX, int maxX, int groundLevelY, int minY, int requiredObjects);

private:
    Object_Writer *object;
    Level_Type::Level_Type levelType;
};

#endif // ITEM_SPAWNER_H