#include "Castle_Generator.h"
#include "Continuous_Enemies_Spawner.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "Physics.h"
#include <assert.h>

Castle_Generator::Castle_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *a) : Level_Generator(file, a) {
    this->itemSpawner = new Item_Spawner(this->objects, Level_Type::CASTLE);
}

Castle_Generator::~Castle_Generator() {
    delete this->itemSpawner;
}

bool Castle_Generator::Generate_Level() {
    this->levelCrawler->Set_Level_Attribute(Level_Attribute::CASTLE);
    this->levelCrawler->Set_Starting_Brick(Brick::SURFACE_4_AND_CEILING_3);
    this->bowserFireReadyToSpawn = false;
    this->bowserFireSpawned = false;
    //Allocate space for Bowser's Fire, Bowser, Toad, and 2 possible page changes
    this->requiredEnemySpawns->Set_Num_End_Bytes(this->requiredEnemySpawns->Get_Num_End_Bytes()+10);
    int x = 0;
    assert(this->Spawn_Intro(x));
    this->midpointHandler->Handle_Midpoint(x);
    this->brick = this->levelCrawler->Get_Starting_Brick();

    //Create the level
    while (!this->end->Is_End_Written()) {
        x = this->Get_Safe_Random_X();
        bool success = false;
        switch (Random::Get_Instance().Get_Num(6)) {
        case 0: success = this->Room_With_Single_Firebar_Pillar(x); break;
        case 1: success = this->Drop_Down_And_Climb_Up_U_Shape(x); break;
        case 2: success = this->Two_Object_Hole(x); break;
        case 3: success = this->Room_With_Platforms_And_Firebars(x); break;
        case 4: success = this->Coin_Tease(x); break;
        case 5: success = this->Item_Tease(x); break;
        case 6: success = this->Defense_Hole(x); break;
        default: break;
        }
        if (!success) {
            if (this->objects->Get_Num_Objects_Available() >= 2) {
                if (Random::Get_Instance().Get_Num(3)==0) {
                    if (!this->Two_Object_Hole(x)) {
                        if (this->objects->Get_Num_Objects_Available() > 0) this->objects->Horizontal_Blocks(1, Physics::GROUND_Y, 1);
                    }
                } else {
                    if (!this->Defense_Hole(x)) {
                        if (this->objects->Get_Num_Objects_Available() > 0) this->objects->Horizontal_Blocks(1, Physics::GROUND_Y, 1);
                    }
                }
            } else if (this->objects->Get_Num_Objects_Available() > 0) this->objects->Horizontal_Blocks(1, Physics::GROUND_Y, 1);
        }

        this->Handle_Bowser_Fire();
        assert(this->objects->Get_Num_Objects_Available() >= 0);
        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Handle Additional Passes
    assert(this->powerupDistributor->Distribute_Powerups());
    assert(this->enemySpawner->Spawn_Enemies());

    //Write the header last
    if (!this->header->Write_Header_To_Buffer(Level_Type::CASTLE, this->levelCrawler->Get_Level_Attribute(), this->levelCrawler->Get_Starting_Brick(), Background::OVER_WATER, Scenery::NO_SCENERY, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->args->difficulty, this->objects->Get_Level_Length(),
                                      this->objects->Get_Num_Items(), this->enemies->Get_Num_Items(), 0)) return false;
    return this->Write_Buffers_To_File();
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
    }
    assert(false); return 0;
}

int Castle_Generator::Get_Random_Y() {
    int y = this->objects->Get_Current_Y();

    //Determine whether to go up or down
    if (y == Physics::HIGHEST_Y) y += Random::Get_Instance().Get_Num(4);
    else if (y == Physics::GROUND_Y+1) y -= Random::Get_Instance().Get_Num(4);
    else {
        if (Random::Get_Instance().Get_Num(1) == 0) { //go up
            y -= Random::Get_Instance().Get_Num(4);
        } else { //go down
            y += Random::Get_Instance().Get_Num(4);
        }
    }
    //Y values should be between the range of 1 at the highest and 11 at the lowest
    if (y > Physics::GROUND_Y+1) y = Physics::GROUND_Y+1;
    if (y < 1) y = 1;

    return y;
}

bool Castle_Generator::Spawn_Firebar(int x, int y) {
    if (this->requiredEnemySpawns->Is_Safe_To_Add_Required_Enemy_Spawn(x)) {
        assert(this->objects->Used_Block(x, y));
        Extra_Enemy_Args args = this->requiredEnemySpawns->Get_Initialized_Extra_Enemy_Args();
        args.fast = static_cast<bool>(Random::Get_Instance().Get_Num(1)==0);
        args.clockwise = static_cast<bool>(Random::Get_Instance().Get_Num(1)==0);
        assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::FIRE_BAR, args, 0, y));
        return true;
    }
    return false;
}

