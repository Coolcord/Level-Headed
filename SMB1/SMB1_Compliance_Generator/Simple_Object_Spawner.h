#ifndef SIMPLE_OBJECT_SPAWNER_H
#define SIMPLE_OBJECT_SPAWNER_H

#include "Spawner.h"

class Object_Writer;

class Simple_Object_Spawner : public Spawner
{
public:
    Simple_Object_Spawner(Object_Writer *object);
    bool Spawn_Simple_Object();

private:
    int Get_Random_Y();
    int Get_Random_Pipe_Y();
    int Get_Random_Length();
    int Get_Random_Hole_Length();
    int Get_Random_Steps_Size();

    Object_Writer *object;

    const static int PROBABILITY_HORIZONTAL_BRICKS =                                                       100;
    const static int PROBABILITY_HORIZONTAL_BLOCKS = PROBABILITY_HORIZONTAL_BRICKS +                       100;
    const static int PROBABILITY_HORIZONTAL_COINS = PROBABILITY_HORIZONTAL_BLOCKS +                        100;
    const static int PROBABILITY_PIPE = PROBABILITY_HORIZONTAL_COINS +                                     100;
    const static int PROBABILITY_HOLE = PROBABILITY_PIPE +                                                 100;
    const static int PROBABILITY_STEPS = PROBABILITY_HOLE +                                                100;
    const static int PROBABILITY_QUESTION_BLOCK_WITH_COIN = PROBABILITY_STEPS +                            100;
    const static int PROBABILITY_VERTICAL_BLOCKS = PROBABILITY_QUESTION_BLOCK_WITH_COIN +                  100;
    const static int PROBABILITY_QUESTION_BLOCK_WITH_MUSHROOM = PROBABILITY_VERTICAL_BLOCKS +               25;
    const static int PROBABILITY_BRICK_WITH_10_COINS = PROBABILITY_QUESTION_BLOCK_WITH_MUSHROOM +           25;
    const static int PROBABILITY_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS = PROBABILITY_BRICK_WITH_10_COINS +  20;
    const static int PROBABILITY_BRICK_WITH_MUSHROOM = PROBABILITY_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS +  10;
    const static int PROBABILITY_BRICK_WITH_STAR = PROBABILITY_BRICK_WITH_MUSHROOM +                        10;
    const static int PROBABILITY_BRICK_WITH_1UP = PROBABILITY_BRICK_WITH_STAR +                              4;
    const static int PROBABILITY_HIDDEN_BLOCK_WITH_1UP = PROBABILITY_BRICK_WITH_1UP +                        1;
    const static int PROBABILITY_TOTAL_SIMPLE = PROBABILITY_HIDDEN_BLOCK_WITH_1UP;
};

#endif // SIMPLE_OBJECT_SPAWNER_H
