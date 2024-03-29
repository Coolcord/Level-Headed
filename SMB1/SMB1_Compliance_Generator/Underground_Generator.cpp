#include "Underground_Generator.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "Continuous_Enemies_Spawner.h"
#include "Simple_Object_Spawner.h"
#include "Common_Pattern_Spawner.h"
#include <assert.h>
#include <QDebug>

Underground_Generator::~Underground_Generator() {
    delete this->simpleObjectSpawner;
    delete this->commonPatternSpawner;
}

bool Underground_Generator::Generate_Level() {
    this->simpleObjectSpawner = new Simple_Object_Spawner(this->objects, Level_Type::UNDERGROUND);
    this->commonPatternSpawner = new Common_Pattern_Spawner(this->objects, Level_Type::UNDERGROUND, this->args);

    this->levelCrawler->Set_Level_Attribute(Level_Attribute::UNDERGROUND);
    this->levelCrawler->Set_Starting_Brick(Brick::ALL);
    int x = this->objects->Get_Last_Object_Length();
    this->firstPageHandler->Handle_First_Page(x);
    assert(this->Spawn_Intro(x));

    //Create the level
    while (!this->end->Is_End_Written()) {
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Random_X(x, this->objects->Get_First_Page_Safety());
        if (this->objects->Get_Num_Objects_Available() >= 3) {
            int random = Random::Get_Instance().Get_Num(5);
            if (random < 3) assert(this->commonPatternSpawner->Spawn_Common_Pattern(x));
            else if (random < 4) assert(this->Brick_Pattern_Distraction(x));
            else assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        } else if (this->objects->Get_Num_Objects_Available() > 0) assert(this->simpleObjectSpawner->Spawn_Simple_Object(x));
        assert(this->objects->Get_Num_Objects_Available() >= 0);
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
        x = this->objects->Get_Last_Object_Length();
    }

    //Handle Additional Passes
    this->Remove_Unsafe_Cannons_From_Level();
    assert(this->powerupDistributor->Distribute_Powerups());
    assert(this->enemySpawner->Spawn_Enemies());

    //Write the header last
    if (!this->header->Write_Header_To_Buffer(Level_Type::UNDERGROUND, this->levelCrawler->Get_Level_Attribute(), this->levelCrawler->Get_Starting_Brick(), this->firstPageHandler->Get_Header_Background(), this->args->headerScenery, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->args->difficulty, this->objects->Get_Level_Length(),
                                      this->objects->Get_Num_Items(), this->enemies->Get_Num_Items(), 0)) return false;
    return this->Write_Buffers_To_File();
}

bool Underground_Generator::Spawn_Intro(int &x) {
    if (this->objects->Get_Num_Objects_Available() < 2) return false;
    this->objects->Set_First_Page_Safety(false); //undground levels can ignore the first page safety
    assert(this->objects->Change_Brick_And_Scenery(0, Brick::SURFACE, Scenery::NO_SCENERY));
    assert(this->objects->Change_Brick_And_Scenery(5, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    int autoScrollX = 0;
    this->Handle_Auto_Scroll_Start(autoScrollX);
    Enemy_Item::Enemy_Item spawner = this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(16);
    if (spawner == Enemy_Item::NOTHING || spawner == Enemy_Item::LAKITU) {
        x = 16;
    } else {
        x = 4;
    }
    return true;
}

bool Underground_Generator::Brick_Pattern_Distraction(int x) {
    int numObjectsAvailable = this->objects->Get_Num_Objects_Available();
    if (numObjectsAvailable < 2) return false;
    if (x == this->objects->Get_Last_Object_Length()) ++x;
    if (x > 0x10) x = 0x10;

    //Determine which kind of brick pattern to use
    Brick::Brick brick = Brick::SURFACE_AND_CEILING;
    switch (Random::Get_Instance().Get_Num(8)) {
    case 0:     brick = Brick::SURFACE_AND_CEILING_4; break;
    case 1:     brick = Brick::SURFACE_AND_CEILING_8; break;
    case 2:     brick = Brick::SURFACE_4_AND_CEILING; break;
    case 3:     brick = Brick::SURFACE_4_AND_CEILING_3; break;
    case 4:     brick = Brick::SURFACE_4_AND_CEILING_4; break;
    case 5:     brick = Brick::SURFACE_5_AND_CEILING; break;
    case 6:     brick = Brick::SURFACE_5_AND_CEILING_4; break;
    case 7:     brick = Brick::SURFACE_AND_CEILING_AND_MIDDLE_5; break;
    case 8:     brick = Brick::SURFACE_AND_CEILING_AND_MIDDLE_4; break;
    default:    assert(false); return false;
    }
    assert(this->objects->Change_Brick_And_Scenery(x, brick, Scenery::NO_SCENERY));
    --numObjectsAvailable;

    //The length of the brick pattern will be between 2 and 8
    assert(this->objects->Change_Brick_And_Scenery(Random::Get_Instance().Get_Num(6)+2, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    this->objects->Set_Last_Object_Length(2);
    --numObjectsAvailable;

    //TODO: Possibly follow the SMB 1-2 style from here
    //TODO: Possibly add coins or 10 coin blocks
    return true;
}
