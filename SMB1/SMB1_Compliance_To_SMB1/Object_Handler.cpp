#include "Object_Handler.h"
#include "../../Common_Files/Random.h"
#include "../Common_SMB1_Files/Scenery_String.h"
#include "../Common_SMB1_Files/Brick_String.h"
#include "../Common_SMB1_Files/Background_String.h"
#include <QStringList>
#include <assert.h>

Object_Handler::Object_Handler(SMB1_Writer_Interface *wp, bool randomEnemies) : Item_Handler(wp) {
    assert(wp);
    this->writerPlugin = wp;
    this->useRandomEnemies = randomEnemies;

    //Build the map for Sceneries
    this->sceneries = new QMap<QString, Scenery::Scenery>();
    this->Populate_Scenery_Map();

    //Build the map for Bricks
    this->bricks = new QMap<QString, Brick::Brick>();
    this->Populate_Brick_Map();

    //Build the map for Backgrounds
    this->backgrounds = new QMap<QString, Background::Background>();
    this->Populate_Background_Map();
}

Object_Handler::~Object_Handler() {
    delete this->sceneries;
    delete this->bricks;
    delete this->backgrounds;
}

void Object_Handler::Populate_Scenery_Map() {
    this->sceneries->clear();
    this->sceneries->insert(Scenery::STRING_NO_SCENERY, Scenery::NO_SCENERY);
    this->sceneries->insert(Scenery::STRING_ONLY_CLOUDS, Scenery::ONLY_CLOUDS);
    this->sceneries->insert(Scenery::STRING_MOUNTAINS, Scenery::MOUNTAINS);
    this->sceneries->insert(Scenery::STRING_FENCES, Scenery::FENCES);
}

