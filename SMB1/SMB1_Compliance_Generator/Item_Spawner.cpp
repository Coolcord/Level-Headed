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
    if (qrand()%3 != 0) return 0; //don't spawn anything at this time
    int amountIncremented = 0;
    int lastObjectLength = maxX-minX;

    //TODO: Add more than just coins, but remember that these values are set up specifically for coins
    int x = ((qrand()%(maxX+1)))-minX;
    if (x > maxX-3) x = maxX-3;
    if (x < minX) x = minX;
    int y = 0;
    if (this->levelType == Level_Type::BRIDGE) y = groundLevelY-((qrand()%3)+3);
    else y = groundLevelY-((qrand()%5)+1);
    if (y < minY) y = minY;
    int length = (qrand()%((maxX-x)+1))+3;
    if (x+length > maxX) length = maxX-x;
    assert(this->object->Horizontal_Coins(x, y, length));
    amountIncremented += x;

    assert(lastObjectLength-amountIncremented >= 0);
    this->object->Set_Last_Object_Length(lastObjectLength-amountIncremented);
    this->object->Set_Current_Y(groundLevelY);
    return amountIncremented;
}