bool Castle_Generator::Spawn_Intro(int &x) {
    if (this->objects->Get_Num_Objects_Available() < 4) return false;
    assert(this->objects->Horizontal_Blocks(0, 5, 3));
    assert(this->objects->Horizontal_Blocks(0, 6, 4));
    assert(this->objects->Horizontal_Blocks(0, 7, 5));
    int autoScrollX = 4;
    this->Handle_Auto_Scroll_Start(autoScrollX);
    Enemy_Item::Enemy_Item spawner = this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(16);
    if (spawner == Enemy_Item::NOTHING || spawner == Enemy_Item::LAKITU) {
        this->objects->Set_Last_Object_Length(16);
    } else {
        this->objects->Set_Last_Object_Length(1);
    }
    x = 16;
    return true;
}

void Castle_Generator::Handle_Bowser_Fire() {
    if (this->bowserFireReadyToSpawn) {
        if (!this->bowserFireSpawned) {
            assert(this->requiredEnemySpawns->Set_Num_End_Bytes(this->requiredEnemySpawns->Get_Num_End_Bytes()-4));
            assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::BOWSER_FIRE_SPAWNER, 0));
            this->bowserFireSpawned = true;
        }
    } else {
        //Start preparing when 1/3rd of the level is left or when there are less than 3 pages left
        if (this->objects->Get_Num_Objects_Available() < (this->args->numObjectBytes/6)+2 ||
                (this->args->maxLevelLength > 0 && this->objects->Get_Level_Length() > static_cast<int>((static_cast<double>(this->args->maxLevelLength)/3.0)*2.0)) ||
                (this->args->maxLevelLength-this->objects->Get_Level_Length() < 48)) {
            if (this->objects->Get_Page_Relative_Absolute_X(0) == 0xF) assert(this->objects->Cancel_Spawner(1));
            else assert(this->objects->Cancel_Spawner(0));
            this->bowserFireReadyToSpawn = true;
        }
    }
}

bool Castle_Generator::Room_With_Single_Firebar_Pillar(int x) {
    if (this->objects->Get_Num_Objects_Available() < 5) return false;

    //Spawn a hole
    if (this->brick != Brick::CEILING) assert(this->objects->Change_Brick_And_Scenery(x, Brick::CEILING, Scenery::NO_SCENERY));
    x = Random::Get_Instance().Get_Num(2)+3; //between 3 and 5
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING, Scenery::NO_SCENERY));

    //Spawn a center platform with a possible firebar
    x = Random::Get_Instance().Get_Num(1)+2; //between 2 and 3
    if (this->requiredEnemySpawns->Is_Safe_To_Add_Required_Enemy_Spawn(x)) {
        assert(this->objects->Used_Block(x, 8));
        if (Random::Get_Instance().Get_Num(5)==0) {
            assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::LARGE_FIRE_BAR, 0, 8));
        } else {
            Extra_Enemy_Args args = this->requiredEnemySpawns->Get_Initialized_Extra_Enemy_Args();
            args.fast = static_cast<bool>(Random::Get_Instance().Get_Num(1)==0);
            args.clockwise = static_cast<bool>(Random::Get_Instance().Get_Num(1)==0);
            assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::FIRE_BAR, args, 0, 8));
        }
    }
    x = Random::Get_Instance().Get_Num(1)+1; //between 1 and 3

    //Spawn another hole
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::CEILING, Scenery::NO_SCENERY));
    x = Random::Get_Instance().Get_Num(2)+3; //between 3 and 5
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING_3, Scenery::NO_SCENERY));
    this->objects->Set_Last_Object_Length(2);
    this->brick = Brick::SURFACE_4_AND_CEILING_3;

    return true;
}

