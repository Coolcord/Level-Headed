#include "Header_Writer.h"
#include <assert.h>
#include "../Common SMB1 Files/Level_Type_String.h"
#include "../Common SMB1 Files/Level_Attribute_String.h"
#include "../Common SMB1 Files/Level_Compliment_String.h"
#include <QDebug>

Header_Writer::Header_Writer(QFile *file) {
    assert(file);
    this->file = file;
    this->written = false;
}

bool Header_Writer::Write_Header(Level_Type::Level_Type type, Level_Attribute::Level_Attribute attribute, Level_Compliment::Level_Compliment compliment,
                  int time, int halfwayPoint, int levelLength, int numObjects, int numEnemies, int numPipePointers) {
    if (this->written) return false; //cannot write the header twice

    //Prepare the buffer
    QString buffer = "";
    buffer += Level_Type::STRING_NAME + "_Level" + "\n";

    //Handle the Level Type
    buffer += "Type: ";
    switch (type) {
    case Level_Type::STANDARD_OVERWORLD:    buffer += Level_Type::STRING_STANDARD_OVERWORLD + "\n"; break;
    case Level_Type::ISLAND:                buffer += Level_Type::STRING_ISLAND + "\n"; break;
    default:                                return false;
    }

    //Handle the Level Attribute
    buffer += "Attribute: ";
    switch (attribute) {
    case Level_Attribute::UNDERWATER:   buffer += Level_Attribute::STRING_UNDERWATER + "\n"; break;
    case Level_Attribute::OVERWORLD:    buffer += Level_Attribute::STRING_OVERWORLD + "\n"; break;
    case Level_Attribute::UNDERGROUND:  buffer += Level_Attribute::STRING_UNDERGROUND + "\n"; break;
    case Level_Attribute::CASTLE:       buffer += Level_Attribute::STRING_CASTLE + "\n"; break;
    default:                            return false;
    }

    //Handle the Level Compliment
    buffer += "Compliment: ";
    switch (compliment) {
    case Level_Compliment::TREES:               buffer += Level_Compliment::STRING_TREES + "\n"; break;
    case Level_Compliment::MUSHROOMS:           buffer += Level_Compliment::STRING_MUSHROOMS + "\n"; break;
    case Level_Compliment::BULLET_BILL_TURRETS: buffer += Level_Compliment::STRING_BULLET_BILL_TURRETS + "\n"; break;
    case Level_Compliment::CLOUDS:              buffer += Level_Compliment::STRING_CLOUDS + "\n"; break;
    default:                                    return false;
    }

    //Write the rest of the header to the buffer
    buffer += "Time: " + QString::number(time) + "\n";
    buffer += "Halfway_Point: " + QString::number(halfwayPoint) + "\n";
    buffer += "Level_Length: " + QString::number(levelLength) + "\n";
    buffer += "Number_Of_Objects: " + QString::number(numObjects) + "\n";
    buffer += "Number_Of_Enemies: " + QString::number(numEnemies) + "\n";
    buffer += "Number_Of_Pipe_Pointers: " + QString::number(numPipePointers) + "\n";
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
