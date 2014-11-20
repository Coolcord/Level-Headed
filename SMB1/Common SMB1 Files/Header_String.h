#ifndef HEADER_STRING_H
#define HEADER_STRING_H

#include "Level_Type_String.h"
#include <QString>

namespace Header {
    const static QString STRING_NAME = Level_Type::STRING_NAME + "_Level";
    const static QString STRING_MAP_NAME = Level_Type::STRING_NAME + "_Level_Map";
    const static QString STRING_TYPE = "Level Type";
    const static QString STRING_ATTRIBUTE = "Attribute";
    const static QString STRING_BRICK = "Brick_Pattern";
    const static QString STRING_BACKGROUND = "Background";
    const static QString STRING_SCENERY = "Scenery";
    const static QString STRING_COMPLIMENT = "Compliment";
    const static QString STRING_TIME = "Time";
    const static QString STRING_MIDPOINT = "Midpoint";
    const static QString STRING_COOLCORD = STRING_NAME + " format created by Coolcord.";
    const static QString STRING_CREATED = "Randomly Generated on";
    const static QString STRING_LEVEL_LENGTH = "Level Length";
    const static QString STRING_NUMBER_OF_OBJECTS = "Number of Objects";
    const static QString STRING_NUMBER_OF_ENEMIES = "Number of Enemies";
    const static QString STRING_NUMBER_OF_PIPE_POINTERS = "Number of Pipe Pointers";
    const static QString STRING_NUMBER_OF_WORLDS = "Number_of_Worlds";
    const static QString STRING_NUMBER_OF_LEVELS_PER_WORLD = "Number_of_Levels_per_World";
}

#endif // HEADER_STRING_H
