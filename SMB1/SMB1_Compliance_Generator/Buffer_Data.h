#ifndef BUFFER_DATA_H
#define BUFFER_DATA_H

#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Enemy_Item.h"
#include "../Common_SMB1_Files/Level.h"
#include "../Common_SMB1_Files/Object_Item.h"
#include "../Common_SMB1_Files/Scenery.h"

class Buffer_Data {
public:
    Buffer_Data() {
        this->x = 0; this->y = 0; this->absoluteX = 0; this->page = 0; this->lineNum = 0;

        //Object Specific
        this->objectItem = Object_Item::NOTHING;
        this->length = 1; this->platform = false; this->height = 1;
        this->brick = Brick::NO_BRICKS; this->scenery = Scenery::NO_SCENERY; this->background = Background::BLANK_BACKGROUND;

        //Enemy Specific
        this->enemyItem = Enemy_Item::NOTHING;
        this->onlyHardMode = false; this->moving = false; this->leaping = false; this->clockwise = false;
        this->fast = false; this->small = false; this->vertical = false; this->up = false;
        this->level = Level::WORLD_1_LEVEL_1;
        this->world = 0; this->num = 0;
    }
    ~Buffer_Data() {}
    int x;
    int y;
    int absoluteX;
    int page;
    int lineNum; //only used in the parser

    //Objects
    Object_Item::Object_Item objectItem;
    int length;
    int height;
    bool platform;
    Background::Background background;
    Brick::Brick brick;
    Scenery::Scenery scenery;

    //Enemies
    Enemy_Item::Enemy_Item enemyItem;
    bool onlyHardMode;
    bool moving;
    bool leaping;
    bool clockwise;
    bool fast;
    bool small;
    bool vertical;
    bool up;
    Level::Level level;
    int world;
    int num;
};

#endif // BUFFER_DATA_H
