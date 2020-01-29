#include "Island_Generator.h"
#include "../../Common_Files/Random.h"
#include "Continuous_Enemies_Spawner.h"
#include "Physics.h"
#include "Item_Spawner.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

Island_Generator::Island_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {
    this->itemSpawner = new Item_Spawner(this->objects, Level_Type::ISLAND);
}

Island_Generator::~Island_Generator() {
    delete this->itemSpawner;
}

bool Island_Generator::Generate_Level() {
    this->levelCrawler->Set_Level_Attribute(Level_Attribute::OVERWORLD);
    this->levelCrawler->Set_Starting_Brick(Brick::SURFACE);
    int x = this->objects->Get_Last_Object_Length();
    this->firstPageHandler->Handle_First_Page(x);
    assert(this->Spawn_Intro(x));

    //Create the level
    while (!this->end->Is_End_Written()) {
        x = this->objects->Get_Last_Object_Length();
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Safe_Jump_Distance(x);

        //TODO: Add support for spawning coins and item boxes above islands
        if (this->objects->Get_Num_Objects_Available() >= 2 && Random::Get_Instance().Get_Num(4) == 0) assert(this->Spawn_Two_Islands(x));
        else assert(this->Spawn_Basic_Island(x));

        assert(this->end->Handle_End(this->Get_Safe_Jump_Distance(this->objects->Get_Last_Object_Length())));
    }

    //Handle Additional Passes
    assert(this->powerupDistributor->Distribute_Powerups());
    assert(this->enemySpawner->Spawn_Enemies());

    //Write the header last
    if (!this->header->Write_Header_To_Buffer(Level_Type::ISLAND, this->levelCrawler->Get_Level_Attribute(), this->levelCrawler->Get_Starting_Brick(), this->firstPageHandler->Get_Header_Background(), this->args->headerScenery, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->args->difficulty, this->objects->Get_Level_Length(),
                                      this->objects->Get_Num_Items(), this->enemies->Get_Num_Items(), 0)) return false;
    return this->Write_Buffers_To_File();
}

bool Island_Generator::Spawn_Intro(int &x) {
    //Decrement x a bit to match SMB1's style
    int autoScrollX = 4;
    this->Handle_Auto_Scroll_Start(autoScrollX);
    if (this->objects->Is_Auto_Scroll_Active()) x -= autoScrollX;
    assert(x >= 0);
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::NO_BRICKS, Scenery::ONLY_CLOUDS));
    this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(0);
    this->objects->Set_Last_Object_Length(1);
    assert(this->objects->Get_Page_Relative_Absolute_X(0) == 0x00);
    return true;
}

int Island_Generator::Get_Island_Length(int min) {
    //Prefer lower numbers
    int length = 0;
    switch (Random::Get_Instance().Get_Num(3)) {
    case 0:     length = Random::Get_Instance().Get_Num(1)+3; break;
    case 1:     length = Random::Get_Instance().Get_Num(3)+3; break;
    case 2:     length = Random::Get_Instance().Get_Num(5)+3; break;
    case 3:     length = Random::Get_Instance().Get_Num(7)+3; break;
    default:    assert(false); return 0;
    }
    if (length < min) length = min;
    return length;
}

int Island_Generator::Get_Safe_Jump_Distance(int min) {
    int x = Random::Get_Instance().Get_Num(3)+2;
    x += min;
    if (x > 0x10) x = 0x10;
    return x;
}

int Island_Generator::Get_Island_Y() {
    int y = this->objects->Get_Current_Y();

    //Determine whether to go up or down
    bool up = Random::Get_Instance().Get_Num(1) == 0;
    if (y <= 2) up = false;
    else if (y >= Physics::GROUND_Y+1) up = true;
    if (up) y -= Random::Get_Instance().Get_Num(4);
    else y += Random::Get_Instance().Get_Num(4);

    //Y values should be between the range of 1 at the highest and 11 at the lowest
    if (y > Physics::GROUND_Y+1) y = Physics::GROUND_Y+1;
    if (y < 1) y = 1;

    return y;
}

bool Island_Generator::Spawn_Basic_Island(int x) {
    if (this->objects->Get_Num_Objects_Available() < 1) return false;
    int y = this->Get_Island_Y();
    int length = this->Get_Island_Length();
    assert(this->objects->Island(x, y, length));
    this->itemSpawner->Spawn_Random_Item(0, length-1, y, Physics::HIGHEST_Y, 0);
    return true;
}

bool Island_Generator::Spawn_Two_Islands(int x) {
    //Determine lowest y
    int y = this->objects->Get_Current_Y();

    //The bottom island should spawn between 5 at the highest and 11 at the lowest
    if (y-4 > 5) { //at these heights, the last y value is relevant
        y -= Random::Get_Instance().Get_Num(4);
    } else { //y is irrelevant, so allow for more freedom
        y = Random::Get_Instance().Get_Num(6)+5;
    }
    int bottomLength = this->Get_Island_Length(5);
    int bottomY = y;
    assert(this->objects->Island(x, y, bottomLength));

    //The top island should spawn 3 - 4 blocks above the bottom island (prefer 4)
    assert(y > 4);
    if (Random::Get_Instance().Get_Num(3) > 0) y -= 4;
    else y -= 3;
    if (y < 1) y = 1;


    //The top island should be shorter than the bottom island
    int topLength = Random::Get_Instance().Get_Num(bottomLength-3)+3;
    if (topLength == bottomLength) --topLength;
    assert(bottomLength > topLength);

    //X should be incremented by at least 1
    x = Random::Get_Instance().Get_Num((bottomLength-topLength)-1)+1;
    assert(this->objects->Island(x, y, topLength));

    //Fix the last object length
    assert(x+topLength <= bottomLength);
    bool incremented = false;
    if (x+topLength < bottomLength) {
        this->objects->Increment_Last_Object_Length(bottomLength-(x+topLength));
        incremented = true;
    }
    int lastObjectLength = this->objects->Get_Last_Object_Length();
    int randomItemX = this->itemSpawner->Spawn_Random_Item(0, topLength-1, y, Physics::HIGHEST_Y, 0);
    lastObjectLength -= randomItemX;
    assert(lastObjectLength >= 0);
    this->objects->Set_Last_Object_Length(lastObjectLength);

    //Make sure the y is reasonable
    if (incremented) this->objects->Set_Current_Y(bottomY);
    return true;
}
