#include "Bridge_Generator.h"
#include "Physics.h"
#include <QTime>
#include <assert.h>

bool Bridge_Generator::Generate_Level() {
    this->midpointHandler->Set_Level_Type(Level_Type::BRIDGE);

    int x = this->object->Get_Last_Object_Length();
    this->firstPageHandler->Handle_First_Page(x);
    this->Spawn_Intro(x);

    //Create the level
    while (!this->end->Is_End_Written()) {
        x = this->object->Get_Last_Object_Length();
        this->midpointHandler->Handle_Midpoint(x);
        x = this->Get_Safe_Jump_Distance(x);


        //TODO: Clean up this
        this->Spawn_Simple_Bridge(x);
        this->Spawn_Multi_Bridge(x);
        this->Spawn_Lone_Bridge(x);
        this->Spawn_Lone_Bridge_Series(x);


        assert(this->end->Handle_End(this->Get_Safe_Random_X()));
    }

    //Spawn the Enemies
    assert(this->enemySpawner->Spawn_Enemies(Brick::SURFACE, Level_Type::BRIDGE));

    //Write the header last
    return this->header->Write_Header(Level_Type::BRIDGE, Level_Attribute::OVERWORLD, Brick::SURFACE, Background::BLANK_BACKGROUND, Scenery::ONLY_CLOUDS, Level_Compliment::TREES, 400,
                                      this->midpointHandler->Get_Midpoint(), this->object->Get_Level_Length(),
                                      this->object->Get_Num_Items(), this->enemy->Get_Num_Items(), 0);
}

int Bridge_Generator::Get_Y_From_Height(int height) {
    return (0xD-height);
}

int Bridge_Generator::Get_Height_From_Y(int y) {
    return this->Get_Y_From_Height(y);
}

int Bridge_Generator::Get_Bridge_Y() {
    switch (qrand()%3) {
    case 0:     return 7;
    case 1:     return 8;
    case 2:     return Physics::GROUND_Y;
    default:    assert(false); return false;
    }
}

int Bridge_Generator::Get_Bridge_Length() {
    int length = this->Get_Random_X(1);
    if (length > 0xF) length = 0xF;
    return length;
}

int Bridge_Generator::Get_Safe_Jump_Distance(int min) {
    //Aim for a lower value... but allow higher values to be possible
    int x = 0;
    switch (qrand()%3) {
    case 0:     x = (qrand()%0x5)+1; break;
    case 1:     x = (qrand()%0x6)+1; break;
    case 2:     x = (qrand()%0x7)+1; break;
    case 3:     x = (qrand()%Physics::RUNNING_JUMP_LENGTH)+1; break;
    default:    assert(false); return 0;
    }
    x += min;
    if (x > Physics::RUNNING_JUMP_LENGTH) x = Physics::RUNNING_JUMP_LENGTH;
    return x;
}

int Bridge_Generator::Trim_Distance(int value, int amount) {
    value -= amount;
    if (value < 1) value = 1;
    return value;
}

bool Bridge_Generator::Spawn_Intro(int &x) {
    //Decrement x a bit to match SMB1's style
    if (this->object->Get_First_Page_Safety()) x -= (qrand()%9);

    //Possibly spawn a hole between the steps and the castle
    assert(this->object->Change_Brick_And_Scenery(x, Brick::NO_BRICKS, Scenery::ONLY_CLOUDS));
    x = (qrand()%Physics::WALKING_JUMP_LENGTH)+1;
    int length = (qrand()%8)+3;
    assert(this->object->Island(x, Physics::GROUND_Y+1, length));

    //Determine the bridge placement
    int y = 0;
    int height = 0;
    switch (qrand()%3) {
    case 0:
        y = 7;
        height = 4;
        break;
    case 1:
        y = 8;
        height = 3;
        break;
    case 2:
        y = Physics::GROUND_Y;
        height = 1;
        break;
    default:
        assert(false);
        return false;
    }

    //Spawn the Steps and Bridge
    if (length == 3) {
        height = 1;
        y = Physics::GROUND_Y+1;
    }
    assert(length >= height);
    x = (qrand()%(length-(height-1)));
    assert(x < length);
    //Reduce the number of used objects if possible
    if (height == 1) assert(this->object->Horizontal_Blocks(x, y, (qrand()%3)+1));
    else assert(this->object->Steps(x, height));
    int numBlocks = (qrand()%3);
    //Possibly extend the top of the steps if stairs were used
    if (height > 1) {
        for (int i = 0; i < numBlocks; ++i) {
            assert(this->object->Vertical_Blocks(this->object->Get_Last_Object_Length(), y, height));
        }
    }
    assert(this->object->Bridge(this->object->Get_Last_Object_Length(), y, this->Get_Bridge_Length()));
    assert(this->object->Vertical_Blocks(this->object->Get_Last_Object_Length(), y, this->Get_Height_From_Y(y)));

    return true;
}

