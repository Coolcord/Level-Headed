#ifndef HEADER_STRING_H
#define HEADER_STRING_H

#include "Level_Type_String.h"
#include <QString>

namespace Header {
    const static QString STRING_NAME = Level_Type::STRING_NAME + "_Level";
    const static QString STRING_TYPE = "Type";
    const static QString STRING_ATTRIBUTE = "Attribute";
    const static QString STRING_BRICK = "Brick";
    const static QString STRING_BACKGROUND = "Background";
    const static QString STRING_SCENERY = "Scenery";
    const static QString STRING_COMPLIMENT = "Compliment";
    const static QString STRING_TIME = "Time";
    const static QString STRING_MIDPOINT = "Midpoint";
    const static QString STRING_COOLCORD = STRING_NAME + " format created by Coolcord.";
    const static QString STRING_CREATED = "Randomly Generated on";
    const static QString STRING_LEVEL_LENGTH = "Level Length";
    const static QString STRING_NUMBER_OF_OBJECTS = "Number Of Objects";
    const static QString STRING_NUMBER_OF_ENEMIES = "Number Of Enemies";
    const static QString STRING_NUMBER_OF_PIPE_POINTERS = "Number Of Pipe Pointers";
}

#endif // HEADER_STRING_H
