#include "Header_Writer.h"
#include <assert.h>
#include "../../Level-Headed/Common_Strings.h"
#include "../Common_SMB1_Files/Level_Type_String.h"
#include "../Common_SMB1_Files/Header_String.h"
#include "../Common_SMB1_Files/Level_Attribute_String.h"
#include "../Common_SMB1_Files/Level_Compliment_String.h"
#include "../Common_SMB1_Files/Background_String.h"
#include "../Common_SMB1_Files/Scenery_String.h"
#include "../Common_SMB1_Files/Brick_String.h"
#include <QTime>
#include <QDate>
#include <QDebug>

Header_Writer::Header_Writer(QFile *file) {
    assert(file);
    this->file = file;
    this->written = false;
}

bool Header_Writer::Write_Header(Level_Type::Level_Type type, Level_Attribute::Level_Attribute attribute, Brick::Brick brick, Background::Background background, Scenery::Scenery scenery, Level_Compliment::Level_Compliment compliment,
                  int time, int halfwayPoint, int difficulty, int levelLength, int numObjects, int numEnemies, int numPipePointers) {
    if (this->written) return false; //cannot write the header twice

    //Prepare the buffer
    QString buffer = "";
    buffer += Header::STRING_NAME + Common_Strings::STRING_NEW_LINE;

    //Add the notes section
    buffer += Level_Type::STRING_BREAK + Common_Strings::STRING_NEW_LINE;
    buffer += Header::STRING_COOLCORD + Common_Strings::STRING_NEW_LINE;
    buffer += Header::STRING_CREATED + " " + QDate::currentDate().toString("dddd, MMMM dd, yyyy") + ", at " + QTime::currentTime().toString("hh:mm:ss A") + "." + Common_Strings::STRING_NEW_LINE;
    //Handle the Level Type
    buffer += Header::STRING_TYPE + ": ";
    switch (type) {
    case Level_Type::STANDARD_OVERWORLD:    buffer += Level_Type::STRING_STANDARD_OVERWORLD + Common_Strings::STRING_NEW_LINE; break;
    case Level_Type::ISLAND:                buffer += Level_Type::STRING_ISLAND + Common_Strings::STRING_NEW_LINE; break;
    case Level_Type::BRIDGE:                buffer += Level_Type::STRING_BRIDGE + Common_Strings::STRING_NEW_LINE; break;
    case Level_Type::UNDERGROUND:           buffer += Level_Type::STRING_UNDERGROUND + Common_Strings::STRING_NEW_LINE; break;
    case Level_Type::UNDERWATER:            buffer += Level_Type::STRING_UNDERWATER + Common_Strings::STRING_NEW_LINE; break;
    case Level_Type::CASTLE:                buffer += Level_Type::STRING_CASTLE + Common_Strings::STRING_NEW_LINE; break;
    default:                                return false;
    }
    buffer += Header::STRING_DIFFICULTY + ": " + QString::number(difficulty) + Common_Strings::STRING_NEW_LINE;
    buffer += Header::STRING_LEVEL_LENGTH + ": " + QString::number(levelLength) + Common_Strings::STRING_NEW_LINE;
    buffer += Header::STRING_NUMBER_OF_OBJECTS + ": " + QString::number(numObjects) + Common_Strings::STRING_NEW_LINE;
    buffer += Header::STRING_NUMBER_OF_ENEMIES + ": " + QString::number(numEnemies) + Common_Strings::STRING_NEW_LINE;
    buffer += Header::STRING_NUMBER_OF_PIPE_POINTERS + ": " + QString::number(numPipePointers) + Common_Strings::STRING_NEW_LINE;
    buffer += Level_Type::STRING_BREAK + Common_Strings::STRING_NEW_LINE;

    //Handle the Level Attribute
    buffer += Header::STRING_ATTRIBUTE + ": ";
    switch (attribute) {
    case Level_Attribute::UNDERWATER:   buffer += Level_Attribute::STRING_UNDERWATER + Common_Strings::STRING_NEW_LINE; break;
    case Level_Attribute::OVERWORLD:    buffer += Level_Attribute::STRING_OVERWORLD + Common_Strings::STRING_NEW_LINE; break;
    case Level_Attribute::UNDERGROUND:  buffer += Level_Attribute::STRING_UNDERGROUND + Common_Strings::STRING_NEW_LINE; break;
    case Level_Attribute::CASTLE:       buffer += Level_Attribute::STRING_CASTLE + Common_Strings::STRING_NEW_LINE; break;
    default:                            return false;
    }

    //Handle the Starting Position
    buffer += Header::STRING_STARTING_POSITION + ": ";
    switch (attribute) { //use the same starting position as the attribute of the level
    case Level_Attribute::UNDERWATER:   buffer += Level_Attribute::STRING_UNDERWATER + Common_Strings::STRING_NEW_LINE; break;
    case Level_Attribute::OVERWORLD:    buffer += Level_Attribute::STRING_OVERWORLD + Common_Strings::STRING_NEW_LINE; break;
    case Level_Attribute::UNDERGROUND:  buffer += Level_Attribute::STRING_UNDERGROUND + Common_Strings::STRING_NEW_LINE; break;
    case Level_Attribute::CASTLE:       buffer += Level_Attribute::STRING_CASTLE + Common_Strings::STRING_NEW_LINE; break;
    default:                            return false;
    }

    //Handle the Brick
    buffer += Header::STRING_BRICK + ": ";
    switch (brick) {
    case Brick::NO_BRICKS:                          buffer += Brick::STRING_NO_BRICKS + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE:                            buffer += Brick::STRING_SURFACE + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_AND_CEILING:                buffer += Brick::STRING_SURFACE_AND_CEILING + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_AND_CEILING_3:              buffer += Brick::STRING_SURFACE_AND_CEILING_3 + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_AND_CEILING_4:              buffer += Brick::STRING_SURFACE_AND_CEILING_4 + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_AND_CEILING_8:              buffer += Brick::STRING_SURFACE_AND_CEILING_8 + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_4_AND_CEILING:              buffer += Brick::STRING_SURFACE_4_AND_CEILING + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_4_AND_CEILING_3:            buffer += Brick::STRING_SURFACE_4_AND_CEILING_3 + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_4_AND_CEILING_4:            buffer += Brick::STRING_SURFACE_4_AND_CEILING_4 + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_5_AND_CEILING:              buffer += Brick::STRING_SURFACE_5_AND_CEILING + Common_Strings::STRING_NEW_LINE; break;
    case Brick::CEILING:                            buffer += Brick::STRING_CEILING + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_5_AND_CEILING_4:            buffer += Brick::STRING_SURFACE_5_AND_CEILING_4 + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_8_AND_CEILING:              buffer += Brick::STRING_SURFACE_8_AND_CEILING + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:   buffer += Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_5 + Common_Strings::STRING_NEW_LINE; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:   buffer += Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_4 + Common_Strings::STRING_NEW_LINE; break;
    case Brick::ALL:                                buffer += Brick::STRING_ALL + Common_Strings::STRING_NEW_LINE; break;
    default:                                        return false;
    }

    //Handle the Background
    buffer += Header::STRING_BACKGROUND + ": ";
    switch (background) {
    case Background::BLANK_BACKGROUND:  buffer += Background::STRING_BLANK_BACKGROUND + Common_Strings::STRING_NEW_LINE; break;
    case Background::IN_WATER:          buffer += Background::STRING_IN_WATER + Common_Strings::STRING_NEW_LINE; break;
    case Background::CASTLE_WALL:       buffer += Background::STRING_CASTLE_WALL + Common_Strings::STRING_NEW_LINE; break;
    case Background::OVER_WATER:        buffer += Background::STRING_OVER_WATER + Common_Strings::STRING_NEW_LINE; break;
    case Background::NIGHT:             buffer += Background::STRING_NIGHT + Common_Strings::STRING_NEW_LINE; break;
    case Background::SNOW:              buffer += Background::STRING_SNOW + Common_Strings::STRING_NEW_LINE; break;
    case Background::NIGHT_AND_SNOW:    buffer += Background::STRING_NIGHT_AND_SNOW + Common_Strings::STRING_NEW_LINE; break;
    case Background::NIGHT_AND_FREEZE:  buffer += Background::STRING_NIGHT_AND_FREEZE + Common_Strings::STRING_NEW_LINE; break;
    default:                            return false;
    }

    //Handle the Scenery
    buffer += Header::STRING_SCENERY + ": ";
    switch (scenery) {
    case Scenery::NO_SCENERY:   buffer += Scenery::STRING_NO_SCENERY + Common_Strings::STRING_NEW_LINE; break;
    case Scenery::ONLY_CLOUDS:  buffer += Scenery::STRING_ONLY_CLOUDS + Common_Strings::STRING_NEW_LINE; break;
    case Scenery::MOUNTAINS:    buffer += Scenery::STRING_MOUNTAINS + Common_Strings::STRING_NEW_LINE; break;
    case Scenery::FENCES:       buffer += Scenery::STRING_FENCES + Common_Strings::STRING_NEW_LINE; break;
    default:                    return false;
    }

    //Handle the Level Compliment
    buffer += Header::STRING_COMPLIMENT + ": ";
    switch (compliment) {
    case Level_Compliment::TREES:               buffer += Level_Compliment::STRING_TREES + Common_Strings::STRING_NEW_LINE; break;
    case Level_Compliment::MUSHROOMS:           buffer += Level_Compliment::STRING_MUSHROOMS + Common_Strings::STRING_NEW_LINE; break;
    case Level_Compliment::BULLET_BILL_TURRETS: buffer += Level_Compliment::STRING_BULLET_BILL_TURRETS + Common_Strings::STRING_NEW_LINE; break;
    case Level_Compliment::CLOUDS:              buffer += Level_Compliment::STRING_CLOUDS + Common_Strings::STRING_NEW_LINE; break;
    default:                                    return false;
    }

    //Time
    buffer += Header::STRING_TIME + ": " + QString::number(time) + Common_Strings::STRING_NEW_LINE;

    //Midpoint
    buffer += Header::STRING_MIDPOINT + ": " + QString::number(halfwayPoint) + Common_Strings::STRING_NEW_LINE;
    buffer += Level_Type::STRING_BREAK + Common_Strings::STRING_NEW_LINE;

    //Append the current file's contents to the buffer
    if (!this->file->seek(0)) return false;
    buffer += QString(this->file->readAll());

    //Write the new file
    if (!this->file->seek(0)) return false;
    if (this->file->write(buffer.toUtf8().data()) == -1) return false;

    this->written = true;
    return true;
}
