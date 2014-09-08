#include "Standard_Overworld_Generator.h"

bool Standard_Overworld_Generator::Generate_Level() {

    //Write the header last
    return this->header->Write_Header(Level_Type::STANDARD_OVERWORLD, Level_Attribute::OVERWORLD, Level_Compliment::TREES, 400, 5, this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}
