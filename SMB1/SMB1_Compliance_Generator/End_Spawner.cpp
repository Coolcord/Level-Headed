#include "End_Spawner.h"
#include "Physics.h"
#include <assert.h>

End_Spawner::End_Spawner(Object_Writer *object, SMB1_Compliance_Generator_Arguments *args) : Object_Spawner(object) {
    assert(object);
    assert(args);
    this->object = object;
    this->args = args;
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
    if (this->endWritten) return false; //can't write another end
    int numObjectsLeft = this->object->Get_Num_Objects_Left();
    if (numObjectsLeft < this->endObjectCount) return false;

    //Handle each end pattern accordingly
    if (numObjectsLeft == this->endObjectCount) {
        bool success = false;
        switch (this->endPattern) {
        case End_Pattern::Shortest:
            success =  this->Shortest_End(x); break;
        case End_Pattern::Shortest_With_Brick:
            success = this->Shortest_With_Brick_End(x); break;
        case End_Pattern::One_Block_Bridge_Island:
            success = this->One_Block_Bridge_Island_End(x); break;
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
    switch (qrand()%1) {
    case 0:
        this->endPattern = End_Pattern::Shortest;
        this->endObjectCount = 2;
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Determine_Underground_End() {
    //TODO: Implement this...
    assert(false); return false;
}

bool End_Spawner::Determine_Underwater_End() {
    switch (qrand()%1) {
    case 0:
        this->endPattern = End_Pattern::Shortest;
        this->endObjectCount = 2;
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Determine_Castle_End() {
    //TODO: Implement this...
    assert(false); return false;
}

bool End_Spawner::Determine_Bridge_End() {
    switch (qrand()%1) {
    case 0:
        this->endPattern = End_Pattern::One_Block_Bridge_Island;
        this->endObjectCount = 8;
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Determine_Island_End() {
    switch (qrand()%1) {
    case 0:
        this->endPattern = End_Pattern::Shortest_With_Brick;
        this->endObjectCount = 3;
        return true;
    default:
        assert(false);
        return false;
    }
}

bool End_Spawner::Shortest_End(int x) {
    //Minimum 3 objects with a standard castle
    if (this->object->Get_Num_Objects_Left() < Physics::MIN_END_OBJECTS) return false;

    //Spawn the end of the level
    int absoluteX = this->object->Get_Absolute_X(x);

    //Handle the problem case of spawning at the edge of a page
    if (absoluteX == 0xF) {
        if (x < 0x10) ++x;
        else if (x > this->object->Get_Last_Object_Length()+1) --x;
        else ++x;
    }

    //Write the End Pattern
    this->object->Set_Coordinate_Safety(false); //turn off the safety check, since absolue value is confirmed first
    if (!this->object->End_Steps(x)) return false;
    if (!this->object->Flagpole(Physics::END_STAIRS_LENGTH+Physics::FLAGPOLE_DISTANCE)) return false;
    if (!this->Spawn_Castle()) return false;
    this->object->Set_Coordinate_Safety(true); //turn back on the safety

    return true;
}

bool End_Spawner::Shortest_With_Brick_End(int x) {
    //Minimum 4 objects with a standard castle
    if (this->object->Get_Num_Objects_Left() < Physics::MIN_END_OBJECTS+1) return false;

    //Change to the surface brick pattern
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE, this->args->headerScenery));

    x = (qrand()%7)+2;
    return this->Shortest_End(x);
}

bool End_Spawner::One_Block_Bridge_Island_End(int x) {
    if (this->object->Get_Num_Objects_Left() < 8) return false;

    //Spawn the ending bridge
    int y = Physics::GROUND_Y;
    int height = 0xD - y;
    int bridgeLength = (qrand()%6)+5;
    assert(this->object->Vertical_Blocks(x, y, height));
    assert(this->object->Bridge(1, y, bridgeLength));
    int distanceRemainingFromBridgeEnd = bridgeLength;

    //Spawn an Island
    int islandLength = (qrand()%12)+3;
    if (islandLength < 5) x = this->object->Get_Last_Object_Length();
    else x = this->object->Get_Last_Object_Length()-(qrand()%(islandLength-4));
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
    x = (qrand()%5)+distanceRemainingFromIslandEnd; //increment to the end of the island
    if (x > 0x10) x = 0x10;
    assert(this->object->Change_Brick_And_Scenery(x, Brick::SURFACE, this->args->headerScenery));

    x = (qrand()%11)+2;
    return this->Shortest_End(x);
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
