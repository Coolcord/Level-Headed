#include "Pipe_Intro_Generator.h"

bool Pipe_Intro_Generator::Generate_Level() {
    this->powerupDistributor->Deallocate_Powerups();
    this->levelCrawler->Set_Level_Attribute(Level_Attribute::OVERWORLD);
    this->levelCrawler->Set_Starting_Brick(Brick::SURFACE);
    Background::Background background = this->args->headerBackground;
    int x = 10;

    //Spawn the Castle
    switch (this->args->startCastle) {
        case Castle::NONE:
        break;
        case Castle::SMALL:
        if (!this->objects->Castle(0)) return false;
        break;
        case Castle::BIG:
        background = Background::CASTLE_WALL;
        if (!this->objects->Big_Castle(0)) return false;
        if (!this->objects->Change_Background(6, this->args->headerBackground)) return false;
        x -= 6;
        break;
    }

    //Spawn the L Pipe
    if (!this->objects->Reverse_L_Pipe_To_Next_Level(x)) return false;
    if (!this->objects->Scroll_Stop(9, false)) return false;

    //Write the header last
    if (!this->header->Write_Header_To_Buffer(Level_Type::PIPE_INTRO, this->levelCrawler->Get_Level_Attribute(), this->levelCrawler->Get_Starting_Brick(), background,
                                              this->args->headerScenery, this->args->levelCompliment, 400, 0, this->args->difficulty,
                                              this->objects->Get_Level_Length(), this->objects->Get_Num_Items(), 0, 0)) return false;
    return this->Write_Buffers_To_File();
}
