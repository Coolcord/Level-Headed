#include "Castle_Generator.h"
#include "Physics.h"
#include <assert.h>

bool Castle_Generator::Generate_Level() {
    int x = 0;
    assert(this->Spawn_Intro(x));
    this->midpointHandler->Handle_Midpoint(x);
    this->brick = Brick::SURFACE_4_AND_CEILING_3;

    //Create the level
    while (!this->end->Is_End_Written()) {
        x = this->Get_Safe_Random_X();
        bool success = false;
        switch (qrand()%1) {
        case 0: success = this->Room_With_Single_Firebar_Pillar(x); break;
        //case 1: success = this->Drop_Down_And_Climb_Up_U_Shape(x); break;
        //case 2: success = this->Two_Object_Hole(x); break;
        default: break;
        }
        if (!success && this->object->Get_Num_Objects_Available() > 0) this->object->Horizontal_Blocks(1, Physics::GROUND_Y, 1);
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE));

    //Write the header last
    return this->header->Write_Header(Level_Type::CASTLE, Level_Attribute::CASTLE, Brick::SURFACE_4_AND_CEILING_3, Background::OVER_WATER, Scenery::NO_SCENERY, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}

int Castle_Generator::Get_Lowest_Y_From_Brick(Brick::Brick brick) {
    switch (brick) {
    case Brick::NO_BRICKS:                          return -1; //there's no ground here
    case Brick::SURFACE:                            return Physics::GROUND_Y;
    case Brick::SURFACE_AND_CEILING:                return Physics::GROUND_Y;
    case Brick::SURFACE_AND_CEILING_3:              return Physics::GROUND_Y;
    case Brick::SURFACE_AND_CEILING_4:              return Physics::GROUND_Y;
    case Brick::SURFACE_AND_CEILING_8:              return Physics::GROUND_Y;
    case Brick::SURFACE_4_AND_CEILING:              return 7;
    case Brick::SURFACE_4_AND_CEILING_3:            return 7;
    case Brick::SURFACE_4_AND_CEILING_4:            return 7;
    case Brick::SURFACE_5_AND_CEILING:              return 6;
    case Brick::CEILING:                            return -1; //there's no ground here
    case Brick::SURFACE_5_AND_CEILING_4:            return 6;
    case Brick::SURFACE_8_AND_CEILING:              return 2;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:   return Physics::GROUND_Y;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:   return Physics::GROUND_Y;
    case Brick::ALL:                                return -1; //there's no ground here
    default: assert(false); return 0;
    }
}

bool Castle_Generator::Spawn_Intro(int &x) {
    if (this->object->Get_Num_Objects_Available() < 3) return false;
    assert(this->object->Horizontal_Blocks(0, 5, 3));
    assert(this->object->Horizontal_Blocks(0, 6, 4));
    assert(this->object->Horizontal_Blocks(0, 7, 5));
    this->object->Set_Last_Object_Length(16);
    x = 16;
    return true;
}

bool Castle_Generator::Room_With_Single_Firebar_Pillar(int x) {
    if (this->object->Get_Num_Objects_Available() < 5) return false;

    //Spawn a hole
    if (this->brick != Brick::CEILING) assert(this->object->Change_Brick_And_Scenery(x, Brick::CEILING, Scenery::NO_SCENERY));
    x = (qrand()%3)+3; //between 3 and 5
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING, Scenery::NO_SCENERY));

    //Spawn a center platform with a possible firebar
    x = (qrand()%2)+2; //between 2 and 3
    assert(this->object->Used_Block(x, 8));
    if (this->requiredEnemySpawns->Is_Safe_To_Add_Required_Enemy_Spawn(0)) {
        if (qrand()%6==0) {
            assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::LARGE_FIRE_BAR, 0, 8));
        } else {
            Extra_Enemy_Args args = this->requiredEnemySpawns->Get_Initialized_Extra_Enemy_Args();
            args.fast = static_cast<bool>(qrand()%2==0);
            args.clockwise = static_cast<bool>(qrand()%2==0);
            assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::FIRE_BAR, args, 0, 8));
        }
    }
    x = (qrand()%2)+1; //between 1 and 3

    //Spawn another hole
    assert(this->object->Change_Brick_And_Scenery(x, Brick::CEILING, Scenery::NO_SCENERY));
    x = (qrand()%3)+3; //between 3 and 5
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING_3, Scenery::NO_SCENERY));
    this->object->Set_Last_Object_Length(2);
    this->brick = Brick::SURFACE_4_AND_CEILING_3;

    return true;
}

bool Castle_Generator::Drop_Down_And_Climb_Up_U_Shape(int x) {
    if (this->object->Get_Num_Objects_Available() < 6) return false;

    if (this->brick != Brick::SURFACE_4_AND_CEILING) assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING, Scenery::NO_SCENERY));
    x = (qrand()%3)+2; //between 2 and 4
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE_8_AND_CEILING, Scenery::NO_SCENERY));
    x = (qrand()%3)+2; //between 2 and 4
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    x = (qrand()%3)+2; //between 2 and 4
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE_AND_CEILING_8, Scenery::NO_SCENERY));
    x = (qrand()%3)+2; //between 2 and 4
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    x = (qrand()%4)+3; //between 3 and 6
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING_3, Scenery::NO_SCENERY));
    this->object->Set_Last_Object_Length(2);
    this->brick = Brick::SURFACE_4_AND_CEILING_3;
    return true;
}

bool Castle_Generator::Two_Object_Hole(int x) {
    if (this->object->Get_Num_Objects_Available() < 2) return false;

    assert(this->object->Change_Brick_And_Scenery(x, Brick::CEILING, Scenery::NO_SCENERY));
    x = (qrand()%4)+2; //between 2 and 5
    assert(this->object->Change_Brick_And_Scenery(x, this->brick, Scenery::NO_SCENERY));
    this->object->Set_Last_Object_Length(2);

    return true;
}

