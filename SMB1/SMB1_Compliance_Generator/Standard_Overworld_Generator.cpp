#include "Standard_Overworld_Generator.h"
#include <assert.h>

bool Standard_Overworld_Generator::Generate_Level() {

    while (this->object->Get_Num_Bytes_Left() > 0) {
        assert(this->simpleObjectSpawner->Spawn_Simple_Object());
    }

    //Write the header last
    return this->header->Write_Header(Level_Type::STANDARD_OVERWORLD, Level_Attribute::OVERWORLD, Level_Compliment::TREES, 400, 5, this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}
