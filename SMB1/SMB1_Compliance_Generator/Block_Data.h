#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include "../Common_SMB1_Files/Object_Item.h"

struct Block_Data {
    Object_Item::Object_Item objectItem;
    int x; //absolute
    int y;
    int groupX;
    int groupY;
    int groupLength;
    bool hittable;
    bool safeForMushroom;
    bool safeForStar;
    bool requiresNewObjectToSpawn;
};

#endif // BLOCK_DATA_H
