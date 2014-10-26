#include "Header_Handler.h"
#include "../Common SMB1 Files/Header_String.h"
#include "../Common SMB1 Files/Level_Type_String.h"
#include "../Common SMB1 Files/Level_Attribute_String.h"
#include "../Common SMB1 Files/Brick_String.h"
#include "../Common SMB1 Files/Background_String.h"
#include "../Common SMB1 Files/Scenery_String.h"
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

    //Populate Bricks
    this->bricks = new QMap<QString, Brick::Brick>();
    this->Populate_Bricks();

    //Populate Backgrounds
    this->backgrounds = new QMap<QString, Background::Background>();
    this->Populate_Backgrounds();

    //Populate Sceneries
    this->sceneries = new QMap<QString, Scenery::Scenery>();
    this->Populate_Sceneries();

    //Populate Level Compliments
    this->compliments = new QMap<QString, Level_Compliment::Level_Compliment>();
    this->Populate_Compliments();
}

Header_Handler::~Header_Handler() {
    delete this->types;
    delete this->attributes;
    delete this->bricks;
    delete this->backgrounds;
    delete this->sceneries;
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

    //Notes Section -- Look for 2 seperators
    for (int i = 0; i < 2; ++i) {
        do {
            ++lineNum;
            line = file->readLine();
            if (line.isEmpty()) continue;
            if (file->atEnd()) return false;
            line.chop(1); //remove the new line character
        } while (line != Level_Type::STRING_BREAK);
    }

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
    if (!this->writerPlugin->Header_Starting_Position(attributeIter.value())) return false;
    //TODO: Add support for setting the attribute

    //Brick
    ++lineNum;
    line = this->file->readLine(); line.chop(1);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_BRICK + ":") return false;
    QMap<QString, Brick::Brick>::iterator brickIter = this->bricks->find(elements.at(1));
    if (brickIter == this->bricks->end()) return false; //not found
    if (!this->writerPlugin->Header_Brick(brickIter.value())) return false;

    //Background
    ++lineNum;
    line = this->file->readLine(); line.chop(1);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_BACKGROUND + ":") return false;
    QMap<QString, Background::Background>::iterator backgroundIter = this->backgrounds->find(elements.at(1));
    if (backgroundIter == this->backgrounds->end()) return false; //not found
    if (!this->writerPlugin->Header_Background(backgroundIter.value())) return false;

    //Scenery
    ++lineNum;
    line = this->file->readLine(); line.chop(1);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_SCENERY + ":") return false;
    QMap<QString, Scenery::Scenery>::iterator sceneryIter = this->sceneries->find(elements.at(1));
    if (sceneryIter == this->sceneries->end()) return false; //not found
    if (!this->writerPlugin->Header_Scenery(sceneryIter.value())) return false;

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

    //Seperator
    bool success = false;
    do {
        ++lineNum;
        line = file->readLine();
        if (line.isEmpty()) continue;
        line.chop(1); //remove the new line character
        if (line == Level_Type::STRING_BREAK) {
            success = true;
            break;
        }
    } while (!file->atEnd());

    return success;
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

void Header_Handler::Populate_Bricks() {
    assert(this->bricks);
    this->bricks->clear();
    this->bricks->insert(Brick::STRING_NO_BRICKS, Brick::NO_BRICKS);
    this->bricks->insert(Brick::STRING_SURFACE, Brick::SURFACE);
    this->bricks->insert(Brick::STRING_SURFACE_AND_CEILING, Brick::SURFACE_AND_CEILING);
    this->bricks->insert(Brick::STRING_SURFACE_AND_CEILING_3, Brick::SURFACE_AND_CEILING_3);
    this->bricks->insert(Brick::STRING_SURFACE_AND_CEILING_4, Brick::SURFACE_AND_CEILING_4);
    this->bricks->insert(Brick::STRING_SURFACE_AND_CEILING_8, Brick::SURFACE_AND_CEILING_8);
    this->bricks->insert(Brick::STRING_SURFACE_4_AND_CEILING, Brick::SURFACE_4_AND_CEILING);
    this->bricks->insert(Brick::STRING_SURFACE_4_AND_CEILING_3, Brick::SURFACE_4_AND_CEILING_3);
    this->bricks->insert(Brick::STRING_SURFACE_4_AND_CEILING_4, Brick::SURFACE_4_AND_CEILING_4);
    this->bricks->insert(Brick::STRING_SURFACE_5_AND_CEILING, Brick::SURFACE_5_AND_CEILING);
    this->bricks->insert(Brick::STRING_CEILING, Brick::CEILING);
    this->bricks->insert(Brick::STRING_SURFACE_5_AND_CEILING_4, Brick::SURFACE_5_AND_CEILING_4);
    this->bricks->insert(Brick::STRING_SURFACE_8_AND_CEILING, Brick::SURFACE_8_AND_CEILING);
    this->bricks->insert(Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_5, Brick::SURFACE_AND_CEILING_AND_MIDDLE_5);
    this->bricks->insert(Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_4, Brick::SURFACE_AND_CEILING_AND_MIDDLE_4);
    this->bricks->insert(Brick::STRING_ALL, Brick::ALL);
}

void Header_Handler::Populate_Backgrounds() {
    assert(this->backgrounds);
    this->backgrounds->clear();
    this->backgrounds->insert(Background::STRING_BLANK_BACKGROUND, Background::BLANK_BACKGROUND);
    this->backgrounds->insert(Background::STRING_IN_WATER, Background::IN_WATER);
    this->backgrounds->insert(Background::STRING_CASTLE_WALL, Background::CASTLE_WALL);
    this->backgrounds->insert(Background::STRING_OVER_WATER, Background::OVER_WATER);
    this->backgrounds->insert(Background::STRING_NIGHT, Background::NIGHT);
    this->backgrounds->insert(Background::STRING_SNOW, Background::SNOW);
    this->backgrounds->insert(Background::STRING_NIGHT_AND_SNOW, Background::NIGHT_AND_SNOW);
    this->backgrounds->insert(Background::STRING_NIGHT_AND_FREEZE, Background::NIGHT_AND_FREEZE);
}

void Header_Handler::Populate_Sceneries() {
    assert(this->sceneries);
    this->sceneries->clear();
    this->sceneries->insert(Scenery::STRING_NO_SCENERY, Scenery::NO_SCENERY);
    this->sceneries->insert(Scenery::STRING_ONLY_CLOUDS, Scenery::ONLY_CLOUDS);
    this->sceneries->insert(Scenery::STRING_MOUNTAINS, Scenery::MOUNTAINS);
    this->sceneries->insert(Scenery::STRING_FENCES, Scenery::FENCES);
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
    return this->Parse_Num(elements.at(1), num);
}
