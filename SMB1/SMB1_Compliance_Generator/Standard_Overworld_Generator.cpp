#include "Standard_Overworld_Generator.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "../Common_SMB1_Files/Castle.h"
#include "Common_Pattern_Spawner.h"
#include "Continuous_Enemies_Spawner.h"
#include "Simple_Object_Spawner.h"
#include <assert.h>
#include <QDebug>

Standard_Overworld_Generator::~Standard_Overworld_Generator() {
    delete this->simpleObjectSpawner;
    delete this->commonPatternSpawner;
}

bool Standard_Overworld_Generator::Generate_Level() {
    this->simpleObjectSpawner = new Simple_Object_Spawner(this->objects, Level_Type::STANDARD_OVERWORLD);
    this->commonPatternSpawner = new Common_Pattern_Spawner(this->objects, Level_Type::STANDARD_OVERWORLD);

    //Spawn the Intro
    this->levelCrawler->Set_Level_Attribute(Level_Attribute::OVERWORLD);
    this->levelCrawler->Set_Starting_Brick(Brick::SURFACE);
    int x = this->objects->Get_Last_Object_Length();
    this->Spawn_Intro(x);

    //Create the level
    while (!this->end->Is_End_Written()) {
        x = this->objects->Get_Last_Object_Length();
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Random_X(x, this->objects->Get_First_Page_Safety());
        if (this->objects->Get_Num_Objects_Available() >= 3) {
            if (Random::Get_Instance().Get_Num(2) == 0) assert(this->commonPatternSpawner->Spawn_Common_Pattern(x));
            else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        }  else if (this->objects->Get_Num_Objects_Available() > 0) assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        assert(this->objects->Get_Num_Objects_Available() >= 0);
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Handle Additional Passes
    this->Remove_Unsafe_Cannons_From_Level();
    assert(this->powerupDistributor->Distribute_Powerups());
    assert(this->enemySpawner->Spawn_Enemies());

    //Write the header last
    if (!this->header->Write_Header_To_Buffer(Level_Type::STANDARD_OVERWORLD, this->levelCrawler->Get_Level_Attribute(), this->levelCrawler->Get_Starting_Brick(), this->firstPageHandler->Get_Header_Background(), this->args->headerScenery, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->args->difficulty, this->objects->Get_Level_Length(),
                                      this->objects->Get_Num_Items(), this->enemies->Get_Num_Items(), 0)) return false;
    return this->Write_Buffers_To_File();
}

void Standard_Overworld_Generator::Spawn_Intro(int &x) {
    this->firstPageHandler->Handle_First_Page(x);
    int autoScrollX = 4;
    this->Handle_Auto_Scroll_Start(autoScrollX);
    if (!this->objects->Was_Auto_Scroll_Used()) autoScrollX = 0;
    Enemy_Item::Enemy_Item spawner = this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(x);
    if (spawner == Enemy_Item::NOTHING || spawner == Enemy_Item::LAKITU) {
        if (args->startCastle == Castle::BIG) this->objects->Set_Last_Object_Length(11-autoScrollX);
        else this->objects->Set_Last_Object_Length(17-autoScrollX);
        this->objects->Set_First_Page_Safety(false);
    }
}
