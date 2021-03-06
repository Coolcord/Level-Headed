#include "Bridge_Generator.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "Continuous_Enemies_Spawner.h"
#include "Physics.h"
#include "Item_Spawner.h"
#include "Difficulty.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

Bridge_Generator::Bridge_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {
    this->itemSpawner = new Item_Spawner(this->objects, Level_Type::BRIDGE);
}

Bridge_Generator::~Bridge_Generator() {
    delete this->itemSpawner;
}

bool Bridge_Generator::Generate_Level() {
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

        //TODO: Clean up probabilities
        switch (Random::Get_Instance().Get_Num(19)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:     if (!this->Spawn_Simple_Bridge(x)) this->Spawn_Lone_Bridge(x);
                    break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:    if (!this->Spawn_Multi_Bridge(x)) this->Spawn_Lone_Bridge(x);
                    break;
        case 16:
        case 17:
        case 18:    this->Spawn_Lone_Bridge(x); break;
        case 19:    if (!this->Spawn_Lone_Bridge_Series(x)) this->Spawn_Lone_Bridge(x); break;
        default:    assert(false); return false;
        }

        assert(this->objects->Get_Num_Objects_Available() >= 0);
        assert(this->end->Handle_End(this->Get_Safe_Jump_Distance(this->objects->Get_Last_Object_Length())));
    }

    //Handle Additional Passes
    assert(this->powerupDistributor->Distribute_Powerups());
    assert(this->enemySpawner->Spawn_Enemies());

    //Write the header last
    if (!this->header->Write_Header_To_Buffer(Level_Type::BRIDGE, this->levelCrawler->Get_Level_Attribute(), this->levelCrawler->Get_Starting_Brick(), this->firstPageHandler->Get_Header_Background(), this->args->headerScenery, this->args->levelCompliment, 400,
                                      this->midpointHandler->Get_Midpoint(), this->args->difficulty, this->objects->Get_Level_Length(),
                                      this->objects->Get_Num_Items(), this->enemies->Get_Num_Items(), 0)) return false;
    return this->Write_Buffers_To_File();
}

int Bridge_Generator::Get_Y_From_Height(int height) {
    return (0xD-height);
}

int Bridge_Generator::Get_Height_From_Y(int y) {
    return this->Get_Y_From_Height(y);
}

int Bridge_Generator::Get_Bridge_Y() {
    switch (Random::Get_Instance().Get_Num(2)) {
    case 0:     return 7;
    case 1:     return 8;
    case 2:     return Physics::GROUND_Y;
    default:    assert(false); return false;
    }
}

int Bridge_Generator::Get_Bridge_Length() {
    int length = 3+(Random::Get_Instance().Get_Num(11));
    assert(length <= 0xF);
    return length;
}

int Bridge_Generator::Get_Safe_Jump_Distance(int min) {
    int x = Random::Get_Instance().Get_Num(3)+2;
    x += min;
    if (x > 0x10) x = 0x10;
    return x;
}

