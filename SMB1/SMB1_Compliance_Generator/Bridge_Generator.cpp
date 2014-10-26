#include "Bridge_Generator.h"
#include <assert.h>

bool Bridge_Generator::Generate_Level() {
    //Create the level
    while (!this->end->Is_End_Written()) {
        int x = this->object->Get_Last_Object_Length();
        this->firstPageHandler->Handle_First_Page(x);
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Random_X(x, this->object->Get_First_Page_Safety());



        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE, Level_Type::BRIDGE));

    //Write the header last
    return this->header->Write_Header(Level_Type::BRIDGE, Level_Attribute::OVERWORLD, Brick::SURFACE, Background::BLANK_BACKGROUND, Scenery::NO_SCENERY, Level_Compliment::TREES, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}
