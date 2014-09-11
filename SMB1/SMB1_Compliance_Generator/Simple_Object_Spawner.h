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
    Object_Writer *object;

    /*
    const static int PROBABILITY_HORIZONTAL_BRICKS =                          100;
    const static int PROBABILITY_HORIZONTAL_BLOCKS =                          100;
    const static int PROBABILITY_HORIZONTAL_COINS =                           100;
    const static int PROBABILITY_PIPE =                                       100;
    const static int PROBABILITY_HOLE =                                       100;
    const static int PROBABILITY_STEPS =                                      100;
    const static int PROBABILITY_QUESTION_BLOCK_WITH_COIN =                   100;
    const static int PROBABILITY_VERTICAL_BLOCKS =                            100;
    const static int PROBABILITY_QUESTION_BLOCK_WITH_MUSHROOM =                25;
    const static int PROBABILITY_BRICK_WITH_10_COINS =                         25;
    const static int PROBABILITY_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS =       20;
    const static int PROBABILITY_BRICK_WITH_MUSHROOM =                         10;
    const static int PROBABILITY_BRICK_WITH_STAR =                             10;
    const static int PROBABILITY_BRICK_WITH_1UP =                               4;
    const static int PROBABILITY_HIDDEN_BLOCK_WITH_1UP =                        1;
    const static int TOTAL_SIMPLE_ITEMS =                                      15;
    */
};

#endif // SIMPLE_OBJECT_SPAWNER_H