bool Bridge_Generator::Spawn_Intro(int &x) {
    //Decrement x a bit to match SMB1's style
    if (this->objects->Get_First_Page_Safety()) x -= Random::Get_Instance().Get_Num(8);

    //Handle Auto Scrolling Levels
    int autoScrollX = 2;
    this->Handle_Auto_Scroll_Start(autoScrollX);
    if (this->objects->Is_Auto_Scroll_Active()) x -= autoScrollX;
    assert(x >= 0);

    //Possibly spawn a hole between the steps and the castle
    assert(this->objects->Change_Brick_And_Scenery(x, Brick::NO_BRICKS, Scenery::ONLY_CLOUDS));
    x = Random::Get_Instance().Get_Num(Physics::WALKING_JUMP_LENGTH)+1;
    int length = Random::Get_Instance().Get_Num(7)+3;
    assert(this->objects->Island(x, Physics::GROUND_Y+1, length));

    //Determine the bridge placement
    int y = 0;
    int height = 0;
    switch (Random::Get_Instance().Get_Num(2)) {
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
        y = Physics::GROUND_Y;
    }
    assert(length >= height);
    x = Random::Get_Instance().Get_Num(length-height);
    assert(x < length);
    assert(height+x <= length);
    int numBlocks = Random::Get_Instance().Get_Num(2);
    //Prevent the blocks from being created over the edge of the island
    while (height+x+numBlocks > length) {
        --numBlocks;
    }
    //Don't extend the platform if there is only one step
    if (height == 1) numBlocks = 0;
    assert(this->objects->Steps(x, height));
    //Possibly extend the top of the steps if stairs were used
    if (height > 1) {
        for (int i = 0; i < numBlocks; ++i) {
            assert(this->objects->Vertical_Blocks(this->objects->Get_Last_Object_Length(), y, height));
        }
    }
    int bridgeLength = this->Get_Bridge_Length();
    if (bridgeLength <= (length-(x+height+numBlocks))+1) bridgeLength = (length-(x+height+numBlocks))+1+Random::Get_Instance().Get_Num(2);
    assert(this->objects->Bridge(this->objects->Get_Last_Object_Length(), y, bridgeLength));
    assert(this->objects->Vertical_Blocks(this->objects->Get_Last_Object_Length(), y, this->Get_Height_From_Y(y)));

    //Create the Continuous Enemies Spawner
    int spawnerX = 16-this->objects->Get_Level_Length();
    if (spawnerX < 0) spawnerX = 0;
    Enemy_Item::Enemy_Item spawner = this->continuousEnemiesSpawner->Create_Continuous_Enemies_Spawner(spawnerX);
    x += spawnerX;
    if (spawner == Enemy_Item::CHEEP_CHEEP_SPAWNER || spawner == Enemy_Item::BULLET_BILL_SPAWNER) x = 1;

    return true;
}

bool Bridge_Generator::Spawn_Simple_Bridge(int x, int y, bool ignoreFirstSupport) {
    //Requires at least 3 objects
    if (this->objects->Get_Num_Objects_Available() < 3) return false;

    //Determine a y value
    if (!y) y = this->Get_Bridge_Y();
    int height = this->Get_Height_From_Y(y);
    int length = this->Get_Bridge_Length();

    //Spawn the left support
    if (!ignoreFirstSupport) assert(this->objects->Vertical_Blocks(x, y, height));

    //Spawn the bridge itself
    assert(this->objects->Bridge(1, y, length));
    this->itemSpawner->Spawn_Random_Item(0, length-1, y, Physics::HIGHEST_Y, 1);

    //Spawn the right support
    assert(this->objects->Vertical_Blocks(this->objects->Get_Last_Object_Length(), y, height));

    return true;
}

