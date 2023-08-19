#include "Pipe_Exit_Generator.h"

bool Pipe_Exit_Generator::Generate_Level() {
    this->powerupDistributor->Deallocate_Powerups();
    this->levelCrawler->Set_Level_Attribute(Level_Attribute::OVERWORLD);
    this->levelCrawler->Set_Starting_Brick(Brick::SURFACE);

    //Spawn the exit pipe
    if (!this->objects->Pipe(3, 9, 2)) return false;

    //Write the stairs, flag, and castle
    if (!this->end->Handle_End(2, true)) return false;

    //Write the header last
    if (!this->header->Write_Header_To_Buffer(Level_Type::PIPE_EXIT, this->levelCrawler->Get_Level_Attribute(), this->levelCrawler->Get_Starting_Brick(), this->args->headerBackground,
                                      this->args->headerScenery, this->args->levelCompliment, 400, 0, this->args->difficulty,
                                      this->objects->Get_Level_Length(), this->objects->Get_Num_Items(), 0, 0)) return false;
    return this->Write_Buffers_To_File();
}