bool Bridge_Generator::Spawn_Simple_Bridge(int x, int y, bool ignoreFirstSupport) {
    //Requires at least 3 objects
    if (this->object->Get_Num_Objects_Available() < 3) return false;

    //Determine a y value
    if (!y) y = this->Get_Bridge_Y();
    int height = this->Get_Height_From_Y(y);
    int length = this->Get_Bridge_Length();

    //Spawn the left support
    if (!ignoreFirstSupport) assert(this->object->Vertical_Blocks(x, y, height));

    //Spawn the bridge itself
    assert(this->object->Bridge(1, y, length));

    //Spawn the right support
    assert(this->object->Vertical_Blocks(length, y, height));

    return true;
}

bool Bridge_Generator::Spawn_Multi_Bridge(int x, int y, bool ignoreFirstSupport) {
    //Requires at least 5 objects
    int numObjectsAvailable = this->object->Get_Num_Objects_Available();
    if (numObjectsAvailable < 5) return false;

    //A Multi Bridge can be anywhere from 2 to 4 bridges
    int numBridges = (qrand()%3)+2;
    int numObjectsRequired = 5+((numBridges-2)*2);
    if (numObjectsAvailable < numObjectsRequired) {
        numBridges = ((numObjectsAvailable-5)/2)+2;
    }

    //Allow the Multi Bridge to be uniform in some aspects
    bool uniformDistance = !(static_cast<bool>(qrand()%4));
    bool uniformHeight = !(static_cast<bool>(qrand()%4));
    bool uniformLength = !(static_cast<bool>(qrand()%4));
    if (!y) y = this->Get_Bridge_Y();
    int height = this->Get_Height_From_Y(y);
    int length = this->Get_Bridge_Length();

    //Spawn the Multi Bridge
    if (ignoreFirstSupport) {
        assert(this->object->Bridge(x, y, length));
    } else {
        assert(this->object->Vertical_Blocks(x, y, height));
        assert(this->object->Bridge(1, y, length));
    }
    if (uniformDistance) x = this->Trim_Distance(this->Get_Safe_Jump_Distance(), 2);
    for (int i = 1; i < numBridges; ++i) {
        if (!uniformDistance) x = this->Trim_Distance(this->Get_Safe_Jump_Distance(), 1);
        if (!uniformLength) length = this->Get_Bridge_Length();
        if (!uniformHeight) {
            y = this->Get_Bridge_Y();
            height = this->Get_Height_From_Y(y);
        }
        assert(this->object->Vertical_Blocks(x, y, height));
        assert(this->object->Bridge(1, y, length));
    }
    return true;
}

bool Bridge_Generator::Spawn_Lone_Bridge(int x, int y, int length) {
    //Requires one object
    if (this->object->Get_Num_Objects_Available() < 1) return false;

    //Determine a y value
    if (!y) y = this->Get_Bridge_Y();
    if (!length) length = (qrand()%3)+2;

    //Spawn the bridge by itself
    assert(this->object->Bridge(x, y, length));
    return true;
}

bool Bridge_Generator::Spawn_Lone_Bridge_Series(int x) {
    //Requires at least 3 objects
    int numObjectsAvailable = this->object->Get_Num_Objects_Available();
    if (numObjectsAvailable < 3) return false;

    //A series of lone bridges can consist of anywhere between 3 and 6 bridges
    int numBridges = (qrand()%3)+3;
    if (numObjectsAvailable < numBridges) numBridges = numObjectsAvailable;

    //Allow the series to be uniform in some aspects
    bool uniformDistance = !(static_cast<bool>(qrand()%4));
    bool uniformHeight = !(static_cast<bool>(qrand()%4));
    bool uniformLength = !(static_cast<bool>(qrand()%4));
    int y = this->Get_Bridge_Y();
    int length = this->Get_Bridge_Length();

    //Spawn the Lone Bridge Series
    assert(this->Spawn_Lone_Bridge(x, y, length));
    if (uniformDistance) x = this->Trim_Distance(this->Get_Safe_Jump_Distance(), 2);
    for (int i = 1; i < numBridges; ++i) {
        if (!uniformDistance) x = this->Trim_Distance(this->Get_Safe_Jump_Distance(), 1);
        if (!uniformHeight) y = this->Get_Bridge_Y();
        if (!uniformLength) length = this->Get_Bridge_Length();
        assert(this->Spawn_Lone_Bridge(x, y, length));
    }
    return true;
}


