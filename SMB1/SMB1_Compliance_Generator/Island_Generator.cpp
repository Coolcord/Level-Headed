#include "Island_Generator.h"
#include "Physics.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

bool Island_Generator::Generate_Level() {
    int x = this->object->Get_Last_Object_Length();
    this->firstPageHandler->Handle_First_Page(x);
    assert(this->Spawn_Intro(x));

    //Create the level
    while (!this->end->Is_End_Written()) {
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Safe_Jump_Distance(x);

        //TODO: Add support for spawning coins and item boxes above islands
        if (this->object->Get_Num_Objects_Available() >= 2 && qrand()%5 == 0) assert(this->Spawn_Two_Islands(x));
        else assert(this->Spawn_Basic_Island(x));

        assert(this->end->Handle_End(this->Get_Safe_Jump_Distance(this->object->Get_Last_Object_Length())));
        x = this->object->Get_Last_Object_Length();
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE));

    //Write the header last
    return this->header->Write_Header(Level_Type::ISLAND, Level_Attribute::OVERWORLD, Brick::SURFACE, Background::BLANK_BACKGROUND, Scenery::ONLY_CLOUDS, Level_Compliment::TREES, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}

bool Island_Generator::Spawn_Intro(int &x) {
    //Decrement x a bit to match SMB1's style
    if (this->object->Get_First_Page_Safety()) x -= (qrand()%5);
    assert(this->object->Change_Brick_And_Scenery(x, Brick::NO_BRICKS, Scenery::ONLY_CLOUDS));
    return true;
}

int Island_Generator::Get_Island_Length(int min) {
    //Prefer lower numbers
    int length = 0;
    switch (qrand()%4) {
    case 0:     length = (qrand()%2)+3; break;
    case 1:     length = (qrand()%4)+3; break;
    case 2:     length = (qrand()%6)+3; break;
    case 3:     length = (qrand()%8)+3; break;
    default:    assert(false); return 0;
    }
    if (length < min) length = min;
    return length;
}

int Island_Generator::Get_Safe_Jump_Distance(int min) {
    int x = (qrand()%0x4)+2;
    x += min;
    if (x > 0x10) x = 0x10;
    return x;
}

int Island_Generator::Get_Island_Y() {
    int y = this->object->Get_Current_Y();

    //Determine whether to go up or down
    if (qrand()%2 == 0) { //go up
        y -= (qrand()%5);
    } else { //go down
        y += (qrand()%5);
    }
    //Y values should be between the range of 1 at the highest and 11 at the lowest
    if (y > Physics::GROUND_Y+1) y = Physics::GROUND_Y+1;
    if (y < 1) y = 1;

    return y;
}

bool Island_Generator::Spawn_Basic_Island(int x) {
    if (this->object->Get_Num_Objects_Available() < 1) return false;
    assert(this->object->Island(x, this->Get_Island_Y(), this->Get_Island_Length()));
    return true;
}

bool Island_Generator::Spawn_Two_Islands(int x) {
    //Determine lowest y
    int y = this->object->Get_Current_Y();

    //The bottom island should spawn between 5 at the highest and 11 at the lowest
    if (y-4 > 5) { //at these heights, the last y value is relevant
        y -= (qrand()%5);
    } else { //y is irrelevant, so allow for more freedom
        y = (qrand()%7)+5;
    }
    int bottomLength = this->Get_Island_Length(5);
    assert(this->object->Island(x, y, bottomLength));

    //The top island should spawn 3 - 4 blocks above the bottom island (prefer 4)
    assert(y > 4);
    if (qrand()%4 > 0) y -= 4;
    else y -= 3;
    if (y < 1) y = 1;


    //The top island should be shorter than the bottom island
    int topLength = (qrand()%(bottomLength-2))+3;
    if (topLength == bottomLength) --topLength;
    assert(bottomLength > topLength);

    //X should be incremented by at least 1
    x = (qrand()%(bottomLength-topLength))+1;
    assert(this->object->Island(x, y, topLength));

    //Fix the last object length
    assert(x+topLength <= bottomLength);
    if (x+topLength < bottomLength) {
        this->object->Increment_Last_Object_Length(bottomLength-(x+topLength));
    }
    return true;
}
