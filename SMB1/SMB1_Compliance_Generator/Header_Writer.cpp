#include "Header_Writer.h"
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

Header_Writer::Header_Writer() {
    this->buffer = new Text_Insertion_Buffer();
}

Header_Writer::~Header_Writer() {
    delete this->buffer;
}

bool Header_Writer::Write_Header_To_Buffer(Level_Type::Level_Type type, Level_Attribute::Level_Attribute attribute, Brick::Brick brick, Background::Background background, Scenery::Scenery scenery, Level_Compliment::Level_Compliment compliment,
                  int time, int halfwayPoint, int difficulty, int levelLength, int numObjects, int numEnemies, int numPipePointers) {
    if (!this->buffer->Is_Empty()) return false; //cannot write the header twice

    //Add the notes section
    this->buffer->Insert_At_End(Header::STRING_NAME);
    this->buffer->Insert_At_End(Level_Type::STRING_BREAK);
    this->buffer->Insert_At_End(Header::STRING_COOLCORD);
    this->buffer->Insert_At_End(Header::STRING_CREATED + " " + QDate::currentDate().toString("dddd, MMMM dd, yyyy") + ", at " + QTime::currentTime().toString("hh:mm:ss A") + ".");
    //Handle the Level Type
    this->buffer->Add_To_Line_Buffer(Header::STRING_TYPE + ": ");
    switch (type) {
    case Level_Type::STANDARD_OVERWORLD:    this->buffer->Insert_At_End(Level_Type::STRING_STANDARD_OVERWORLD); break;
    case Level_Type::ISLAND:                this->buffer->Insert_At_End(Level_Type::STRING_ISLAND); break;
    case Level_Type::BRIDGE:                this->buffer->Insert_At_End(Level_Type::STRING_BRIDGE); break;
    case Level_Type::UNDERGROUND:           this->buffer->Insert_At_End(Level_Type::STRING_UNDERGROUND); break;
    case Level_Type::UNDERWATER:            this->buffer->Insert_At_End(Level_Type::STRING_UNDERWATER); break;
    case Level_Type::CASTLE:                this->buffer->Insert_At_End(Level_Type::STRING_CASTLE); break;
    case Level_Type::PIPE_EXIT:             this->buffer->Insert_At_End(Level_Type::STRING_PIPE_EXIT); break;
    case Level_Type::UNDERGROUND_BONUS:     this->buffer->Insert_At_End(Level_Type::STRING_UNDERGROUND_BONUS); break;
    default:                                return false;
    }
    this->buffer->Insert_At_End(Header::STRING_DIFFICULTY + ": " + QString::number(difficulty));
    this->buffer->Insert_At_End(Header::STRING_LEVEL_LENGTH + ": " + QString::number(levelLength));
    this->buffer->Insert_At_End(Header::STRING_NUMBER_OF_OBJECTS + ": " + QString::number(numObjects));
    this->buffer->Insert_At_End(Header::STRING_NUMBER_OF_ENEMIES + ": " + QString::number(numEnemies));
    this->buffer->Insert_At_End(Header::STRING_NUMBER_OF_PIPE_POINTERS + ": " + QString::number(numPipePointers));
    this->buffer->Insert_At_End(Level_Type::STRING_BREAK);

    //Handle the Level Attribute
    this->buffer->Add_To_Line_Buffer(Header::STRING_ATTRIBUTE + ": ");
    switch (attribute) {
    case Level_Attribute::UNDERWATER:   this->buffer->Insert_At_End(Level_Attribute::STRING_UNDERWATER); break;
    case Level_Attribute::OVERWORLD:    this->buffer->Insert_At_End(Level_Attribute::STRING_OVERWORLD); break;
    case Level_Attribute::UNDERGROUND:  this->buffer->Insert_At_End(Level_Attribute::STRING_UNDERGROUND); break;
    case Level_Attribute::CASTLE:       this->buffer->Insert_At_End(Level_Attribute::STRING_CASTLE); break;
    }

    //Handle the Starting Position
    this->buffer->Add_To_Line_Buffer(Header::STRING_STARTING_POSITION + ": ");
    switch (attribute) { //use the same starting position as the attribute of the level
    case Level_Attribute::UNDERWATER:   this->buffer->Insert_At_End(Level_Attribute::STRING_UNDERWATER); break;
    case Level_Attribute::OVERWORLD:    this->buffer->Insert_At_End(Level_Attribute::STRING_OVERWORLD); break;
    case Level_Attribute::UNDERGROUND:  this->buffer->Insert_At_End(Level_Attribute::STRING_UNDERGROUND); break;
    case Level_Attribute::CASTLE:       this->buffer->Insert_At_End(Level_Attribute::STRING_CASTLE); break;
    }

    //Handle the Brick
    this->buffer->Add_To_Line_Buffer(Header::STRING_BRICK + ": ");
    switch (brick) {
    case Brick::NO_BRICKS:                          this->buffer->Insert_At_End(Brick::STRING_NO_BRICKS); break;
    case Brick::SURFACE:                            this->buffer->Insert_At_End(Brick::STRING_SURFACE); break;
    case Brick::SURFACE_AND_CEILING:                this->buffer->Insert_At_End(Brick::STRING_SURFACE_AND_CEILING); break;
    case Brick::SURFACE_AND_CEILING_3:              this->buffer->Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_3); break;
    case Brick::SURFACE_AND_CEILING_4:              this->buffer->Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_4); break;
    case Brick::SURFACE_AND_CEILING_8:              this->buffer->Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_8); break;
    case Brick::SURFACE_4_AND_CEILING:              this->buffer->Insert_At_End(Brick::STRING_SURFACE_4_AND_CEILING); break;
    case Brick::SURFACE_4_AND_CEILING_3:            this->buffer->Insert_At_End(Brick::STRING_SURFACE_4_AND_CEILING_3); break;
    case Brick::SURFACE_4_AND_CEILING_4:            this->buffer->Insert_At_End(Brick::STRING_SURFACE_4_AND_CEILING_4); break;
    case Brick::SURFACE_5_AND_CEILING:              this->buffer->Insert_At_End(Brick::STRING_SURFACE_5_AND_CEILING); break;
    case Brick::CEILING:                            this->buffer->Insert_At_End(Brick::STRING_CEILING); break;
    case Brick::SURFACE_5_AND_CEILING_4:            this->buffer->Insert_At_End(Brick::STRING_SURFACE_5_AND_CEILING_4); break;
    case Brick::SURFACE_8_AND_CEILING:              this->buffer->Insert_At_End(Brick::STRING_SURFACE_8_AND_CEILING); break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:   this->buffer->Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_5); break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:   this->buffer->Insert_At_End(Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_4); break;
    case Brick::ALL:                                this->buffer->Insert_At_End(Brick::STRING_ALL); break;
    }

    //Handle the Background
    this->buffer->Add_To_Line_Buffer(Header::STRING_BACKGROUND + ": ");
    switch (background) {
    case Background::BLANK_BACKGROUND:  this->buffer->Insert_At_End(Background::STRING_BLANK_BACKGROUND); break;
    case Background::IN_WATER:          this->buffer->Insert_At_End(Background::STRING_IN_WATER); break;
    case Background::CASTLE_WALL:       this->buffer->Insert_At_End(Background::STRING_CASTLE_WALL); break;
    case Background::OVER_WATER:        this->buffer->Insert_At_End(Background::STRING_OVER_WATER); break;
    case Background::NIGHT:             this->buffer->Insert_At_End(Background::STRING_NIGHT); break;
    case Background::SNOW:              this->buffer->Insert_At_End(Background::STRING_SNOW); break;
    case Background::NIGHT_AND_SNOW:    this->buffer->Insert_At_End(Background::STRING_NIGHT_AND_SNOW); break;
    case Background::NIGHT_AND_FREEZE:  this->buffer->Insert_At_End(Background::STRING_NIGHT_AND_FREEZE); break;
    }

    //Handle the Scenery
    this->buffer->Add_To_Line_Buffer(Header::STRING_SCENERY + ": ");
    switch (scenery) {
    case Scenery::NO_SCENERY:   this->buffer->Insert_At_End(Scenery::STRING_NO_SCENERY); break;
    case Scenery::ONLY_CLOUDS:  this->buffer->Insert_At_End(Scenery::STRING_ONLY_CLOUDS); break;
    case Scenery::MOUNTAINS:    this->buffer->Insert_At_End(Scenery::STRING_MOUNTAINS); break;
    case Scenery::FENCES:       this->buffer->Insert_At_End(Scenery::STRING_FENCES); break;
    }

    //Handle the Level Compliment
    this->buffer->Add_To_Line_Buffer(Header::STRING_COMPLIMENT + ": ");
    switch (compliment) {
    case Level_Compliment::TREES:               this->buffer->Insert_At_End(Level_Compliment::STRING_TREES); break;
    case Level_Compliment::MUSHROOMS:           this->buffer->Insert_At_End(Level_Compliment::STRING_MUSHROOMS); break;
    case Level_Compliment::BULLET_BILL_TURRETS: this->buffer->Insert_At_End(Level_Compliment::STRING_BULLET_BILL_TURRETS); break;
    case Level_Compliment::CLOUDS:              this->buffer->Insert_At_End(Level_Compliment::STRING_CLOUDS); break;
    }

    //Time
    this->buffer->Insert_At_End(Header::STRING_TIME + ": " + QString::number(time));

    //Midpoint
    this->buffer->Insert_At_End(Header::STRING_MIDPOINT + ": " + QString::number(halfwayPoint));
    return true;
}

bool Header_Writer::Write_Buffer_To_File(QFile *file) {
    if (this->buffer->Is_Empty()) return false;
    return this->buffer->Write_To_File(file);
}
