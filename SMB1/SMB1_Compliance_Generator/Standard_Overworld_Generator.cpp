#include "Standard_Overworld_Generator.h"
#include "Simple_Object_Spawner.h"
#include "Common_Pattern_Spawner.h"
#include <assert.h>
#include <QDebug>

Standard_Overworld_Generator::~Standard_Overworld_Generator() {
    delete this->simpleObjectSpawner;
    delete this->commonPatternSpawner;
}

bool Standard_Overworld_Generator::Generate_Level() {
    this->simpleObjectSpawner = new Simple_Object_Spawner(this->object, Level_Type::STANDARD_OVERWORLD);
    this->commonPatternSpawner = new Common_Pattern_Spawner(this->object, Level_Type::STANDARD_OVERWORLD);

    //Create the level
    while (!this->end->Is_End_Written()) {
        int x = this->object->Get_Last_Object_Length();
        this->firstPageHandler->Handle_First_Page(x);
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Random_X(x, this->object->Get_First_Page_Safety());
        if (this->object->Get_Num_Objects_Available() >= 3) {
            if (qrand() % 3 == 0) assert(this->commonPatternSpawner->Spawn_Common_Pattern(x));
            else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        } else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE));

    //Write the header last
    return this->header->Write_Header(Level_Type::STANDARD_OVERWORLD, Level_Attribute::OVERWORLD, Brick::SURFACE, this->firstPageHandler->Get_Header_Background(), Scenery::MOUNTAINS, Level_Compliment::TREES, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}
