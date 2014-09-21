#include "Standard_Overworld_Generator.h"
#include <assert.h>

bool Standard_Overworld_Generator::Generate_Level() {

    //Spawn the Level
    while (!this->end->Is_End_Written()) {
        if (this->object->Get_Num_Objects_Available() >= 3) {
            if (qrand() % 3 == 0) assert(this->commonPatternSpawner->Spawn_Common_Pattern());
            else assert(this->simpleObjectSpawner->Spawn_Simple_Object());
        } else assert(this->simpleObjectSpawner->Spawn_Simple_Object());
        assert(this->end->Handle_End());
    }

    //Write the header last
    return this->header->Write_Header(Level_Type::STANDARD_OVERWORLD, Level_Attribute::OVERWORLD, Level_Compliment::TREES, 400, 5, this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}
