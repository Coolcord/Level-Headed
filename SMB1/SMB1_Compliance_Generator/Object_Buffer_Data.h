#ifndef OBJECT_BUFFER_DATA_H
#define OBJECT_BUFFER_DATA_H

#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Object_Item.h"
#include "../Common_SMB1_Files/Scenery.h"

struct Object_Buffer_Data {
    Object_Item::Object_Item objectItem;
    int x;
    int y;
    int length;
    int height;
    int page;
    bool platform;
    Background::Background background;
    Brick::Brick brick;
    Scenery::Scenery scenery;
};

#endif // OBJECT_BUFFER_DATA_H