bool Bridge_Generator::Spawn_Multi_Bridge(int x, int y, bool ignoreFirstSupport) {
    //Requires at least 5 objects
    int numObjectsAvailable = this->objects->Get_Num_Objects_Available();
    if (numObjectsAvailable < 5) return false;

    //A Multi Bridge can be anywhere from 2 to 4 bridges
    int numBridges = Random::Get_Instance().Get_Num(2)+2;
    int numObjectsRequired = 5+((numBridges-2)*2);
    if (numObjectsAvailable < numObjectsRequired) {
        numBridges = ((numObjectsAvailable-5)/2)+2;
        numObjectsRequired = numObjectsAvailable;
    }

    //Allow the Multi Bridge to be uniform in some aspects
    bool uniformDistance = !(static_cast<bool>(Random::Get_Instance().Get_Num(2)));
    bool uniformLength = !(static_cast<bool>(Random::Get_Instance().Get_Num(2)));
    bool uniformHeight = (static_cast<bool>(Random::Get_Instance().Get_Num(2)));
    if (!y) y = this->Get_Bridge_Y();
    int height = this->Get_Height_From_Y(y);
    int length = this->Get_Bridge_Length();

    //Spawn the Multi Bridge
    if (ignoreFirstSupport) {
        assert(this->objects->Bridge(x, y, length));
        --numObjectsRequired;
    } else {
        assert(this->objects->Vertical_Blocks(x, y, height));
        assert(this->objects->Bridge(1, y, length));
        numObjectsRequired -= 2;
    }
    this->itemSpawner->Spawn_Random_Item(0, length-1, y, Physics::HIGHEST_Y, numObjectsRequired);
    if (uniformDistance) x = this->Get_Safe_Jump_Distance(this->objects->Get_Last_Object_Length()+1);
    for (int i = 1; i < numBridges; ++i) {
        if (!uniformDistance) x = this->Get_Safe_Jump_Distance(this->objects->Get_Last_Object_Length()+1);
        if (!uniformLength) length = this->Get_Bridge_Length();
        if (!uniformHeight) {
            int previousY = y;
            int previousHeight = height;
            y = this->Get_Bridge_Y();
            height = this->Get_Height_From_Y(y);
            if (y > previousY) assert(this->objects->Vertical_Blocks(this->objects->Get_Last_Object_Length(), previousY, previousHeight));
            else assert(this->objects->Vertical_Blocks(this->objects->Get_Last_Object_Length(), y, height));
        } else {
            assert(this->objects->Vertical_Blocks(this->objects->Get_Last_Object_Length(), y, height));
        }
        assert(this->objects->Bridge(this->objects->Get_Last_Object_Length(), y, length));
        numObjectsRequired -= 2;
        this->itemSpawner->Spawn_Random_Item(0, length-1, y, Physics::HIGHEST_Y, numObjectsRequired);
    }
    assert(this->objects->Vertical_Blocks(this->objects->Get_Last_Object_Length(), y, height));
    return true;
}

bool Bridge_Generator::Spawn_Lone_Bridge(int x, int y, int length) {
    //Requires one object
    if (this->objects->Get_Num_Objects_Available() < 1) return false;

    //Determine a y value
    if (!y) y = this->Get_Bridge_Y();
    if (!length) length = Random::Get_Instance().Get_Num(2)+2;

    //Spawn the bridge by itself
    assert(this->objects->Bridge(x, y, length));
    return true;
}

bool Bridge_Generator::Spawn_Lone_Bridge_Series(int x) {
    //Requires at least 3 objects
    int numObjectsAvailable = this->objects->Get_Num_Objects_Available();
    if (numObjectsAvailable < 3) return false;

    //A series of lone bridges can consist of anywhere between 3 and 6 bridges
    int numBridges = Random::Get_Instance().Get_Num(2)+3;
    if (numBridges > numObjectsAvailable) numBridges = numObjectsAvailable;

    //Allow the series to be uniform in some aspects
    bool uniformDistance = !(static_cast<bool>(Random::Get_Instance().Get_Num(3)));
    bool uniformHeight = !(static_cast<bool>(Random::Get_Instance().Get_Num(3)));
    bool uniformLength = !(static_cast<bool>(Random::Get_Instance().Get_Num(3)));
    int y = this->Get_Bridge_Y();
    int length = Random::Get_Instance().Get_Num(2)+2;

    //Spawn the Lone Bridge Series
    assert(this->Spawn_Lone_Bridge(x, y, length));
    if (uniformDistance) x = this->Get_Safe_Jump_Distance(this->objects->Get_Last_Object_Length());
    for (int i = 1; i < numBridges; ++i) {
        if (!uniformDistance) x = this->Get_Safe_Jump_Distance(this->objects->Get_Last_Object_Length());
        if (!uniformHeight) y = this->Get_Bridge_Y();
        if (!uniformLength) length = Random::Get_Instance().Get_Num(2)+2;
        assert(this->Spawn_Lone_Bridge(x, y, length));
    }
    return true;
}

