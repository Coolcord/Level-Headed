#ifndef SIMPLE_OBJECT_SPAWNER_H
#define SIMPLE_OBJECT_SPAWNER_H

#include "Object_Spawner.h"
#include "../Common_SMB1_Files/Level_Type.h"

class Simple_Object_Spawner : public Object_Spawner {
public:
    Simple_Object_Spawner(Object_Buffer *object, Level_Type::Level_Type levelType);
    bool Spawn_Simple_Object(int x);

private:
    Simple_Object_Spawner(const Simple_Object_Spawner&);
    Simple_Object_Spawner& operator=(const Simple_Object_Spawner&);
    int Get_Random_Length();
    int Get_Random_Hole_Length();
    int Get_Random_Steps_Size();
    void Fill_With_Question_Blocks(int y, int length);
    void Fill_With_Question_Blocks_And_Bricks(int y, int length);

    Level_Type::Level_Type levelType;
    int minY;

    //Probabilities for Filling Bricks
    const static int BRICK_NOTHING =                                                                      3000;
    const static int BRICK_QUESTION_COIN = BRICK_NOTHING +                                                 500;
    const static int BRICK_QUESTION_MUSHROOM = BRICK_QUESTION_COIN +                                        90;
    const static int BRICK_MUSHROOM = BRICK_QUESTION_MUSHROOM +                                             90;
    const static int BRICK_10_COINS = BRICK_MUSHROOM +                                                      70;
    const static int BRICK_STAR = BRICK_10_COINS +                                                          25;
    const static int BRICK_1UP = BRICK_STAR +                                                                5;
    const static int BRICK_TOTAL = BRICK_QUESTION_MUSHROOM;

    //Probabilities for Filling Question Blocks
    const static int QUESTION_NOTHING =                                                                   1000;
    const static int QUESTION_MUSHROOM = QUESTION_NOTHING +                                                100;
    const static int QUESTION_TOTAL = QUESTION_MUSHROOM;

    //Object Probabilities
    const static int PROBABILITY_HORIZONTAL_BRICKS =                                                       100;
    const static int PROBABILITY_HORIZONTAL_BLOCKS = PROBABILITY_HORIZONTAL_BRICKS +                       100;
    const static int PROBABILITY_HORIZONTAL_COINS = PROBABILITY_HORIZONTAL_BLOCKS +                        100;
    const static int PROBABILITY_PIPE = PROBABILITY_HORIZONTAL_COINS +                                     100;
    const static int PROBABILITY_HOLE = PROBABILITY_PIPE +                                                 100;
    const static int PROBABILITY_STEPS = PROBABILITY_HOLE +                                                100;
    const static int PROBABILITY_QUESTION_BLOCK_WITH_COIN = PROBABILITY_STEPS +                            100;
    const static int PROBABILITY_VERTICAL_BLOCKS = PROBABILITY_QUESTION_BLOCK_WITH_COIN +                  100;
    const static int PROBABILITY_QUESTION_BLOCK_WITH_MUSHROOM = PROBABILITY_VERTICAL_BLOCKS +               75;
    const static int PROBABILITY_BRICK_WITH_10_COINS = PROBABILITY_QUESTION_BLOCK_WITH_MUSHROOM +           25;
    const static int PROBABILITY_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS = PROBABILITY_BRICK_WITH_10_COINS +  20;
    const static int PROBABILITY_BRICK_WITH_MUSHROOM = PROBABILITY_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS +  10;
    const static int PROBABILITY_BRICK_WITH_STAR = PROBABILITY_BRICK_WITH_MUSHROOM +                        10;
    const static int PROBABILITY_BRICK_WITH_1UP = PROBABILITY_BRICK_WITH_STAR +                              4;
    const static int PROBABILITY_HIDDEN_BLOCK_WITH_1UP = PROBABILITY_BRICK_WITH_1UP +                        1;
    const static int PROBABILITY_TOTAL_SIMPLE = PROBABILITY_HIDDEN_BLOCK_WITH_1UP;
};

#endif // SIMPLE_OBJECT_SPAWNER_H
