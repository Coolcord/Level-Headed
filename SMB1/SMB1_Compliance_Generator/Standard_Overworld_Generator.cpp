#include "../../Common_Files/Random.h"
#include "Common_Pattern_Spawner.h"
#include "Continuous_Enemies_Spawner.h"
#include "Standard_Overworld_Generator.h"
#include "Simple_Object_Spawner.h"
#include <assert.h>
#include <QDebug>

Standard_Overworld_Generator::~Standard_Overworld_Generator() {
    delete this->simpleObjectSpawner;
    delete this->commonPatternSpawner;
}

bool Standard_Overworld_Generator::Generate_Level() {
    this->simpleObjectSpawner = new Simple_Object_Spawner(this->object, Level_Type::STANDARD_OVERWORLD);
    this->commonPatternSpawner = new Common_Pattern_Spawner(this->object, Level_Type::STANDARD_OVERWORLD);

    //Spawn the Intro
    this->Spawn_Intro();

    //Create the level
    while (!this->end->Is_End_Written()) {
        int x = this->object->Get_Last_Object_Length();
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Random_X(x, this->object->Get_First_Page_Safety());
        if (this->object->Get_Num_Objects_Available() >= 3) {
            if (Random::Get_Num(2) == 0) assert(this->commonPatternSpawner->Spawn_Common_Pattern(x));
            else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        } else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE));

    //Write the header last
    return this->header->Write_Header(Level_Type::STANDARD_OVERWORLD, Level_Attribute::OVERWORLD, Brick::SURFACE, this->firstPageHandler->Get_Header_Background(), this->args->headerScenery, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->args->difficulty, this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}

void Standard_Overworld_Generator::Spawn_Intro() {
    int x = this->object->Get_Last_Object_Length();
    this->firstPageHandler->Handle_First_Page(x);

    //int spawnerX = 16-x;
    //if (spawnerX < 0) spawnerX = 0;
    Enemy_Item::Enemy_Item spawner = this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(x);
    if (spawner == Enemy_Item::NOTHING || spawner == Enemy_Item::LAKITU) {
        this->object->Set_Last_Object_Length(17);
    }
}
