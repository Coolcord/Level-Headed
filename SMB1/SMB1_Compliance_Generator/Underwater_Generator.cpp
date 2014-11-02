#include "Underwater_Generator.h"
#include "Physics.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

bool Underwater_Generator::Generate_Level() {
    this->midpointHandler->Set_Level_Type(Level_Type::UNDERWATER);

    //Create the level
    while (!this->end->Is_End_Written()) {
        int x = this->Get_Safe_Random_X();
        this->firstPageHandler->Handle_First_Page(x);
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Random_X(x, this->object->Get_First_Page_Safety());

        //TODO: Fix probabilities
        switch (qrand()%3) {
        case 0:     this->Brick_Pattern_Distraction(x); break;
        case 1:     this->Corral(x); break;
        case 2:     this->Corral_Series(x); break;
        default: assert(false); return false;
        }

        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE, Level_Type::UNDERWATER));

    //Write the header last
    return this->header->Write_Header(Level_Type::UNDERWATER, Level_Attribute::UNDERWATER, Brick::SURFACE, Background::IN_WATER, Scenery::NO_SCENERY, Level_Compliment::TREES, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}

bool Underwater_Generator::Brick_Pattern_Distraction(int x) {
    if (this->object->Get_Num_Objects_Available() < 2) return false;

    //Determine which kind of brick pattern to use
    Brick::Brick brick = Brick::NO_BRICKS;
    switch (qrand()%10) {
    case 0:     brick = Brick::SURFACE_AND_CEILING_4; break;
    case 1:     brick = Brick::SURFACE_AND_CEILING_8; break;
    case 2:     brick = Brick::SURFACE_4_AND_CEILING; break;
    case 3:     brick = Brick::SURFACE_4_AND_CEILING_3; break;
    case 4:     brick = Brick::SURFACE_4_AND_CEILING_4; break;
    case 5:     brick = Brick::SURFACE_5_AND_CEILING; break;
    case 6:     brick = Brick::SURFACE_5_AND_CEILING_4; break;
    case 7:     brick = Brick::SURFACE_8_AND_CEILING; break;
    case 8:     brick = Brick::SURFACE_AND_CEILING_AND_MIDDLE_5; break;
    case 9:     brick = Brick::SURFACE_AND_CEILING_AND_MIDDLE_4; break;
    default:    assert(false); return false;
    }
    assert(this->object->Change_Brick_And_Scenery(x, brick, Scenery::NO_SCENERY));

    //The length of the brick pattern will be between 2 and 8
    assert(this->object->Change_Brick_And_Scenery((qrand()%7)+2, Brick::SURFACE, Scenery::NO_SCENERY));

    return true;
}

bool Underwater_Generator::Corral(int x) {
    if (this->object->Get_Num_Objects_Available() < 1) return false;

    //The corral can be anywhere between 2 - 9 in height
    int height = 0;
    if (qrand()%4 == 0) height = (qrand()%8)+2;
    else height = (qrand()%4)+2;
    int y = Physics::GROUND_Y - height + 1;
    assert(this->object->Corral(x, y, height));
    return true;
}

bool Underwater_Generator::Corral_Series(int x) {
    int numObjectsAvailable = this->object->Get_Num_Objects_Available();
    if (numObjectsAvailable < 3) return false;

    //Use between 3 and 5 corral
    int numCorral = (qrand()%3)+3;
    if (numCorral > numObjectsAvailable) numCorral = numObjectsAvailable;

    //Spawn the Corral Series
    bool uniformDistance = static_cast<bool>(qrand()%2);
    assert(this->Corral(x));
    x = (qrand()%3)+1;
    for (int i = 1; i < numCorral; ++i) {
        if (!uniformDistance) x = (qrand()%3)+1;
        assert(this->Corral(x));
    }
    return true;
}
