#include "Vertical_Limit_Checker.h"
#include "Object_Writer.h"
#include <QVector>
#include <assert.h>

bool Vertical_Limit_Checker::Does_Level_Exceed_Vertical_Object_Limit(Object_Writer *objects) {
    assert(objects);
    Text_Insertion_Buffer *objectsBuffer = objects->Get_Buffer();
    assert(objectsBuffer);
    QVector<int> objectsAtXCoordinates(objects->Get_Level_Length()+16, 0);

    //Scan the level for patterns that break the 3 vertical object limit rule
    int absoluteX = 0;
    objectsBuffer->Seek_To_Before_Beginning();
    while (!objectsBuffer->At_End()) {
        QString line = objectsBuffer->Get_Next_Line();
        int x = 0, y = 0, length = 0; QString attributes = QString(); Object_Item::Object_Item object = Object_Item::NOTHING;
        assert(this->Parse_Line(line, object, x, y, length, attributes));
        absoluteX += x;

        //Update lengths to reflect their horizontal value
        switch (object) {
        default:                                    break; //length value is fine
        case Object_Item::PAGE_CHANGE:              absoluteX = length*16; continue;
        case Object_Item::NOTHING:                  continue;
        case Object_Item::BOWSER_BRIDGE:            length = 13; break;
        case Object_Item::CANNON:                   length = 1; break;
        case Object_Item::CORRAL:                   length = 1; break;
        case Object_Item::END_STEPS:                length = 9; break;
        case Object_Item::PIPE:                     length = 2; break;
        case Object_Item::ENTERABLE_PIPE:           length = 2; break;
        case Object_Item::PIPE_WALL:                length = 4; break;
        case Object_Item::TALL_REVERSE_L_PIPE:      length = 4; break;
        case Object_Item::VERTICAL_BLOCKS:          length = 1; break;
        case Object_Item::VERTICAL_BRICKS:          length = 1; break;
        }

        //Mark the x coordinates as used
        if (length == 0) length = 1;
        for (int i = 0; i < length; ++i) {
            int tmp = objectsAtXCoordinates.at(absoluteX+i);
            if (tmp > 3) return true; //more than 3 objects detected!
            objectsAtXCoordinates[absoluteX+i] = tmp + 1;
        }
    }

    return false;
}
