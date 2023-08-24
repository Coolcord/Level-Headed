#include "Underground_Bonus_Generator.h"
#include "Physics.h"

bool Underground_Bonus_Generator::Generate_Level() {
    this->powerupDistributor->Deallocate_Powerups();
    this->levelCrawler->Set_Level_Attribute(Level_Attribute::UNDERGROUND);
    this->levelCrawler->Set_Starting_Brick(Brick::ALL);
    assert(this->objects->Change_Brick_And_Scenery(0, Brick::SURFACE, Scenery::NO_SCENERY));

    //Generate the 1-1 underground room
    assert(this->objects->Change_Brick_And_Scenery(3, Brick::SURFACE_4_AND_CEILING, Scenery::NO_SCENERY));
    assert(this->objects->Horizontal_Coins(1, 5, 7));
    assert(this->objects->Horizontal_Coins(0, 7, 7));
    assert(this->objects->Horizontal_Coins(1, 3, 5));
    assert(this->objects->Change_Brick_And_Scenery(5, Brick::SURFACE, Scenery::NO_SCENERY));
    assert(this->pipePointers->Tall_Reverse_L_Pipe(3, Physics::GROUND_Y, Level::PIPE_EXIT_SMALL_CASTLE, this->args->currentWorld, 0)); //TODO: Exits will need to be updated later!
    assert(this->objects->Scroll_Stop(7, false));

    //Generate the 1-2 underground room
    assert(this->objects->Vertical_Bricks(12, 0, 11));
    assert(this->objects->Change_Brick_And_Scenery(2, Brick::SURFACE_AND_CEILING_4, Scenery::NO_SCENERY));
    assert(this->objects->Horizontal_Bricks(1, 7, 9));
    assert(this->objects->Horizontal_Coins(0, Physics::GROUND_Y, 9));
    assert(this->objects->Horizontal_Coins(1, 6, 8));
    assert(this->objects->Brick_With_10_Coins(8, 7));
    assert(this->objects->Change_Brick_And_Scenery(0, Brick::ALL, Scenery::NO_SCENERY));
    assert(this->pipePointers->Tall_Reverse_L_Pipe(1, Physics::GROUND_Y, Level::PIPE_EXIT_SMALL_CASTLE, this->args->currentWorld, 0)); //TODO: Exits will need to be updated later!
    assert(this->objects->Scroll_Stop(7, false));

    //Generate the 3-1 underground room
    assert(this->objects->Vertical_Bricks(12, 0, 11));
    assert(this->objects->Change_Brick_And_Scenery(0, Brick::SURFACE, Scenery::NO_SCENERY));
    assert(this->objects->Vertical_Bricks(3, 3, 4));
    assert(this->objects->Vertical_Bricks(1, 3, 2));
    assert(this->objects->Horizontal_Coins(0, 6, 1));
    assert(this->objects->Vertical_Bricks(0, 7, 1));
    assert(this->objects->Brick_With_Mushroom(1, 3));
    assert(this->objects->Horizontal_Coins(0, 5, 1));
    assert(this->objects->Vertical_Bricks(0, 6, 1));
    assert(this->objects->Horizontal_Coins(1, 2, 4));
    assert(this->objects->Horizontal_Coins(0, 4, 1));
    assert(this->objects->Vertical_Bricks(0, 5, 1));
    assert(this->objects->Horizontal_Coins(1, 1, 2));
    assert(this->objects->Vertical_Bricks(0, 3, 2));
    assert(this->objects->Vertical_Bricks(1, 3, 2));
    assert(this->objects->Horizontal_Coins(1, 4, 1));
    assert(this->objects->Vertical_Bricks(0, 5, 1));
    assert(this->objects->Vertical_Bricks(1, 3, 1));
    assert(this->objects->Horizontal_Coins(0, 5, 1));
    assert(this->objects->Vertical_Bricks(0, 6, 1));
    assert(this->objects->Vertical_Bricks(1, 3, 2));
    assert(this->objects->Horizontal_Coins(0, 6, 1));
    assert(this->objects->Vertical_Bricks(0, 7, 1));
    assert(this->objects->Vertical_Bricks(1, 3, 4));
    assert(this->pipePointers->Tall_Reverse_L_Pipe(1, Physics::GROUND_Y, Level::PIPE_EXIT_SMALL_CASTLE, this->args->currentWorld, 0)); //TODO: Exits will need to be updated later!
    assert(this->objects->Scroll_Stop(7, false));

    //Generate the 4-1 underground room
    assert(this->objects->Vertical_Bricks(12, 0, 11));
    assert(this->objects->Change_Brick_And_Scenery(0, Brick::SURFACE, Scenery::NO_SCENERY));
    assert(this->objects->Change_Brick_And_Scenery(2, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    assert(this->objects->Horizontal_Coins(1, 6, 8));
    assert(this->objects->Vertical_Bricks(0, 7, 3));
    assert(this->objects->Horizontal_Coins(0, 10, 10));
    assert(this->objects->Horizontal_Bricks(1, 7, 6));
    assert(this->objects->Vertical_Bricks(6, 7, 3));
    assert(this->objects->Brick_With_Mushroom(3, 7));
    assert(this->pipePointers->Tall_Reverse_L_Pipe(0, Physics::GROUND_Y, Level::PIPE_EXIT_SMALL_CASTLE, this->args->currentWorld, 0)); //TODO: Exits will need to be updated later!
    assert(this->objects->Scroll_Stop(7, false));

    //Generate the 5-1 underground room
    assert(this->objects->Vertical_Bricks(12, 0, 11));
    assert(this->objects->Change_Brick_And_Scenery(0, Brick::SURFACE, Scenery::NO_SCENERY));
    assert(this->objects->Change_Brick_And_Scenery(3, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    assert(this->objects->Vertical_Bricks(1, 4, 2));
    assert(this->objects->Horizontal_Coins(1, 3, 5));
    assert(this->objects->Horizontal_Coins(0, 4, 5));
    assert(this->objects->Horizontal_Bricks(0, 5, 5));
    assert(this->objects->Vertical_Bricks(5, 1, 5));
    assert(this->objects->Horizontal_Bricks(1, 4, 2));
    assert(this->pipePointers->Tall_Reverse_L_Pipe(2, Physics::GROUND_Y, Level::PIPE_EXIT_SMALL_CASTLE, this->args->currentWorld, 0)); //TODO: Exits will need to be updated later!
    assert(this->objects->Brick_With_10_Coins(2, 5));
    assert(this->objects->Scroll_Stop(6, false));

    //Write the header last
    if (!this->header->Write_Header_To_Buffer(Level_Type::UNDERGROUND_BONUS, this->levelCrawler->Get_Level_Attribute(), this->levelCrawler->Get_Starting_Brick(), this->args->headerBackground,
                                              this->args->headerScenery, this->args->levelCompliment, 400, 0, this->args->difficulty,
                                              this->objects->Get_Level_Length(), this->objects->Get_Num_Items(), 0, 0)) return false;
    return this->Write_Buffers_To_File();
}
