#include "../../Common_Files/Random.h"
#include "End_Spawner.h"
#include "Enemy_Writer.h"
#include "Required_Enemy_Spawns.h"
#include "Physics.h"
#include <assert.h>

End_Spawner::End_Spawner(Object_Writer *object, Enemy_Writer *enemy, SMB1_Compliance_Generator_Arguments *args, Required_Enemy_Spawns *requiredEnemySpawns) : Object_Spawner(object) {
    assert(object);
    assert(enemy);
    assert(args);
    assert(requiredEnemySpawns);
    this->object = object;
    this->enemy = enemy;
    this->args = args;
    this->requiredEnemySpawns = requiredEnemySpawns;
    this->endWritten = false;
    switch (args->endCastle) {
    case Castle::NONE:  this->castleObjectCount = 0; break;
    case Castle::SMALL: this->castleObjectCount = 1; break;
    case Castle::BIG:   this->castleObjectCount = 2; break;
    default: assert(false);
    }

    //Determine the amount of objects that need to be allocated for the end
    this->Determine_End();
    this->object->Set_End_Object_Count(this->endObjectCount);
}

bool End_Spawner::Is_End_Written() {
    return this->endWritten;
}

bool End_Spawner::Handle_End(int x) {
    return this->Handle_End(x, false);
}

bool End_Spawner::Handle_End(int x, bool forceWrite) {
    if (this->endWritten) return false; //can't write another end
    int numObjectsLeft = this->object->Get_Num_Objects_Left();
    assert(numObjectsLeft >= this->endObjectCount);
    assert(this->enemy->Get_Num_Bytes_Left() >= this->requiredEnemySpawns->Get_Num_End_Bytes());

    //Handle each end pattern accordingly
    if (forceWrite || numObjectsLeft == this->endObjectCount) {
        bool success = false;
        switch (this->endPattern) {
        case End_Pattern::Shortest:
            success =  this->Shortest_End(x); break;
        case End_Pattern::Shortest_With_Brick:
            success = this->Shortest_With_Brick_End(x); break;
        case End_Pattern::Shortest_Castle:
            success = this->Shortest_Castle(x); break;
        case End_Pattern::One_Block_Bridge:
            success = this->One_Block_Bridge_End(x); break;
        default:
            assert(false); return false;
        }
        if (success) this->endWritten = true;
        return success;
    }
    return true;
}

void End_Spawner::Determine_End() {
    switch (this->args->levelType) {
    case Level_Type::PIPE_EXIT:
    case Level_Type::STANDARD_OVERWORLD:
        assert(this->Determine_Standard_Overworld_End()); break;
    case Level_Type::UNDERGROUND:
        assert(this->Determine_Underground_End()); break;
    case Level_Type::UNDERWATER:
        assert(this->Determine_Underwater_End()); break;
    case Level_Type::CASTLE:
        assert(this->Determine_Castle_End()); break;
    case Level_Type::BRIDGE:
        assert(this->Determine_Bridge_End()); break;
    case Level_Type::ISLAND:
        assert(this->Determine_Island_End()); break;
    default:
        assert(false); return;
    }
    this->endObjectCount += this->castleObjectCount;
    this->object->Set_End_Object_Count(this->endObjectCount);
    assert(this->object->Get_Num_Bytes_Left() > this->endObjectCount);
}

