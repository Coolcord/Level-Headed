#include "Underwater_Generator.h"
#include "Physics.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

bool Underwater_Generator::Generate_Level() {
    this->midpointHandler->Set_Level_Type(Level_Type::UNDERWATER);

    int x = this->object->Get_Last_Object_Length();
    this->firstPageHandler->Handle_First_Page(x);
    assert(this->Spawn_Intro(x));

    //Create the level
    while (!this->end->Is_End_Written()) {
        x = this->object->Get_Last_Object_Length();
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Underwater_X(x);

        //TODO: Fix probabilities
        //TODO: Add coins to some of the patterns
        switch (qrand()%5) {
        case 0:     this->Brick_Pattern_Distraction(x); break;
        case 1:     this->Corral(x); break;
        case 2:     this->Corral_Series(x); break;
        case 3:     this->Corral_On_Blocks(x); break;
        case 4:     this->Hole(x); break;
        default: assert(false); return false;
        }

        assert(this->end->Handle_End(this->Get_Underwater_X(this->object->Get_Last_Object_Length())));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE, Level_Type::UNDERWATER));

    //Write the header last
    return this->header->Write_Header(Level_Type::UNDERWATER, Level_Attribute::UNDERWATER, Brick::SURFACE, Background::IN_WATER, Scenery::NO_SCENERY, Level_Compliment::TREES, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}

int Underwater_Generator::Get_Underwater_X(int min) {
    //Aim for a lower value... but allow higher values to be possible
    int x = min;
    switch (qrand()%4) {
    case 0:     x += (qrand()%0x04); break;
    case 1:     x += (qrand()%0x06); break;
    case 2:     x += (qrand()%0x08); break;
    case 3:     x += (qrand()%0x0A); break;
    default:    assert(false); return 0;
    }
    if (x > 0x10) x = 0x10;
    return x;
}

bool Underwater_Generator::Spawn_Intro(int x) {
    if (this->object->Get_Num_Objects_Available() < 1) return false;
    assert(this->object->Swimming_Cheep_Cheep_Spawner(x));
    return true;
}

bool Underwater_Generator::Brick_Pattern_Distraction(int x) {
    if (this->object->Get_Num_Objects_Available() < 2) return false;
    if (x == this->object->Get_Last_Object_Length()) ++x;
    if (x > 0x10) x = 0x10;

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
    this->object->Set_Last_Object_Length(2);
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

bool Underwater_Generator::Corral_On_Blocks(int x) {
    int numObjectsAvailable = this->object->Get_Num_Objects_Available();
    if (numObjectsAvailable < 1) return false;

    //Determine the length of the horizontal blocks
    int length = (qrand()%4)+2; //length should be between 2 and 5
    int blocksY = (qrand()%8)+3; //y should be between 3 and 8
    assert(this->object->Horizontal_Blocks(x, blocksY, length));
    --numObjectsAvailable;

    //Spawn the corral
    x = 0;
    int remainingLength = length;
    for (int i = 0; i < length && numObjectsAvailable > 0; ++i) {
        if (qrand()%3 == 0) {
            assert(x <= remainingLength);
            remainingLength -= x;
            int height = 0;
            if (qrand()%4 == 0) height = (qrand()%8)+2;
            else height = (qrand()%4)+2;
            if (blocksY-height < 1) height = (qrand()%(blocksY-2))+2;
            int y = blocksY - height;
            assert(this->object->Corral(x, y, height));
            x = 1;
            --numObjectsAvailable;
        } else {
            ++x;
        }
    }

    //Fix the object length
    assert(remainingLength >= 0);
    if (remainingLength > 0) this->object->Increment_Last_Object_Length(remainingLength);
    return true;
}

bool Underwater_Generator::Hole(int x) {
    int numObjectsAvailable = this->object->Get_Num_Objects_Available();
    if (numObjectsAvailable < 1) return false;

    int holeLength = (qrand()%7)+2; //length is from 2 to 8
    bool sideBarriers = static_cast<bool>(qrand()%4);
    int firstHeight = 0;
    int secondHeight = 0;
    if (numObjectsAvailable < 5) sideBarriers = false;

    //Spawn a hole with uniform side barriers
    if (sideBarriers) {
        firstHeight = (qrand()%5)+2; //height is from 2 to 6
        secondHeight = (qrand()%5)+2;
        assert(this->object->Vertical_Blocks(x, (Physics::GROUND_Y+1)-firstHeight, firstHeight));
        assert(this->object->Vertical_Blocks(1, (Physics::GROUND_Y+1)-secondHeight, secondHeight));
        x = 1;
    }
    assert(this->object->Hole(x, holeLength));
    if (sideBarriers) {
        assert(this->object->Vertical_Blocks(holeLength, (Physics::GROUND_Y+1)-secondHeight, secondHeight));
        assert(this->object->Vertical_Blocks(1, (Physics::GROUND_Y+1)-firstHeight, firstHeight));
        x = 1;
    }
    return true;
}
