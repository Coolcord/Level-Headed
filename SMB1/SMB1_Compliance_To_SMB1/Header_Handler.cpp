#include "Header_Handler.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common SMB1 Files/Header_String.h"
#include "../Common SMB1 Files/Level_Type_String.h"
#include "../Common SMB1 Files/Level_Attribute_String.h"
#include "../Common SMB1 Files/Brick_String.h"
#include "../Common SMB1 Files/Background_String.h"
#include "../Common SMB1 Files/Scenery_String.h"
#include "../Common SMB1 Files/Level_Compliment_String.h"
#include <assert.h>

Header_Handler::Header_Handler(SMB1_Writer_Interface *writerPlugin, QFile *file) : Item_Handler(writerPlugin) {
    assert(writerPlugin);
    assert(file);
    this->writerPlugin = writerPlugin;
    this->file = file;

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
    delete this->attributes;
    delete this->bricks;
    delete this->backgrounds;
    delete this->sceneries;
    delete this->compliments;
}

bool Header_Handler::Parse_Header(int &lineNum, int &errorCode) {
    assert(lineNum == 1);
    QStringList elements;
    int num = 0;

    //Level Name
    QString line;
    line = this->file->readLine().trimmed();
    if (line != Header::STRING_NAME) return false;

    //Notes Section -- Look for 2 seperators
    for (int i = 0; i < 2; ++i) {
        do {
            ++lineNum;
            line = file->readLine().trimmed();
            if (line.isEmpty()) continue;
            if (file->atEnd()) return false; //TODO: Handle this error
        } while (!line.startsWith("==="));
    }

    //Attribute
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_ATTRIBUTE + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_ATTRIBUTE + ":") return false;
    QMap<QString, Level_Attribute::Level_Attribute>::iterator attributeIter = this->attributes->find(elements.at(1));
    if (attributeIter == this->attributes->end()) return false; //not found
    if (!this->writerPlugin->Header_Attribute(attributeIter.value())) {
        errorCode = 3;
        return false;
    }

    //Starting Position
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_STARTING_POSITION + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_STARTING_POSITION + ":") return false;
    //Handle Autowalk if the user set it
    if (elements.at(1) == Level_Attribute::STRING_OVERWORLD_WALKING) {
        if (!this->writerPlugin->Header_Autowalk(true)) {
            errorCode = 3;
            return false;
        }
        if (!this->writerPlugin->Header_Starting_Position(Level_Attribute::OVERWORLD)) {
            errorCode = 3;
            return false;
        }
    } else {
        //Remember that starting positions and attributes are very similar!
        attributeIter = this->attributes->find(elements.at(1));
        if (attributeIter == this->attributes->end()) return false; //not found
        if (!this->writerPlugin->Header_Autowalk(false)) {
            errorCode = 3;
            return false;
        }
        if (!this->writerPlugin->Header_Starting_Position(attributeIter.value())) {
            errorCode = 3;
            return false;
        }
    }

    //Brick
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_BRICK + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_BRICK + ":") return false;
    QMap<QString, Brick::Brick>::iterator brickIter = this->bricks->find(elements.at(1));
    if (brickIter == this->bricks->end()) return false; //not found
    if (!this->writerPlugin->Header_Brick(brickIter.value())) {
        errorCode = 3;
        return false;
    }

    //Background
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_BACKGROUND + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_BACKGROUND + ":") return false;
    QMap<QString, Background::Background>::iterator backgroundIter = this->backgrounds->find(elements.at(1));
    if (backgroundIter == this->backgrounds->end()) return false; //not found
    if (!this->writerPlugin->Header_Background(backgroundIter.value())) {
        errorCode = 3;
        return false;
    }

    //Scenery
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_SCENERY + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_SCENERY + ":") return false;
    QMap<QString, Scenery::Scenery>::iterator sceneryIter = this->sceneries->find(elements.at(1));
    if (sceneryIter == this->sceneries->end()) return false; //not found
    if (!this->writerPlugin->Header_Scenery(sceneryIter.value())) {
        errorCode = 3;
        return false;
    }

    //Compliment
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_COMPLIMENT + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_COMPLIMENT + ":") return false;
    QMap<QString, Level_Compliment::Level_Compliment>::iterator complimentIter = this->compliments->find(elements.at(1));
    if (complimentIter == this->compliments->end()) return false; //not found
    if (!this->writerPlugin->Header_Level_Compliment(complimentIter.value())) {
        errorCode = 3;
        return false;
    }

    //Time
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_TIME + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_TIME + ":") return false;
    if (!this->Parse_Num(elements.at(1), num)) return false;
    if (!this->writerPlugin->Header_Time(num)) {
        errorCode = 3;
        return false;
    }

    //Midpoint
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_MIDPOINT + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_MIDPOINT + ":") return false;
    if (!this->Parse_Num(elements.at(1), num)) return false;
    if (!this->writerPlugin->Header_Midpoint(num)) {
        errorCode = 3;
        return false;
    }

    //Seperator
    do {
        ++lineNum;
        line = this->file->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (this->file->atEnd()) return false; //TODO: Handle this error
    } while (!line.startsWith("==="));

    return true;
}

bool Header_Handler::Parse_Through_Comments_Until_String(const QString &value, int &lineNum) {
    do {
        ++lineNum;
        QString line = file->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line == value) return true;
    } while (!file->atEnd());
    return false;
}

QString Header_Handler::Parse_Through_Comments_Until_First_Word(const QString &word, int &lineNum) {
    do {
        ++lineNum;
        QString line = this->file->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        QStringList elements = line.split(' ');
        if (elements.at(0) == word) return line;
    } while (!file->atEnd());
    return "Invalid Line";
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
    QString line = file->readLine().trimmed();
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != name + ":") return false;
    int num = 0;
    return this->Parse_Num(elements.at(1), num);
}
