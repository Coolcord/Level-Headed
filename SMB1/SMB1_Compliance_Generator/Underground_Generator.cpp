#include "Underground_Generator.h"
#include "Simple_Object_Spawner.h"
#include "Common_Pattern_Spawner.h"
#include <assert.h>
#include <QDebug>

Underground_Generator::~Underground_Generator() {
    delete this->simpleObjectSpawner;
    delete this->commonPatternSpawner;
}

bool Underground_Generator::Generate_Level() {
    this->simpleObjectSpawner = new Simple_Object_Spawner(this->object, Level_Type::UNDERGROUND);
    this->commonPatternSpawner = new Common_Pattern_Spawner(this->object, Level_Type::UNDERGROUND);

    int x = this->object->Get_Last_Object_Length();
    this->firstPageHandler->Handle_First_Page(x);
    assert(this->Spawn_Intro(x));

    //Create the level
    while (!this->end->Is_End_Written()) {
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Random_X(x, this->object->Get_First_Page_Safety());
        if (this->object->Get_Num_Objects_Available() >= 3) {
            if (qrand() % 3 == 0) assert(this->commonPatternSpawner->Spawn_Common_Pattern(x));
            else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        } else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
        x = this->object->Get_Last_Object_Length();
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::ALL));

    //Write the header last
    return this->header->Write_Header(Level_Type::UNDERGROUND, Level_Attribute::UNDERGROUND, Brick::ALL, this->firstPageHandler->Get_Header_Background(), this->args->headerScenery, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}

bool Underground_Generator::Spawn_Intro(int &x) {
    if (this->object->Get_Num_Objects_Available() < 2) return false;
    this->object->Set_First_Page_Safety(false); //undground levels can ignore the first page safety
    assert(this->object->Change_Brick_And_Scenery(0, Brick::SURFACE, Scenery::NO_SCENERY));
    assert(this->object->Change_Brick_And_Scenery(5, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    x = 4;
    return true;
}
