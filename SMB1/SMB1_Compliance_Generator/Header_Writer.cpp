#include "Header_Writer.h"
#include "../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.h"
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
#include <assert.h>

Header_Writer::Header_Writer(QFile *file) {
    assert(file);
    this->file = file;
    this->written = false;
}

bool Header_Writer::Write_Header(Level_Type::Level_Type type, Level_Attribute::Level_Attribute attribute, Brick::Brick brick, Background::Background background, Scenery::Scenery scenery, Level_Compliment::Level_Compliment compliment,
                  int time, int halfwayPoint, int difficulty, int levelLength, int numObjects, int numEnemies, int numPipePointers) {
    if (this->written) return false; //cannot write the header twice

    //Prepare the buffer
    Text_Insertion_Buffer buffer;
    buffer.Insert_At_End(Header::STRING_NAME);

    //Add the notes section
    buffer.Insert_At_End(Level_Type::STRING_BREAK);
    buffer.Insert_At_End(Header::STRING_COOLCORD);
    buffer.Insert_At_End(Header::STRING_CREATED + " " + QDate::currentDate().toString("dddd, MMMM dd, yyyy") + ", at " + QTime::currentTime().toString("hh:mm:ss A") + ".");
    //Handle the Level Type
    buffer.Add_To_Line_Buffer(Header::STRING_TYPE + ": ");
    switch (type) {
    case Level_Type::STANDARD_OVERWORLD:    buffer.Insert_At_End(Level_Type::STRING_STANDARD_OVERWORLD); break;
    case Level_Type::ISLAND:                buffer.Insert_At_End(Level_Type::STRING_ISLAND); break;
    case Level_Type::BRIDGE:                buffer.Insert_At_End(Level_Type::STRING_BRIDGE); break;
    case Level_Type::UNDERGROUND:           buffer.Insert_At_End(Level_Type::STRING_UNDERGROUND); break;
    case Level_Type::UNDERWATER:            buffer.Insert_At_End(Level_Type::STRING_UNDERWATER); break;
    case Level_Type::CASTLE:                buffer.Insert_At_End(Level_Type::STRING_CASTLE); break;
    default:                                return false;
    }
    buffer.Insert_At_End(Header::STRING_DIFFICULTY + ": " + QString::number(difficulty));
    buffer.Insert_At_End(Header::STRING_LEVEL_LENGTH + ": " + QString::number(levelLength));
    buffer.Insert_At_End(Header::STRING_NUMBER_OF_OBJECTS + ": " + QString::number(numObjects));
    buffer.Insert_At_End(Header::STRING_NUMBER_OF_ENEMIES + ": " + QString::number(numEnemies));
    buffer.Insert_At_End(Header::STRING_NUMBER_OF_PIPE_POINTERS + ": " + QString::number(numPipePointers));
    buffer.Insert_At_End(Level_Type::STRING_BREAK);

    //Handle the Level Attribute
    buffer.Add_To_Line_Buffer(Header::STRING_ATTRIBUTE + ": ");
    switch (attribute) {
    case Level_Attribute::UNDERWATER:   buffer.Insert_At_End(Level_Attribute::STRING_UNDERWATER); break;
    case Level_Attribute::OVERWORLD:    buffer.Insert_At_End(Level_Attribute::STRING_OVERWORLD); break;
    case Level_Attribute::UNDERGROUND:  buffer.Insert_At_End(Level_Attribute::STRING_UNDERGROUND); break;
    case Level_Attribute::CASTLE:       buffer.Insert_At_End(Level_Attribute::STRING_CASTLE); break;
    }

    //Handle the Starting Position
    buffer.Add_To_Line_Buffer(Header::STRING_STARTING_POSITION + ": ");
    switch (attribute) { //use the same starting position as the attribute of the level
    case Level_Attribute::UNDERWATER:   buffer.Insert_At_End(Level_Attribute::STRING_UNDERWATER); break;
    case Level_Attribute::OVERWORLD:    buffer.Insert_At_End(Level_Attribute::STRING_OVERWORLD); break;
    case Level_Attribute::UNDERGROUND:  buffer.Insert_At_End(Level_Attribute::STRING_UNDERGROUND); break;
    case Level_Attribute::CASTLE:       buffer.Insert_At_End(Level_Attribute::STRING_CASTLE); break;
    }

    //Handle the Brick
    buffer.Add_To_Line_Buffer(Header::STRING_BRICK + ": ");
    switch (brick) {
    case Brick::NO_BRICKS:                          buffer.Insert_At_End(Brick::STRING_NO_BRICKS); break;
    case Brick::SURFACE:                            buffer.Insert_At_End(Brick::STRING_SURFACE); break;
    case Brick::SURFACE_AND_CEILING:                buffer.Insert_At_End(Brick::STRING_SURFACE_AND_CEILING); break;
    case Brick::SURFACE_AND_CEILING_3:              buffer.Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_3); break;
    case Brick::SURFACE_AND_CEILING_4:              buffer.Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_4); break;
    case Brick::SURFACE_AND_CEILING_8:              buffer.Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_8); break;
    case Brick::SURFACE_4_AND_CEILING:              buffer.Insert_At_End(Brick::STRING_SURFACE_4_AND_CEILING); break;
    case Brick::SURFACE_4_AND_CEILING_3:            buffer.Insert_At_End(Brick::STRING_SURFACE_4_AND_CEILING_3); break;
    case Brick::SURFACE_4_AND_CEILING_4:            buffer.Insert_At_End(Brick::STRING_SURFACE_4_AND_CEILING_4); break;
    case Brick::SURFACE_5_AND_CEILING:              buffer.Insert_At_End(Brick::STRING_SURFACE_5_AND_CEILING); break;
    case Brick::CEILING:                            buffer.Insert_At_End(Brick::STRING_CEILING); break;
    case Brick::SURFACE_5_AND_CEILING_4:            buffer.Insert_At_End(Brick::STRING_SURFACE_5_AND_CEILING_4); break;
    case Brick::SURFACE_8_AND_CEILING:              buffer.Insert_At_End(Brick::STRING_SURFACE_8_AND_CEILING); break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:   buffer.Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_5); break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:   buffer.Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_4); break;
    case Brick::ALL:                                buffer.Insert_At_End(Brick::STRING_ALL); break;
    }

    //Handle the Background
    buffer.Add_To_Line_Buffer(Header::STRING_BACKGROUND + ": ");
    switch (background) {
    case Background::BLANK_BACKGROUND:  buffer.Insert_At_End(Background::STRING_BLANK_BACKGROUND); break;
    case Background::IN_WATER:          buffer.Insert_At_End(Background::STRING_IN_WATER); break;
    case Background::CASTLE_WALL:       buffer.Insert_At_End(Background::STRING_CASTLE_WALL); break;
    case Background::OVER_WATER:        buffer.Insert_At_End(Background::STRING_OVER_WATER); break;
    case Background::NIGHT:             buffer.Insert_At_End(Background::STRING_NIGHT); break;
    case Background::SNOW:              buffer.Insert_At_End(Background::STRING_SNOW); break;
    case Background::NIGHT_AND_SNOW:    buffer.Insert_At_End(Background::STRING_NIGHT_AND_SNOW); break;
    case Background::NIGHT_AND_FREEZE:  buffer.Insert_At_End(Background::STRING_NIGHT_AND_FREEZE); break;
    }

    //Handle the Scenery
    buffer.Add_To_Line_Buffer(Header::STRING_SCENERY + ": ");
    switch (scenery) {
    case Scenery::NO_SCENERY:   buffer.Insert_At_End(Scenery::STRING_NO_SCENERY); break;
    case Scenery::ONLY_CLOUDS:  buffer.Insert_At_End(Scenery::STRING_ONLY_CLOUDS); break;
    case Scenery::MOUNTAINS:    buffer.Insert_At_End(Scenery::STRING_MOUNTAINS); break;
    case Scenery::FENCES:       buffer.Insert_At_End(Scenery::STRING_FENCES); break;
    }

    //Handle the Level Compliment
    buffer.Add_To_Line_Buffer(Header::STRING_COMPLIMENT + ": ");
    switch (compliment) {
    case Level_Compliment::TREES:               buffer.Insert_At_End(Level_Compliment::STRING_TREES); break;
    case Level_Compliment::MUSHROOMS:           buffer.Insert_At_End(Level_Compliment::STRING_MUSHROOMS); break;
    case Level_Compliment::BULLET_BILL_TURRETS: buffer.Insert_At_End(Level_Compliment::STRING_BULLET_BILL_TURRETS); break;
    case Level_Compliment::CLOUDS:              buffer.Insert_At_End(Level_Compliment::STRING_CLOUDS); break;
    }

    //Time
    buffer.Insert_At_End(Header::STRING_TIME + ": " + QString::number(time));

    //Midpoint
    buffer.Insert_At_End(Header::STRING_MIDPOINT + ": " + QString::number(halfwayPoint));
    buffer.Insert_At_End(Level_Type::STRING_BREAK);

    //TODO: Remove this and write it after objects and enemies are ready
    //Append the current file's contents to the buffer
    //==================================================================
    if (!this->file->seek(0)) return false;
    QTextStream stream(this->file);
    while (!stream.atEnd()) buffer.Insert_At_End(stream.readLine().trimmed());
    if (!this->file->seek(0)) return false;
    //==================================================================

    //Write the new file
    if (!buffer.Write_To_File(this->file)) return false;

    this->written = true;
    return true;
}