bool Castle_Generator::Room_With_Platforms_And_Firebars(int x) {
    int numObjectsAvailable = this->objects->Get_Num_Objects_Available();
    if (numObjectsAvailable < 9) return false; //(numPlatforms*2)+3

    //Spawn anywhere between 3 and 6 platforms
    int numPlatforms = Random::Get_Instance().Get_Num(3, 6);
    if (numObjectsAvailable < 15) numPlatforms = 5;
    if (numObjectsAvailable < 13) numPlatforms = 4;
    if (numObjectsAvailable < 11) numPlatforms = 3;
    assert(numPlatforms >= 3);

    //Make sure that there is a place to stand
    if (this->brick != Brick::SURFACE_4_AND_CEILING_3) {
        assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING_3, Scenery::NO_SCENERY));
        x = Random::Get_Instance().Get_Num(3)+3; //between 3 and 6
    }
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::NO_BRICKS, Scenery::NO_SCENERY));
    this->objects->Set_Last_Object_Length(1);
    this->objects->Set_Current_Y(8);
    for (int i = numPlatforms; i > 0; --i) {
        x = this->objects->Get_Last_Object_Length()+Random::Get_Instance().Get_Num(3)+2; //between 2 and 5
        int length = Random::Get_Instance().Get_Num(3)+2; //between 2 and 5
        int y = this->Get_Random_Y();
        if (i == 1 && y < 5) y = 5;
        assert(this->objects->Horizontal_Blocks(x, y, length));

        //Possibly spawn something on the platform
        if (this->objects->Get_Num_Objects_Available()-(i+1) > 0) {
            int random = Random::Get_Instance().Get_Num(1);
            if (random == 0) { //spawn a firebar
                x = Random::Get_Instance().Get_Num(length-1);
                if (this->requiredEnemySpawns->Is_Safe_To_Add_Required_Enemy_Spawn(x)) {
                    assert(this->objects->Used_Block(x, y));
                    this->objects->Set_Last_Object_Length(length-x);
                    if (Random::Get_Instance().Get_Num(5)==0 && (y < 8 && y > 2)) {
                        assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::LARGE_FIRE_BAR, 0, y));
                    } else {
                        Extra_Enemy_Args args = this->requiredEnemySpawns->Get_Initialized_Extra_Enemy_Args();
                        args.fast = static_cast<bool>(Random::Get_Instance().Get_Num(1)==0);
                        args.clockwise = static_cast<bool>(Random::Get_Instance().Get_Num(1)==0);
                        assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::FIRE_BAR, args, 0, y));
                    }
                }
            } else {
                bool noBlocks = false;
                if (i == 1 && y < 5) noBlocks = true; //prevent players from jumping onto the ceiling and getting stuck
                this->itemSpawner->Spawn_Random_Item(0, length-1, y, Physics::HIGHEST_Y, i+1, noBlocks); //don't forget about the change brick at the end!
            }
        }
    }

    x = this->objects->Get_Last_Object_Length()+Random::Get_Instance().Get_Num(2)+2; //between 2 and 4
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING_3, Scenery::NO_SCENERY));
    this->objects->Set_Last_Object_Length(2);
    this->brick = Brick::SURFACE_4_AND_CEILING_3;
    return true;
}

bool Castle_Generator::Drop_Down_And_Climb_Up_U_Shape(int x) {
    if (this->objects->Get_Num_Objects_Available() < 6) return false;

    if (this->brick != Brick::SURFACE_4_AND_CEILING) assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING, Scenery::NO_SCENERY));
    x = Random::Get_Instance().Get_Num(2)+2; //between 2 and 4
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_8_AND_CEILING, Scenery::NO_SCENERY));
    x = Random::Get_Instance().Get_Num(2)+2; //between 2 and 4
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    x = Random::Get_Instance().Get_Num(2)+2; //between 2 and 4
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_AND_CEILING_8, Scenery::NO_SCENERY));
    x = Random::Get_Instance().Get_Num(2)+2; //between 2 and 4
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    x = Random::Get_Instance().Get_Num(3)+3; //between 3 and 6
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING_3, Scenery::NO_SCENERY));
    this->objects->Set_Last_Object_Length(2);
    this->brick = Brick::SURFACE_4_AND_CEILING_3;
    return true;
}

bool Castle_Generator::Two_Object_Hole(int x) {
    if (this->objects->Get_Num_Objects_Available() < 2) return false;

    assert(this->objects->Change_Brick_And_Scenery(x, Brick::CEILING, Scenery::NO_SCENERY));
    x = Random::Get_Instance().Get_Num(3)+2; //between 2 and 5
    //Possibly add a Podoboo
    if (Random::Get_Instance().Get_Num(1) == 0) {
        int podobooX = Random::Get_Instance().Get_Num(1, x);
        if (this->requiredEnemySpawns->Is_Safe_To_Add_Required_Enemy_Spawn(podobooX)) {
            assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::PODOBOO, podobooX));
        }
    }
    assert(this->objects->Change_Brick_And_Scenery(x, this->brick, Scenery::NO_SCENERY));
    this->objects->Set_Last_Object_Length(2);

    return true;
}

