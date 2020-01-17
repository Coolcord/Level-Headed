#include "Standard_Overworld_Generator.h"
#include "../../Common_Files/Random.h"
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
    this->simpleObjectSpawner = new Simple_Object_Spawner(this->object, Level_Type::STANDARD_OVERWORLD);
    this->commonPatternSpawner = new Common_Pattern_Spawner(this->object, Level_Type::STANDARD_OVERWORLD);

    //Spawn the Intro
    int x = this->object->Get_Last_Object_Length();
    this->Spawn_Intro(x);

    //Create the level
    while (!this->end->Is_End_Written()) {
        x = this->object->Get_Last_Object_Length();
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Random_X(x, this->object->Get_First_Page_Safety());
        if (this->object->Get_Num_Objects_Available() >= 3) {
            if (Random::Get_Instance().Get_Num(2) == 0) assert(this->commonPatternSpawner->Spawn_Common_Pattern(x));
            else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        } else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE));

    //Write the header last
    if (!this->header->Write_Header_To_Buffer(Level_Type::STANDARD_OVERWORLD, Level_Attribute::OVERWORLD, Brick::SURFACE, this->firstPageHandler->Get_Header_Background(), this->args->headerScenery, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->args->difficulty, this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0)) return false;
    return this->Write_Buffers_To_File();
}

void Standard_Overworld_Generator::Spawn_Intro(int &x) {
    this->firstPageHandler->Handle_First_Page(x);
    int autoScrollX = 4;
    this->Handle_Auto_Scroll_Start(autoScrollX);
    Enemy_Item::Enemy_Item spawner = this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(x);
    if (spawner == Enemy_Item::NOTHING || spawner == Enemy_Item::LAKITU) {
        if (args->startCastle == Castle::BIG) this->object->Set_Last_Object_Length(10-autoScrollX);
        else this->object->Set_Last_Object_Length(17-autoScrollX);
    }
}
