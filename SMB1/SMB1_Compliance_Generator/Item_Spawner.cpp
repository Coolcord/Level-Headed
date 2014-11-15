#include "Item_Spawner.h"
#include "Object_Writer.h"
#include <assert.h>

Item_Spawner::Item_Spawner(Object_Writer *object, Level_Type::Level_Type levelType) {
    this->object = object;
    this->levelType = levelType;
}

int Item_Spawner::Spawn_Random_Item(int minX, int maxX, int groundLevelY, int minY, int requiredObjects) {
    assert(requiredObjects >= 0);
    assert(minX <= maxX);
    assert(groundLevelY >= minY);
    if (this->object->Get_Num_Objects_Available()-requiredObjects < 1) return 0; //nothing to do
    if (qrand()%3 == 0) return 0; //don't spawn anything at this time
    int amountIncremented = 0;
    int lastObjectLength = (maxX-minX)+1;

    int x = 0;
    int y = 0;
    int random = qrand()%3;
    y = groundLevelY - 4;
    if (random == 0 && y >= minY) { //possibly spawn a powerup
        x = qrand()%lastObjectLength;
        if (qrand()%2 == 0) { //spawn a mushroom
            assert(this->object->Question_Block_With_Mushroom(x, y));
        } else {
            assert(this->object->Question_Block_With_Coin(x, y));
        }
    } else { //spawn coins
        int length = (qrand()%((maxX-minX)+1))+3; //try to spawn at least 3 coins
        if (minX+length > maxX) length = (maxX-minX)+1; //cut back if necessary
        x = (qrand()%((maxX-(length-1))+1))+minX;
        assert(x <= maxX);
        if (x < minX) x = minX;
        if (this->levelType == Level_Type::BRIDGE) y = groundLevelY-((qrand()%3)+3);
        else y = groundLevelY-((qrand()%5)+1);
        if (y < minY) y = minY;
        assert(this->object->Horizontal_Coins(x, y, length));
    }
    amountIncremented += x;

    assert(lastObjectLength-amountIncremented >= 0);
    this->object->Set_Last_Object_Length(lastObjectLength-amountIncremented);
    this->object->Set_Current_Y(groundLevelY);
    return amountIncremented;
}

