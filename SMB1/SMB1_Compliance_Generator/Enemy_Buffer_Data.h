#ifndef ENEMY_BUFFER_DATA_H
#define ENEMY_BUFFER_DATA_H

#include "../Common_SMB1_Files/Enemy_Item.h"
#include "../Common_SMB1_Files/Level.h"
#include "Extra_Enemy_Args.h"
#include <QString>

struct Enemy_Buffer_Data {
    Enemy_Item::Enemy_Item enemyItem;
    int x;
    int y;
    Extra_Enemy_Args args;
};

#endif // ENEMY_BUFFER_DATA_H