bool End_Spawner::Determine_Standard_Overworld_End() {
    switch (0) {
    case 0:
        this->endPattern = End_Pattern::Shortest;
        this->endObjectCount = 4;
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Determine_Underground_End() {
    switch (0) {
    case 0:
        this->endPattern = End_Pattern::Shortest_With_Brick;
        this->endObjectCount = 5;
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Determine_Underwater_End() {
    switch (0) {
    case 0:
        this->endPattern = End_Pattern::Shortest;
        this->endObjectCount = 4;
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Determine_Castle_End() {
    switch (0) {
    case 0:
        this->endPattern = End_Pattern::Shortest_Castle;
        this->endObjectCount = 9;
        assert(this->requiredEnemySpawns->Set_Num_End_Bytes(6));
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Determine_Bridge_End() {
    switch (0) {
    case 0:
        this->endPattern = End_Pattern::One_Block_Bridge;
        this->endObjectCount = 9;
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Determine_Island_End() {
    switch (0) {
    case 0:
        this->endPattern = End_Pattern::Shortest_With_Brick;
        this->endObjectCount = 5;
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Shortest_End(int x) {
    return this->Shortest_End(x, true);
}

bool End_Spawner::Shortest_End(int x, bool cancelSpawner) {
    //Minimum 3 objects with a standard castle
    if (this->object->Get_Num_Objects_Left() < Physics::MIN_END_OBJECTS) return false;

    //Spawn the end of the level
    int absoluteX = this->object->Get_Absolute_X(x);

    //Handle the problem case of spawning at the edge of a page
    if (absoluteX == 0xF) {
        if (x < 0x10) ++x;
        else if (x < this->object->Get_Last_Object_Length()+1) ++x;
        else --x;
    }
    absoluteX = this->object->Get_Absolute_X(x);
    assert(absoluteX != 0xF);

    //Write the End Pattern
    this->object->Set_Coordinate_Safety(false); //turn off the safety check, since absolue value is confirmed first
    if (cancelSpawner) {
        if (!this->object->Cancel_Spawner(x)) return false;
        if (!this->object->End_Steps(0)) return false;
    } else {
        if (!this->object->End_Steps(x)) return false;
    }
    if (!this->object->Flagpole(Physics::END_STAIRS_LENGTH+Physics::FLAGPOLE_DISTANCE)) return false;
    if (!this->Spawn_Castle()) return false;

    //Handle the Scroll Stop
    x = 0x09;
    if (this->object->Get_Absolute_X(x) == 0xF) --x;
    if (!this->object->Scroll_Stop(x, false)) return false;
    this->object->Set_Coordinate_Safety(true); //turn back on the safety

    return true;
}

bool End_Spawner::Shortest_With_Brick_End(int x) {
    //Minimum 4 objects with a standard castle
    if (this->object->Get_Num_Objects_Left() < Physics::MIN_END_OBJECTS+1) return false;

    //Change to the surface brick pattern
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE, this->args->headerScenery));

    x = Random::Get_Num(6)+2;
    return this->Shortest_End(x);
}

bool End_Spawner::Shortest_Castle(int x) {
    if (this->object->Get_Num_Objects_Left() < 9) return false;
    if (this->enemy->Get_Num_Bytes_Left()-this->requiredEnemySpawns->Get_Num_Required_Bytes() < 0) return false;
    assert(this->requiredEnemySpawns->Set_Num_End_Bytes(0));
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE_4_AND_CEILING, Scenery::NO_SCENERY));

    //Ensure that Bowser lands on an EVEN page and Toad lands on an ODD page!
    x = 0xF - this->object->Get_Absolute_X(0);
    if (this->object->Get_Current_Page()%2 == 0) { //page is even
        x += 0x10; //increment to an odd numbered page
        this->object->Set_Coordinate_Safety(false);
    }
    assert(this->object->Change_Brick_And_Scenery(x, Brick::CEILING, Scenery::NO_SCENERY));
    this->object->Set_Coordinate_Safety(true);

    //Create the Bowser Bridge page
    assert(this->object->Bowser_Bridge(1));
    assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::BOWSER, 7));
    assert(this->object->Change_Brick_And_Scenery(12, Brick::SURFACE_5_AND_CEILING, Scenery::NO_SCENERY));
    assert(this->object->Axe_Rope(0));
    assert(this->object->Change_Brick_And_Scenery(1, Brick::SURFACE_5_AND_CEILING_4, Scenery::NO_SCENERY));
    assert(this->object->Axe(0));
    assert(this->object->Change_Brick_And_Scenery(2, Brick::SURFACE_AND_CEILING, Scenery::NO_SCENERY));
    assert(this->object->Scroll_Stop(6, false));
    assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::TOAD, 4));
    return true;
}

bool End_Spawner::One_Block_Bridge_End(int x) {
    if (this->object->Get_Num_Objects_Left() < 8) {
        return false;
    }

    //Spawn the ending bridge
    int y = Physics::GROUND_Y;
    int height = 0xD - y;
    assert(height == 3);
    int bridgeLength = Random::Get_Num(5)+5;
    assert(this->object->Vertical_Blocks(x, y, height));
    assert(this->object->Bridge(1, y, bridgeLength));
    int distanceRemainingFromBridgeEnd = bridgeLength;

    //Spawn an Island
    int islandLength = Random::Get_Num(11)+3;
    if (islandLength < 5) x = this->object->Get_Last_Object_Length();
    else x = this->object->Get_Last_Object_Length()-Random::Get_Num(islandLength-5);
    if (x < 0) x = 0; //TODO: Fix the else statement above to remove this line
    assert(this->object->Island(x, Physics::GROUND_Y+1, islandLength));
    distanceRemainingFromBridgeEnd -= x;
    int distanceRemainingFromIslandEnd = islandLength;

    //Spawn steps down from the bridge
    x = distanceRemainingFromBridgeEnd; //increment to the end of the bridge
    height = (Physics::GROUND_Y+1) - y;
    assert(height > 0);
    while (height > 0) {
        assert(this->object->Vertical_Blocks(x, y, height));
        distanceRemainingFromIslandEnd -= x;
        x = 1;
        ++y;
        --height;
    }
    if (this->object->Get_Absolute_X(0) == 0xF) assert(this->object->Cancel_Spawner(1));
    else assert(this->object->Cancel_Spawner(0));

    //Change the brick type back to surface
    x = Random::Get_Num(4)+distanceRemainingFromIslandEnd; //increment to the end of the island
    if (x > 0x10) x = 0x10;
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE, this->args->headerScenery));

    x = Random::Get_Num(10)+2;
    return this->Shortest_End(x, false);
}

bool End_Spawner::Spawn_Castle() {
    switch (this->castleObjectCount) {
    case 0:
        return true; //nothing to do
    case 1:
        return this->object->Castle(Physics::CASTLE_DISTANCE);
    case 2:
        if (!this->object->Change_Background(2, Background::CASTLE_WALL)) return false;
        return this->object->Big_Castle(3);
    default:
        assert(false);
        return false;
    }
}
