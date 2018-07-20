#include "Island_Generator.h"
#include "../../Common_Files/Random.h"
#include "Continuous_Enemies_Spawner.h"
#include "Physics.h"
#include "Item_Spawner.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

Island_Generator::Island_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {
    this->itemSpawner = new Item_Spawner(this->object, Level_Type::ISLAND);
}

Island_Generator::~Island_Generator() {
    delete this->itemSpawner;
}

bool Island_Generator::Generate_Level() {
    int x = this->object->Get_Last_Object_Length();
    this->firstPageHandler->Handle_First_Page(x);
    assert(this->Spawn_Intro(x));

    //Create the level
    while (!this->end->Is_End_Written()) {
        x = this->object->Get_Last_Object_Length();
        x = this->Get_Safe_Jump_Distance(x);

        //TODO: Add support for spawning coins and item boxes above islands
        if (this->object->Get_Num_Objects_Available() >= 2 && Random::Get_Num(4) == 0) assert(this->Spawn_Two_Islands(x));
        else assert(this->Spawn_Basic_Island(x));

        assert(this->end->Handle_End(this->Get_Safe_Jump_Distance(this->object->Get_Last_Object_Length())));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE));

    //Write the header last
    return this->header->Write_Header(Level_Type::ISLAND, Level_Attribute::OVERWORLD, Brick::SURFACE, this->firstPageHandler->Get_Header_Background(), this->args->headerScenery, this->args->levelCompliment, 300,
                                      0, this->args->difficulty, this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}

bool Island_Generator::Spawn_Intro(int &x) {
    //Decrement x a bit to match SMB1's style
    assert(this->object->Change_Brick_And_Scenery(x, Brick::NO_BRICKS, Scenery::ONLY_CLOUDS));
    this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(0);
    this->object->Set_Last_Object_Length(1);
    assert(this->object->Get_Absolute_X(0) == 0x00);
    int autoScrollX = 4;
    this->Handle_Auto_Scroll_Start(autoScrollX);
    return true;
}

int Island_Generator::Get_Island_Length(int min) {
    //Prefer lower numbers
    int length = 0;
    switch (Random::Get_Num(3)) {
    case 0:     length = Random::Get_Num(1)+3; break;
    case 1:     length = Random::Get_Num(3)+3; break;
    case 2:     length = Random::Get_Num(5)+3; break;
    case 3:     length = Random::Get_Num(7)+3; break;
    default:    assert(false); return 0;
    }
    if (length < min) length = min;
    return length;
}

int Island_Generator::Get_Safe_Jump_Distance(int min) {
    int x = Random::Get_Num(3)+2;
    x += min;
    if (x > 0x10) x = 0x10;
    return x;
}

int Island_Generator::Get_Island_Y() {
    int y = this->object->Get_Current_Y();

    //Determine whether to go up or down
    bool up = Random::Get_Num(1) == 0;
    if (y <= 2) up = false;
    else if (y >= Physics::GROUND_Y+1) up = true;
    if (up) y -= Random::Get_Num(4);
    else y += Random::Get_Num(4);

    //Y values should be between the range of 1 at the highest and 11 at the lowest
    if (y > Physics::GROUND_Y+1) y = Physics::GROUND_Y+1;
    if (y < 1) y = 1;

    return y;
}

bool Island_Generator::Spawn_Basic_Island(int x) {
    if (this->object->Get_Num_Objects_Available() < 1) return false;
    int y = this->Get_Island_Y();
    int length = this->Get_Island_Length();
    assert(this->object->Island(x, y, length));
    this->itemSpawner->Spawn_Random_Item(0, length-1, y, Physics::HIGHEST_Y, 0);
    return true;
}

bool Island_Generator::Spawn_Two_Islands(int x) {
    //Determine lowest y
    int y = this->object->Get_Current_Y();

    //The bottom island should spawn between 5 at the highest and 11 at the lowest
    if (y-4 > 5) { //at these heights, the last y value is relevant
        y -= Random::Get_Num(4);
    } else { //y is irrelevant, so allow for more freedom
        y = Random::Get_Num(6)+5;
    }
    int bottomLength = this->Get_Island_Length(5);
    int bottomY = y;
    assert(this->object->Island(x, y, bottomLength));

    //The top island should spawn 3 - 4 blocks above the bottom island (prefer 4)
    assert(y > 4);
    if (Random::Get_Num(3) > 0) y -= 4;
    else y -= 3;
    if (y < 1) y = 1;


    //The top island should be shorter than the bottom island
    int topLength = Random::Get_Num(bottomLength-3)+3;
    if (topLength == bottomLength) --topLength;
    assert(bottomLength > topLength);

    //X should be incremented by at least 1
    x = Random::Get_Num((bottomLength-topLength)-1)+1;
    assert(this->object->Island(x, y, topLength));

    //Fix the last object length
    assert(x+topLength <= bottomLength);
    bool incremented = false;
    if (x+topLength < bottomLength) {
        this->object->Increment_Last_Object_Length(bottomLength-(x+topLength));
    }
    this->itemSpawner->Spawn_Random_Item(0, topLength-1, y, Physics::HIGHEST_Y, 0);

    //Make sure the y is reasonable
    if (incremented) this->object->Set_Current_Y(bottomY);
    return true;
}