bool Castle_Generator::Coin_Tease(int x) {
    if (this->objects->Get_Num_Objects_Available() < 5) return false;
    if (this->requiredEnemySpawns->Get_Num_Bytes_Left() < 4) return false;

    if (this->brick != Brick::SURFACE) {
        assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE, Scenery::NO_SCENERY));
        x = Random::Get_Instance().Get_Num(4)+2; //between 2 and 6
    }

    //Spawn the coins
    int length = Random::Get_Instance().Get_Num(4)+3; //between 3 and 7
    int fireBarY = Random::Get_Instance().Get_Num(3)+6; //between 6 and 9
    assert(this->objects->Horizontal_Coins(x, fireBarY-(Random::Get_Instance().Get_Num(2)+1), length));
    assert(this->objects->Horizontal_Coins(0, Physics::GROUND_Y, length));

    //Spawn the firebar
    int usedBlockX = 0;
    if (length%2 == 0) usedBlockX = (length/2)-Random::Get_Instance().Get_Num(1);
    else usedBlockX = length/2;
    assert(this->objects->Used_Block(usedBlockX, fireBarY));
    if (Random::Get_Instance().Get_Num(5)==0 && (fireBarY < 8 && fireBarY > 2)) {
        assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::LARGE_FIRE_BAR, 0, fireBarY));
    } else {
        Extra_Enemy_Args args = this->requiredEnemySpawns->Get_Initialized_Extra_Enemy_Args();
        args.fast = static_cast<bool>(Random::Get_Instance().Get_Num(1)==0);
        args.clockwise = static_cast<bool>(Random::Get_Instance().Get_Num(1)==0);
        assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::FIRE_BAR, args, 0, fireBarY));
    }
    x = usedBlockX + Random::Get_Instance().Get_Num(4)+2; //between 2 and 6
    assert(this->objects->Change_Brick_And_Scenery(x, this->brick, Scenery::NO_SCENERY));
    this->objects->Set_Last_Object_Length(2);
    return true;
}

bool Castle_Generator::Item_Tease(int x) {
    if (this->objects->Get_Num_Objects_Available() < 7) return false;

    Brick::Brick brick;
    switch (Random::Get_Instance().Get_Num(5)) {
    case 0: brick = Brick::SURFACE_4_AND_CEILING; break;
    case 1: brick = Brick::SURFACE_5_AND_CEILING; break;
    case 2: brick = Brick::SURFACE; break;
    case 3: brick = Brick::SURFACE_AND_CEILING; break;
    case 4: brick = Brick::SURFACE_AND_CEILING_3; break;
    case 5: brick = Brick::SURFACE_AND_CEILING_4; break;
    default: assert(false); return false;
    }
    if (this->brick != brick) {
        assert(this->objects->Change_Brick_And_Scenery(x, brick, Scenery::NO_SCENERY));
        x = Random::Get_Instance().Get_Num(3)+3; //between 3 and 6
    }
    int y = this->Get_Lowest_Y_From_Brick(brick)-3;

    if (Random::Get_Instance().Get_Num(1) == 0) { //one firebar
        bool uniform = !(static_cast<bool>((Random::Get_Instance().Get_Num(3)==0))); //75% chance
        int length = Random::Get_Instance().Get_Num(2)+1; //between 1 and 3
        for (int i = 0; i < 2; ++i) {
            if (y == 3 || y == 7) {
                if (!uniform) length = Random::Get_Instance().Get_Num(2)+1; //between 1 and 3
                if (length == 1) {
                    assert(this->objects->Question_Block_With_Coin(x, y));
                    x = 1;
                } else {
                    assert(this->objects->Horizontal_Question_Blocks_With_Coins(x, y, length));
                    x = length;
                }
            } else {
                assert(this->objects->Question_Block_With_Coin(x, y));
                x = 1;
            }
            if (i != 1 && !this->Spawn_Firebar(x, y)) break;
            x = 1;
        }
    } else { //two firebars
        if (this->Spawn_Firebar(x, y)) x = 1;
        else x = Random::Get_Instance().Get_Num(3)+3; //between 3 and 6
        int length = 1;
        if (y == 3 || y == 7) {
            length += Random::Get_Instance().Get_Num(2);
            if (length == 1) {
                assert(this->objects->Question_Block_With_Coin(x, y));
                x = 1;
            } else {
                assert(this->objects->Horizontal_Question_Blocks_With_Coins(x, y, length));
                x = length;
            }
        } else {
            assert(this->objects->Question_Block_With_Coin(x, y));
        }
        this->Spawn_Firebar(x, y);
    }

    x = this->objects->Get_Last_Object_Length()+Random::Get_Instance().Get_Num(2)+2; //between 2 and 5
    assert(this->objects->Change_Brick_And_Scenery(x, this->brick, Scenery::NO_SCENERY));
    this->objects->Set_Last_Object_Length(2);
    return true;
}

bool Castle_Generator::Defense_Hole(int x) {
    if (this->objects->Get_Num_Objects_Available() < 2 || this->brick == Brick::SURFACE_AND_CEILING) return false;

    assert(this->objects->Change_Brick_And_Scenery(x, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    if (Random::Get_Instance().Get_Num(2)==0) x = Random::Get_Instance().Get_Num(3)+2; //between 2 and 5
    else x = Random::Get_Instance().Get_Num(1)+2; //between 2 and 3
    assert(this->objects->Change_Brick_And_Scenery(x, this->brick, Scenery::NO_SCENERY));
    return true;
}

