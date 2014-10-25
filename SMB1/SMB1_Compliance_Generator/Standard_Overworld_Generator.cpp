#include "Standard_Overworld_Generator.h"
#include <assert.h>

bool Standard_Overworld_Generator::Generate_Level() {
    //Spawn the Level
    while (!this->end->Is_End_Written()) {
        int x = this->object->Get_Last_Object_Length();
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Random_X(x);
        if (this->object->Get_Num_Objects_Available() >= 3) {
            if (qrand() % 3 == 0) assert(this->commonPatternSpawner->Spawn_Common_Pattern(x));
            else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        } else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE, Level_Type::STANDARD_OVERWORLD));

    //Write the header last
    return this->header->Write_Header(Level_Type::STANDARD_OVERWORLD, Level_Attribute::OVERWORLD, Level_Compliment::TREES, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}
