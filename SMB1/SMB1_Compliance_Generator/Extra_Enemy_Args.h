#ifndef EXTRA_ENEMY_ARGS_H
#define EXTRA_ENEMY_ARGS_H

#include "../Common_SMB1_Files/Level.h"

struct Extra_Enemy_Args {
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
    int page;
    int num;
    bool allowSpawnAfterCancelSpawner;
};

#endif // EXTRA_ENEMY_ARGS_H
