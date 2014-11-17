#include "Castle_Generator.h"
#include <assert.h>

bool Castle_Generator::Generate_Level() {
    int x = 0;
    assert(this->Spawn_Intro(x));
    this->midpointHandler->Handle_Midpoint(x);

    //Create the level
    while (!this->end->Is_End_Written()) {
        x = this->object->Get_Last_Object_Length();
        this->object->Nothing();
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE));

    //Write the header last
    return this->header->Write_Header(Level_Type::CASTLE, Level_Attribute::CASTLE, Brick::SURFACE_4_AND_CEILING_3, Background::OVER_WATER, Scenery::NO_SCENERY, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}

bool Castle_Generator::Spawn_Intro(int &x) {
    if (this->object->Get_Num_Objects_Available() < 3) return false;
    assert(this->object->Horizontal_Blocks(0, 5, 3));
    assert(this->object->Horizontal_Blocks(0, 6, 4));
    assert(this->object->Horizontal_Blocks(0, 7, 5));
    this->object->Set_Last_Object_Length(16);
    x = 16;
    return true;
}
