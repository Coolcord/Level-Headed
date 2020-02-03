#include "../../Common_Files/Random.h"
#include "Item_Spawner.h"
#include "Object_Buffer.h"
#include <assert.h>

Item_Spawner::Item_Spawner(Object_Buffer *object, Level_Type::Level_Type levelType) {
    this->object = object;
    this->levelType = levelType;
}

int Item_Spawner::Spawn_Random_Item(int minX, int maxX, int groundLevelY, int minY, int requiredObjects, bool noBlocks) {
    assert(requiredObjects >= 0);
    assert(minX <= maxX);
    assert(groundLevelY >= minY);
    if (this->object->Get_Num_Objects_Available()-requiredObjects < 1) return 0; //nothing to do
    if (Random::Get_Instance().Get_Num(2) == 0) return 0; //don't spawn anything at this time
    int amountIncremented = 0;
    int lastObjectLength = (maxX-minX)+1;

    int x = 0;
    int y = 0;
    int random = Random::Get_Instance().Get_Num(2);
    y = groundLevelY - 4;
    if (!noBlocks && random == 0 && y >= minY) { //possibly spawn a powerup
        x = Random::Get_Instance().Get_Num(lastObjectLength-1);
        assert(this->object->Question_Block_With_Coin(x, y));
    } else if (random != 0) { //spawn coins
        int length = Random::Get_Instance().Get_Num(maxX-minX)+3; //try to spawn at least 3 coins
        if (minX+length > maxX) length = (maxX-minX)+1; //cut back if necessary
        x = Random::Get_Instance().Get_Num(maxX-(length-1))+minX;
        assert(x <= maxX);
        if (x < minX) x = minX;
        if (this->levelType == Level_Type::BRIDGE) y = groundLevelY-(Random::Get_Instance().Get_Num(2)+3);
        else y = groundLevelY-(Random::Get_Instance().Get_Num(4)+1);
        if (y < minY) y = minY;
        if (groundLevelY == 2 && y == 0) y = 1; //this works around a bug in the engine that can cause Mario to fall through the floor
        assert(this->object->Horizontal_Coins(x, y, length));
    }
    amountIncremented += x;

    assert(lastObjectLength-amountIncremented >= 0);
    this->object->Set_Last_Object_Length(lastObjectLength-amountIncremented);
    this->object->Set_Current_Y(groundLevelY);
    return amountIncremented;
}

