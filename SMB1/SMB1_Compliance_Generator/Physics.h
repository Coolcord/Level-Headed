#ifndef PHYSICS_H
#define PHYSICS_H

namespace Physics {
    const static int HIGHEST_Y = 0x0;
    const static int GROUND_Y = 0xA;
    const static int BASIC_BLOCK_Y = 0x7;
    const static int BASIC_JUMP_HEIGHT = 4;
    const static int WALKING_JUMP_LENGTH = 5;
    const static int RUNNING_JUMP_LENGTH = 8;
    const static int MAX_JUMP_LENGTH = 9;
    const static int MAX_OBJECT_LENGTH = 16;
    const static int MIN_OBJECT_LENGTH = 1;
    const static int MIN_STEPS_SIZE = 2;
    const static int MAX_STEPS_SIZE = 8;
    const static int PIPE_LENGTH = 2;
    const static int TALL_PIPE_LENGTH = 4;
}

#endif // PHYSICS_H