void Object_Handler::Populate_Brick_Map() {
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

void Object_Handler::Populate_Background_Map() {
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

bool Object_Handler::Question_Block_With_Mushroom(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Question_Block_With_Mushroom(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Question_Block_With_Coin(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Question_Block_With_Coin(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Hidden_Block_With_Coin(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Hidden_Block_With_Coin(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Hidden_Block_With_1up(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Hidden_Block_With_1up(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Brick_With_Mushroom(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Brick_With_Mushroom(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Brick_With_Vine(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Brick_With_Vine(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Brick_With_Star(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Brick_With_Star(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Brick_With_10_Coins(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Brick_With_10_Coins(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Brick_With_1up(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Brick_With_1up(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Underwater_Sideways_Pipe(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Underwater_Sideways_Pipe(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Used_Block(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Used_Block(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Trampoline(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int y = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->writerPlugin->Object_Trampoline(x, y)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Cannon(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; int height = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), height)) return false;
    if (!this->writerPlugin->Object_Cannon(x, y, height)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Island(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; int length = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), length)) return false;
    if (!this->writerPlugin->Object_Island(x, y, length)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Horizontal_Bricks(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; int length = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), length)) return false;
    if (!this->writerPlugin->Object_Horizontal_Bricks(x, y, length)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Horizontal_Blocks(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; int length = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), length)) return false;
    if (!this->writerPlugin->Object_Horizontal_Blocks(x, y, length)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Horizontal_Coins(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; int length = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), length)) return false;
    if (!this->writerPlugin->Object_Horizontal_Coins(x, y, length)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Vertical_Bricks(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; int height = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), height)) return false;
    if (!this->writerPlugin->Object_Vertical_Bricks(x, y, height)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Vertical_Blocks(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; int height = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), height)) return false;
    if (!this->writerPlugin->Object_Vertical_Blocks(x, y, height)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Corral(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; int height = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), height)) return false;
    if (!this->writerPlugin->Object_Coral(x, y, height)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Pipe(const QString &line, bool enterable, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; int height = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), height)) return false;
    if (!this->writerPlugin->Object_Pipe(x, y, height, enterable)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Hole(const QString &line, bool filledWithWater, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int length = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), length)) return false;
    if (!this->writerPlugin->Object_Hole(x, length, filledWithWater)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Bridge(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int yPlacement = 0; int length = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), yPlacement)) return false;
    if (!this->Parse_Num(elements.at(3), length)) return false;
    if (!this->writerPlugin->Object_Bridge(x, yPlacement, length)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Horizontal_Question_Blocks_With_Coins(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int yPlacement = 0; int length = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), yPlacement)) return false;
    if (!this->Parse_Num(elements.at(3), length)) return false;
    if (!this->writerPlugin->Object_Horizontal_Question_Blocks_With_Coins(x, yPlacement, length)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Page_Change(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int page = 0;
    if (!this->Parse_Num(elements.at(1), page)) return false;
    if (!this->writerPlugin->Object_Page_Change(page)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Reverse_L_Pipe(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Reverse_L_Pipe(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Flagpole(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Flagpole(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Castle(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Castle(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Big_Castle(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Big_Castle(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Axe(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Axe(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Axe_Rope(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Axe_Rope(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Bowser_Bridge(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Bowser_Bridge(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Scroll_Stop(const QString &line, bool warpZone, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Scroll_Stop(x, warpZone)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Toggle_Auto_Scroll(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Toggle_Auto_Scroll(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Flying_Cheep_Cheep_Spawner(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;

    //Write the object
    bool success = false;
    if (this->useRandomEnemies && Random::Get_Instance().Get_Num(1)) {
        Level_Attribute::Level_Attribute attribute = Level_Attribute::OVERWORLD;
        if (!this->writerPlugin->Header_Get_Current_Attribute(attribute)) return false;
        if (attribute == Level_Attribute::UNDERWATER) success = this->writerPlugin->Object_Swimming_Cheep_Cheep_Spawner(x);
        else success = this->writerPlugin->Object_Bullet_Bill_Spawner(x);
    } else success = this->writerPlugin->Object_Flying_Cheep_Cheep_Spawner(x);
    if (!success) errorCode = 3;
    return success;
}

bool Object_Handler::Swimming_Cheep_Cheep_Spawner(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Swimming_Cheep_Cheep_Spawner(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Bullet_Bill_Spawner(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;

    //Write the object
    bool success = false;
    if (this->useRandomEnemies && Random::Get_Instance().Get_Num(1)) success = this->writerPlugin->Object_Flying_Cheep_Cheep_Spawner(x);
    else success = this->writerPlugin->Object_Bullet_Bill_Spawner(x);
    if (!success) errorCode = 3;
    return success;
}

bool Object_Handler::Cancel_Spawner(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Cancel_Spawner(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Loop_Command(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Loop_Command(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Change_Brick_And_Scenery(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0;
    Brick::Brick brick = Brick::NO_BRICKS;
    Scenery::Scenery scenery = Scenery::NO_SCENERY;
    if (!this->Parse_Num(elements.at(1), x)) return false;

    //Parse the brick
    QMap<QString, Brick::Brick>::iterator brickIter = this->bricks->find(elements.at(2));
    if (brickIter == this->bricks->end()) return false; //not found
    brick = brickIter.value();

    //Parse the scenery
    QMap<QString, Scenery::Scenery>::iterator sceneryIter = this->sceneries->find(elements.at(3));
    if (sceneryIter == this->sceneries->end()) return false; //not found
    scenery = sceneryIter.value();

    if (!this->writerPlugin->Object_Change_Brick_And_Scenery(x, brick, scenery)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Change_Background(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; Background::Background background = Background::BLANK_BACKGROUND;
    if (!this->Parse_Num(elements.at(1), x)) return false;

    //Parse the background
    QMap<QString, Background::Background>::iterator iter = this->backgrounds->find(elements.at(2));
    if (iter == this->backgrounds->end()) return false; //not found
    background = iter.value();

    if (!this->writerPlugin->Object_Change_Background(x, background)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Lift_Rope(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Lift_Rope(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Balance_Lift_Vertical_Rope(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int length = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), length)) return false;
    if (!this->writerPlugin->Object_Balance_Lift_Vertical_Rope(x, length)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Balance_Lift_Horizontal_Rope(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int length = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), length)) return false;
    if (!this->writerPlugin->Object_Balance_Lift_Horizontal_Rope(x, length)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Steps(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int width = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), width)) return false;
    if (!this->writerPlugin->Object_Steps(x, width)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::End_Steps(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_End_Steps(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Tall_Reverse_L_Pipe(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; int yPlacement = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), yPlacement)) return false;
    if (!this->writerPlugin->Object_Tall_Reverse_L_Pipe(x, yPlacement)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Pipe_Wall(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Pipe_Wall(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Object_Handler::Nothing(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Object_Nothing(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}
