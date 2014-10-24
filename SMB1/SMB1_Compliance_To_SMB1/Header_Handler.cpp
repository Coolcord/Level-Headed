#include "Header_Handler.h"
#include "../Common SMB1 Files/Header_String.h"
#include "../Common SMB1 Files/Level_Type_String.h"
#include "../Common SMB1 Files/Level_Attribute_String.h"
#include "../Common SMB1 Files/Level_Compliment_String.h"
#include <assert.h>

Header_Handler::Header_Handler(SMB1_Writer_Interface *writerPlugin, QFile *file) : Item_Handler::Item_Handler(writerPlugin) {
    assert(writerPlugin);
    assert(file);
    this->writerPlugin = writerPlugin;
    this->file = file;

    //Populate Level Types
    this->types = new QMap<QString, Level_Type::Level_Type>();
    this->Populate_Types();

    //Populate Level Attributes
    this->attributes = new QMap<QString, Level_Attribute::Level_Attribute>();
    this->Populate_Attributes();

    //Populate Level Compliments
    this->compliments = new QMap<QString, Level_Compliment::Level_Compliment>();
    this->Populate_Compliments();
}

Header_Handler::~Header_Handler() {
    delete this->types;
    delete this->attributes;
    delete this->compliments;
}

bool Header_Handler::Parse_Header(int &lineNum) {
    assert(lineNum == 1);
    QStringList elements;
    int num = 0;

    //Level Name
    QString line;
    line = this->file->readLine(); line.chop(1);
    if (line != Header::STRING_NAME) return false;

    //Type
    ++lineNum;
    line = this->file->readLine(); line.chop(1);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_TYPE + ":") return false;
    QMap<QString, Level_Type::Level_Type>::iterator typeIter = this->types->find(elements.at(1));
    if (typeIter == this->types->end()) return false; //not found

    //Attribute
    ++lineNum;
    line = this->file->readLine(); line.chop(1);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_ATTRIBUTE + ":") return false;
    QMap<QString, Level_Attribute::Level_Attribute>::iterator attributeIter = this->attributes->find(elements.at(1));
    if (attributeIter == this->attributes->end()) return false; //not found
    //TODO: Add support for setting the attribute

    //Compliment
    ++lineNum;
    line = this->file->readLine(); line.chop(1);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_COMPLIMENT + ":") return false;
    QMap<QString, Level_Compliment::Level_Compliment>::iterator complimentIter = this->compliments->find(elements.at(1));
    if (complimentIter == this->compliments->end()) return false; //not found
    if (!this->writerPlugin->Header_Level_Compliment(complimentIter.value())) return false;

    //Time
    ++lineNum;
    line = this->file->readLine(); line.chop(1);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_TIME + ":") return false;
    if (!this->Parse_Num(elements.at(1), num)) return false;
    if (!this->writerPlugin->Header_Time(num)) return false;

    //Midpoint
    ++lineNum;
    line = this->file->readLine(); line.chop(1);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_MIDPOINT + ":") return false;
    if (!this->Parse_Num(elements.at(1), num)) return false;
    if (!this->writerPlugin->Header_Midpoint(num)) return false;

    //Level Length
    if (!this->Parse_Ignored_Line(Header::STRING_LEVEL_LENGTH, lineNum)) {
        return false;
    }

    //Number of Objects
    if (!this->Parse_Ignored_Line(Header::STRING_NUMBER_OF_OBJECTS, lineNum)) {
        return false;
    }

    //Number of Enemies
    if (!this->Parse_Ignored_Line(Header::STRING_NUMBER_OF_ENEMIES, lineNum)) {
        return false;
    }

    //Number of Pipe Pointers
    if (!this->Parse_Ignored_Line(Header::STRING_NUMBER_OF_PIPE_POINTERS, lineNum)) {
        return false;
    }

    //Seperator
    ++lineNum;
    line = file->readLine(); line.chop(1);
    if (line != Level_Type::STRING_BREAK) return false;

    return true;
}

void Header_Handler::Populate_Types() {
    assert(this->types);
    this->types->clear();
    this->types->insert(Level_Type::STRING_STANDARD_OVERWORLD, Level_Type::STANDARD_OVERWORLD);
    this->types->insert(Level_Type::STRING_ISLAND, Level_Type::ISLAND);
    this->types->insert(Level_Type::STRING_UNDERGROUND, Level_Type::UNDERGROUND);
    this->types->insert(Level_Type::STRING_UNDERWATER, Level_Type::UNDERWATER);
    this->types->insert(Level_Type::STRING_CASTLE, Level_Type::CASTLE);
    this->types->insert(Level_Type::STRING_BRIDGE, Level_Type::BRIDGE);
}

void Header_Handler::Populate_Attributes() {
    assert(this->attributes);
    this->attributes->clear();
    this->attributes->insert(Level_Attribute::STRING_OVERWORLD, Level_Attribute::OVERWORLD);
    this->attributes->insert(Level_Attribute::STRING_UNDERGROUND, Level_Attribute::UNDERGROUND);
    this->attributes->insert(Level_Attribute::STRING_UNDERWATER, Level_Attribute::UNDERWATER);
    this->attributes->insert(Level_Attribute::STRING_CASTLE, Level_Attribute::CASTLE);
}

void Header_Handler::Populate_Compliments() {
    assert(this->compliments);
    this->compliments->clear();
    this->compliments->insert(Level_Compliment::STRING_TREES, Level_Compliment::TREES);
    this->compliments->insert(Level_Compliment::STRING_BULLET_BILL_TURRETS, Level_Compliment::BULLET_BILL_TURRETS);
    this->compliments->insert(Level_Compliment::STRING_CLOUDS, Level_Compliment::CLOUDS);
    this->compliments->insert(Level_Compliment::STRING_MUSHROOMS, Level_Compliment::MUSHROOMS);
}

bool Header_Handler::Parse_Ignored_Line(const QString &name, int &lineNum) {
    ++lineNum;
    QString line = file->readLine(); line.chop(1);
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != name + ":") return false;
    int num = 0;
    if (!this->Parse_Num(elements.at(1), num)) return false;
}
