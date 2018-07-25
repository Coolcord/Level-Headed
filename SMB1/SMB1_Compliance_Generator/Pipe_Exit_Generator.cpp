#include "Pipe_Exit_Generator.h"

bool Pipe_Exit_Generator::Generate_Level() {
    //Spawn the exit pipe
    if (!this->object->Pipe(4, 9, 2)) return false;

    //Write the stairs, flag, and castle
    if (!this->end->Handle_End(2, true)) return false;

    //Write the header last
    return this->header->Write_Header(Level_Type::PIPE_EXIT, Level_Attribute::OVERWORLD, Brick::SURFACE, this->firstPageHandler->Get_Header_Background(),
                                      this->args->headerScenery, this->args->levelCompliment, 400, 0, this->args->difficulty,
                                      this->object->Get_Level_Length(), this->object->Get_Num_Items(), 0, 0);
}
