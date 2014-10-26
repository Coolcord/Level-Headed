#include "Header_Writer.h"
#include <assert.h>
#include "../Common SMB1 Files/Level_Type_String.h"
#include "../Common SMB1 Files/Header_String.h"
#include "../Common SMB1 Files/Level_Attribute_String.h"
#include "../Common SMB1 Files/Level_Compliment_String.h"
#include "../Common SMB1 Files/Background_String.h"
#include "../Common SMB1 Files/Scenery_String.h"
#include "../Common SMB1 Files/Brick_String.h"
#include <QDebug>

Header_Writer::Header_Writer(QFile *file) {
    assert(file);
    this->file = file;
    this->written = false;
}

bool Header_Writer::Write_Header(Level_Type::Level_Type type, Level_Attribute::Level_Attribute attribute, Brick::Brick brick, Background::Background background, Scenery::Scenery scenery, Level_Compliment::Level_Compliment compliment,
                  int time, int halfwayPoint, int levelLength, int numObjects, int numEnemies, int numPipePointers) {
    if (this->written) return false; //cannot write the header twice

    //Prepare the buffer
    QString buffer = "";
    buffer += Header::STRING_NAME + "\n";

    //Handle the Level Type
    buffer += Header::STRING_TYPE + ": ";
    switch (type) {
    case Level_Type::STANDARD_OVERWORLD:    buffer += Level_Type::STRING_STANDARD_OVERWORLD + "\n"; break;
    case Level_Type::ISLAND:                buffer += Level_Type::STRING_ISLAND + "\n"; break;
    default:                                return false;
    }

    //Handle the Level Attribute
    buffer += Header::STRING_ATTRIBUTE + ": ";
    switch (attribute) {
    case Level_Attribute::UNDERWATER:   buffer += Level_Attribute::STRING_UNDERWATER + "\n"; break;
    case Level_Attribute::OVERWORLD:    buffer += Level_Attribute::STRING_OVERWORLD + "\n"; break;
    case Level_Attribute::UNDERGROUND:  buffer += Level_Attribute::STRING_UNDERGROUND + "\n"; break;
    case Level_Attribute::CASTLE:       buffer += Level_Attribute::STRING_CASTLE + "\n"; break;
    default:                            return false;
    }

    //Handle the Brick
    buffer += Header::STRING_BRICK + ": ";
    switch (brick) {
    case Brick::NO_BRICKS:                          buffer += Brick::STRING_NO_BRICKS + "\n"; break;
    case Brick::SURFACE:                            buffer += Brick::STRING_SURFACE + "\n"; break;
    case Brick::SURFACE_AND_CEILING:                buffer += Brick::STRING_SURFACE_AND_CEILING + "\n"; break;
    case Brick::SURFACE_AND_CEILING_3:              buffer += Brick::STRING_SURFACE_AND_CEILING_3 + "\n"; break;
    case Brick::SURFACE_AND_CEILING_4:              buffer += Brick::STRING_SURFACE_AND_CEILING_4 + "\n"; break;
    case Brick::SURFACE_AND_CEILING_8:              buffer += Brick::STRING_SURFACE_AND_CEILING_8 + "\n"; break;
    case Brick::SURFACE_4_AND_CEILING:              buffer += Brick::STRING_SURFACE_4_AND_CEILING + "\n"; break;
    case Brick::SURFACE_4_AND_CEILING_3:            buffer += Brick::STRING_SURFACE_4_AND_CEILING_3 + "\n"; break;
    case Brick::SURFACE_4_AND_CEILING_4:            buffer += Brick::STRING_SURFACE_4_AND_CEILING_4 + "\n"; break;
    case Brick::SURFACE_5_AND_CEILING:              buffer += Brick::STRING_SURFACE_5_AND_CEILING + "\n"; break;
    case Brick::CEILING:                            buffer += Brick::STRING_CEILING + "\n"; break;
    case Brick::SURFACE_5_AND_CEILING_4:            buffer += Brick::STRING_SURFACE_5_AND_CEILING_4 + "\n"; break;
    case Brick::SURFACE_8_AND_CEILING:              buffer += Brick::STRING_SURFACE_8_AND_CEILING + "\n"; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:   buffer += Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_5 + "\n"; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:   buffer += Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_4 + "\n"; break;
    case Brick::ALL:                                buffer += Brick::STRING_ALL + "\n"; break;
    default:                                        return false;
    }

    //Handle the Background
    buffer += Header::STRING_BACKGROUND + ": ";
    switch (background) {
    case Background::BLANK_BACKGROUND:  buffer += Background::STRING_BLANK_BACKGROUND + "\n"; break;
    case Background::IN_WATER:          buffer += Background::STRING_IN_WATER + "\n"; break;
    case Background::CASTLE_WALL:       buffer += Background::STRING_CASTLE_WALL + "\n"; break;
    case Background::OVER_WATER:        buffer += Background::STRING_OVER_WATER + "\n"; break;
    case Background::NIGHT:             buffer += Background::STRING_NIGHT + "\n"; break;
    case Background::SNOW:              buffer += Background::STRING_SNOW + "\n"; break;
    case Background::NIGHT_AND_SNOW:    buffer += Background::STRING_NIGHT_AND_SNOW + "\n"; break;
    case Background::NIGHT_AND_FREEZE:  buffer += Background::STRING_NIGHT_AND_FREEZE + "\n"; break;
    default:                            return false;
    }

    //Handle the Scenery
    buffer += Header::STRING_SCENERY + ": ";
    switch (scenery) {
    case Scenery::NO_SCENERY:   buffer += Scenery::STRING_NO_SCENERY + "\n"; break;
    case Scenery::ONLY_CLOUDS:  buffer += Scenery::STRING_ONLY_CLOUDS + "\n"; break;
    case Scenery::MOUNTAINS:    buffer += Scenery::STRING_MOUNTAINS + "\n"; break;
    case Scenery::FENCES:       buffer += Scenery::STRING_FENCES + "\n"; break;
    default:                    return false;
    }

    //Handle the Level Compliment
    buffer += Header::STRING_COMPLIMENT + ": ";
    switch (compliment) {
    case Level_Compliment::TREES:               buffer += Level_Compliment::STRING_TREES + "\n"; break;
    case Level_Compliment::MUSHROOMS:           buffer += Level_Compliment::STRING_MUSHROOMS + "\n"; break;
    case Level_Compliment::BULLET_BILL_TURRETS: buffer += Level_Compliment::STRING_BULLET_BILL_TURRETS + "\n"; break;
    case Level_Compliment::CLOUDS:              buffer += Level_Compliment::STRING_CLOUDS + "\n"; break;
    default:                                    return false;
    }

    //Write the rest of the header to the buffer
    buffer += Header::STRING_TIME + ": " + QString::number(time) + "\n";
    buffer += Header::STRING_MIDPOINT + ": " + QString::number(halfwayPoint) + "\n";
    buffer += Header::STRING_LEVEL_LENGTH + ": " + QString::number(levelLength) + "\n";
    buffer += Header::STRING_NUMBER_OF_OBJECTS + ": " + QString::number(numObjects) + "\n";
    buffer += Header::STRING_NUMBER_OF_ENEMIES + ": " + QString::number(numEnemies) + "\n";
    buffer += Header::STRING_NUMBER_OF_PIPE_POINTERS + ": " + QString::number(numPipePointers) + "\n";
    buffer += Level_Type::STRING_BREAK + "\n";

    //Append the current file's contents to the buffer
    if (!this->file->seek(0)) return false;
    buffer += QString(this->file->readAll());

    //Write the new file
    if (!this->file->seek(0)) return false;
    if (this->file->write(buffer.toUtf8().data()) == -1) return false;

    this->written = true;
    return true;
}
