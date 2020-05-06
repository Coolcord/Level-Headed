#include "Level_Script_Parser.h"
#include "../Common_SMB1_Files/Header_String.h"
#include "../Common_SMB1_Files/Level_Attribute_String.h"
#include "../Common_SMB1_Files/Brick_String.h"
#include "../Common_SMB1_Files/Background_String.h"
#include "../Common_SMB1_Files/Enemy_Item_String.h"
#include "../Common_SMB1_Files/Scenery_String.h"
#include "../Common_SMB1_Files/Level_Compliment_String.h"
#include <QTextStream>
#include <assert.h>

Level_Script_Parser::Level_Script_Parser(SMB1_Compliance_Parser_Arguments *args) : SMB1_Compliance_Map() {
    assert(args);
    this->args = args;
}

Level_Script_Parser::~Level_Script_Parser() {
    delete this->stream;
    this->stream = nullptr;
    this->args = nullptr;
}

bool Level_Script_Parser::Parse_Level_Script() {
    if (!this->stream) this->stream = new QTextStream(this->args->levelScriptBytes);
    if (!this->Parse_Header()) return false;
    return this->Parse_Items();
}

bool Level_Script_Parser::Parse_Header() {
    this->args->lineNum = 1;
    QStringList elements;

    //Check the header
    QString line;
    line = this->stream->readLine().trimmed();
    if (line != Header::STRING_NAME) return false;

    //Notes Section -- Look for 2 seperators
    for (int i = 0; i < 2; ++i) {
        do {
            ++this->args->lineNum;
            line = this->stream->readLine().trimmed();
            if (line.isEmpty()) continue;
            if (this->stream->atEnd()) return false; //TODO: Handle this error
        } while (!line.startsWith("==="));
    }

    //Attribute
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_ATTRIBUTE + ":");
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_ATTRIBUTE + ":") return false;
    QMap<QString, Level_Attribute::Level_Attribute>::iterator attributeIter = this->attributesMap->find(elements.at(1));
    if (attributeIter == this->attributesMap->end()) return false; //not found
    this->args->levelAttribute = attributeIter.value();

    //Starting Position
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_STARTING_POSITION + ":");
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_STARTING_POSITION + ":") return false;
    //Handle Autowalk if the user set it
    if (elements.at(1) == Level_Attribute::STRING_OVERWORLD_WALKING) {
        this->args->autowalk = true;
        this->args->levelAttribute = Level_Attribute::OVERWORLD;
    } else {
        //Remember that starting positions and attributes are very similar!
        attributeIter = this->attributesMap->find(elements.at(1));
        if (attributeIter == this->attributesMap->end()) return false; //not found
        this->args->autowalk = false;
        this->args->levelAttribute = attributeIter.value();
    }

    //Brick
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_BRICK + ":");
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_BRICK + ":") return false;
    QMap<QString, Brick::Brick>::iterator brickIter = this->bricksMap->find(elements.at(1));
    if (brickIter == this->bricksMap->end()) return false; //not found
    this->args->headerBrick = brickIter.value();

    //Background
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_BACKGROUND + ":");
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_BACKGROUND + ":") return false;
    QMap<QString, Background::Background>::iterator backgroundIter = this->backgroundsMap->find(elements.at(1));
    if (backgroundIter == this->backgroundsMap->end()) return false; //not found
    this->args->headerBackground = backgroundIter.value();

    //Scenery
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_SCENERY + ":");
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_SCENERY + ":") return false;
    QMap<QString, Scenery::Scenery>::iterator sceneryIter = this->sceneriesMap->find(elements.at(1));
    if (sceneryIter == this->sceneriesMap->end()) return false; //not found
    this->args->headerScenery = sceneryIter.value();

    //Compliment
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_COMPLIMENT + ":");
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_COMPLIMENT + ":") return false;
    QMap<QString, Level_Compliment::Level_Compliment>::iterator complimentIter = this->complimentsMap->find(elements.at(1));
    if (complimentIter == this->complimentsMap->end()) return false; //not found
    this->args->levelCompliment = complimentIter.value();

    //Time
    line = this->Parse_Through_Comments_Until_First_Word(Header::STRING_TIME + ":");
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_TIME + ":") return false;
    bool valid = false;
    this->args->time = elements.at(1).toInt(&valid);
    if (!valid) return false;

    //Midpoint
    line = this->Parse_Through_Comments_Until_First_Word_Or_Seperator(Header::STRING_MIDPOINT + ":");
    if (line.startsWith("===")) {
        this->args->midpoint = -1;
    } else {
        elements = line.split(' ');
        if (elements.size() != 2) return false;
        if (elements.at(0) != Header::STRING_MIDPOINT + ":") return false;
        valid = false;
        this->args->midpoint = elements.at(1).toInt(&valid);
        if (!valid) return false;
        line = this->Parse_Through_Comments_Until_Seperator();
        if (!line.startsWith("===")) return false;
    }

    return true;
}

bool Level_Script_Parser::Parse_Items() {
    QString line = QString();

    //Read the Objects
    bool success = false;
    do {
        ++this->args->lineNum;
        line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line.startsWith("===")) {
            success = true;
            break;
        } else {
            if (!this->Parse_Object(line)) {
                return false;
            }
        }
    } while (!this->stream->atEnd());

    //The seperator could not be found
    if (!success) return false; //TODO: Add an error here

    //Read the Enemies
    success = false;
    do {
        ++this->args->lineNum;
        line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line.startsWith("===")) {
            success = true;
            break;
        } else {
            if (!this->Parse_Enemy(line)) {
                return false;
            }
        }
    } while (!this->stream->atEnd());
    return success; //TODO: Add an error here if success is false (The seperator could not be found)
}

bool Level_Script_Parser::Parse_Object(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() < 2) return false; //line is invalid
    QString object = elements.at(0);

    //Set up variables and parse number up front
    QString var1, var2, var3;
    if (elements.size() >= 2) var1 = elements.at(1);
    if (elements.size() >= 3) var2 = elements.at(2);
    if (elements.size() >= 4) var3 = elements.at(3);
    int num1 = 0, num2 = 0, num3 = 0;
    bool isNum1Valid = false, isNum2Valid = false, isNum3Valid = false;
    num1 = var1.toInt(&isNum1Valid); num2 = var2.toInt(&isNum2Valid); num3 = var3.toInt(&isNum3Valid);
    QMap<QString, Brick::Brick>::iterator brickIter = this->bricksMap->end();
    QMap<QString, Scenery::Scenery>::iterator sceneryIter = this->sceneriesMap->end();
    QMap<QString, Background::Background>::iterator backgroundIter = this->backgroundsMap->end();

    //Parse the object
    QMap<QString, Object_Item::Object_Item>::iterator iter = this->objectsMap->find(object);
    if (iter == this->objectsMap->end()) return false; //not found
    switch (iter.value()) {
    case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Question_Block_With_Mushroom(num1, num2);
    case Object_Item::QUESTION_BLOCK_WITH_COIN:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Question_Block_With_Coin(num1, num2);
    case Object_Item::HIDDEN_BLOCK_WITH_COIN:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Hidden_Block_With_Coin(num1, num2);
    case Object_Item::HIDDEN_BLOCK_WITH_1UP:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Hidden_Block_With_1up(num1, num2);
    case Object_Item::BRICK_WITH_MUSHROOM:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Brick_With_Mushroom(num1, num2);
    case Object_Item::BRICK_WITH_VINE:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Brick_With_Vine_Without_Pointer(num1, num2);
    case Object_Item::BRICK_WITH_STAR:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Brick_With_Star(num1, num2);
    case Object_Item::BRICK_WITH_10_COINS:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Brick_With_10_Coins(num1, num2);
    case Object_Item::BRICK_WITH_1UP:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Brick_With_1up(num1, num2);
    case Object_Item::UNDERWATER_SIDEWAYS_PIPE:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Underwater_Sideways_Pipe_Without_Pointer(num1, num2);
    case Object_Item::USED_BLOCK:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Used_Block(num1, num2);
    case Object_Item::TRAMPOLINE:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Trampoline(num1, num2);
    case Object_Item::BULLET_BILL_TURRET:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        if (this->args->levelCompliment != Level_Compliment::BULLET_BILL_TURRETS) return false;
        return this->args->objectBuffer->Bullet_Bill_Turret(num1, num2, num3);
    case Object_Item::ISLAND:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        if (this->args->levelCompliment == Level_Compliment::BULLET_BILL_TURRETS) return false;
        return this->args->objectBuffer->Island(num1, num2, num3);
    case Object_Item::HORIZONTAL_BRICKS:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        return this->args->objectBuffer->Horizontal_Bricks(num1, num2, num3);
    case Object_Item::HORIZONTAL_BLOCKS:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        return this->args->objectBuffer->Horizontal_Blocks(num1, num2, num3);
    case Object_Item::HORIZONTAL_COINS:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        return this->args->objectBuffer->Horizontal_Coins(num1, num2, num3);
    case Object_Item::VERTICAL_BRICKS:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        if (this->args->levelAttribute == Level_Attribute::UNDERWATER) return false;
        return this->args->objectBuffer->Vertical_Bricks(num1, num2, num3);
    case Object_Item::VERTICAL_BLOCKS:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        return this->args->objectBuffer->Vertical_Blocks(num1, num2, num3);
    case Object_Item::CORAL:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        if (this->args->levelAttribute != Level_Attribute::UNDERWATER) return false;
        return this->args->objectBuffer->Coral(num1, num2, num3);
    case Object_Item::PIPE:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        return this->args->objectBuffer->Pipe(num1, num2, num3);
    case Object_Item::ENTERABLE_PIPE:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        return this->args->objectBuffer->Enterable_Pipe_Without_Pointer(num1, num2, num3);
    case Object_Item::HOLE:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Hole(num1, num2, false);
    case Object_Item::HOLE_WITH_WATER:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Hole(num1, num2, true);
    case Object_Item::BALANCE_LIFT_VERTICAL_ROPE:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Balance_Lift_Vertical_Rope(num1, num2);
    case Object_Item::BRIDGE:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        return this->args->objectBuffer->Bridge(num1, num2, num3);
    case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        return this->args->objectBuffer->Horizontal_Question_Blocks_With_Coins(num1, num2, num3);
    case Object_Item::PAGE_CHANGE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Page_Change(num1);
    case Object_Item::REVERSE_L_PIPE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Reverse_L_Pipe_Without_Pointer(num1);
    case Object_Item::FLAGPOLE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Flagpole(num1);
    case Object_Item::CASTLE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Castle(num1);
    case Object_Item::BIG_CASTLE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Big_Castle(num1);
    case Object_Item::AXE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Axe(num1);
    case Object_Item::AXE_ROPE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Axe_Rope(num1);
    case Object_Item::BOWSER_BRIDGE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Bowser_Bridge(num1);
    case Object_Item::SCROLL_STOP:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Scroll_Stop(num1, false);
    case Object_Item::SCROLL_STOP_WARP_ZONE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Scroll_Stop(num1, true);
    case Object_Item::TOGGLE_AUTO_SCROLL:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Toggle_Auto_Scroll(num1);
    case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Flying_Cheep_Cheep_Spawner(num1);
    case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:
        if (elements.size() != 2 || !isNum1Valid) return false;
        if (this->args->levelAttribute != Level_Attribute::UNDERWATER) return false;
        return this->args->objectBuffer->Swimming_Cheep_Cheep_Spawner(num1);
    case Object_Item::BULLET_BILL_SPAWNER:
        if (elements.size() != 2 || !isNum1Valid) return false;
        if (this->args->levelAttribute == Level_Attribute::UNDERWATER) return false;
        return this->args->objectBuffer->Bullet_Bill_Spawner(num1);
    case Object_Item::CANCEL_SPAWNER:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Cancel_Spawner(num1);
    case Object_Item::LOOP_COMMAND:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Loop_Command(num1);
    case Object_Item::CHANGE_BRICK_AND_SCENERY:
        if (elements.size() != 4 || !isNum1Valid) return false;
        brickIter = this->bricksMap->find(var2);
        if (brickIter == this->bricksMap->end()) return false; //not found
        sceneryIter = this->sceneriesMap->find(var3);
        if (sceneryIter == this->sceneriesMap->end()) return false; //not found
        return this->args->objectBuffer->Change_Brick_And_Scenery(num1, brickIter.value(), sceneryIter.value());
    case Object_Item::CHANGE_BACKGROUND:
        if (elements.size() != 3 || !isNum1Valid) return false;
        backgroundIter = this->backgroundsMap->find(var2);
        if (backgroundIter == this->backgroundsMap->end()) return false; //not found
        return this->args->objectBuffer->Change_Background(num1, backgroundIter.value());
    case Object_Item::LIFT_ROPE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Lift_Rope(num1);
    case Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Balance_Lift_Horizontal_Rope(num1, num2);
    case Object_Item::STEPS:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Steps(num1, num2);
    case Object_Item::END_STEPS:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->End_Steps(num1);
    case Object_Item::TALL_REVERSE_L_PIPE:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        return this->args->objectBuffer->Tall_Reverse_L_Pipe_Without_Pointer(num1, num2);
    case Object_Item::PIPE_WALL:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->objectBuffer->Pipe_Wall(num1);
    case Object_Item::NOTHING:
        if (elements.size() != 1) return false;
        return this->args->objectBuffer->Nothing();
    }
    assert(false); return false;
}

bool Level_Script_Parser::Parse_Enemy(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() < 2) return false; //line is invalid
    QString enemy = elements.at(0);

    //Set up variables and parse number up front
    QString var1, var2, var3, var4, var5;
    if (elements.size() >= 2) var1 = elements.at(1);
    if (elements.size() >= 3) var2 = elements.at(2);
    if (elements.size() >= 4) var3 = elements.at(3);
    if (elements.size() >= 5) var4 = elements.at(4);
    if (elements.size() >= 6) var5 = elements.at(5);
    int num1 = 0, num2 = 0, num3 = 0;
    QMap<QString, Level::Level>::iterator levelIter = this->levelsMap->end();
    bool bit1 = false, bit2 = false, bit3 = false;
    bool isNum1Valid = false, isNum2Valid = false, isNum3Valid = false;
    num1 = var1.toInt(&isNum1Valid); num2 = var2.toInt(&isNum2Valid); num3 = var3.toInt(&isNum3Valid);

    QMap<QString, Enemy_Item::Enemy_Item>::iterator iter = this->enemiesMap->find(enemy);
    if (iter == this->enemiesMap->end()) return false; //not found
    switch (iter.value()) {
    case Enemy_Item::GREEN_KOOPA:
        if (elements.size() != 5 || !isNum1Valid || !isNum2Valid) return false;
        if (var3 == Enemy_Item::STRING_MOVING) bit1 = true;
        else if (var3 == Enemy_Item::STRING_STOPPED) bit1 = false;
        else return false; //invalid movement type
        if (!this->Parse_Difficulty(var4, bit2)) return false;
        return this->args->enemyBuffer->Green_Koopa(num1, num2, bit1, bit2);
    case Enemy_Item::RED_KOOPA:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Red_Koopa(num1, num2, bit1);
    case Enemy_Item::BUZZY_BEETLE:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Buzzy_Beetle(num1, num2, bit1);
    case Enemy_Item::HAMMER_BRO:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Hammer_Bro(num1, num2, bit1);
    case Enemy_Item::GOOMBA:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Goomba(num1, num2, bit1);
    case Enemy_Item::BLOOPER:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Blooper(num1, num2, bit1);
    case Enemy_Item::BULLET_BILL:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Bullet_Bill(num1, num2, bit1);
    case Enemy_Item::GREEN_PARATROOPA:
        if (elements.size() != 5 || !isNum1Valid || !isNum2Valid) return false;
        if (var3 == Enemy_Item::STRING_LEAPING) {
            bit1 = true; //moving
            bit2 = true; //leaping
        } else if (var3 == Enemy_Item::STRING_FLYING) {
            bit1 = true; //moving
            bit2 = false; //leaping
        } else if (var3 == Enemy_Item::STRING_STOPPED) {
            bit1 = false; //moving
            bit2 = false; //leaping
        } else {
            return false; //invalid movement type
        }
        if (!this->Parse_Difficulty(var4, bit3)) return false;
        return this->args->enemyBuffer->Green_Paratroopa(num1, num2, bit1, bit2, bit3);
    case Enemy_Item::RED_PARATROOPA:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Red_Koopa(num1, num2, bit1);
    case Enemy_Item::GREEN_CHEEP_CHEEP:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Green_Cheep_Cheep(num1, num2, bit1);
    case Enemy_Item::RED_CHEEP_CHEEP:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Red_Cheep_Cheep(num1, num2, bit1);
    case Enemy_Item::PODOBOO:
        if (elements.size() != 3 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var2, bit1)) return false;
        return this->args->enemyBuffer->Podoboo(num1, bit1);
    case Enemy_Item::PIRANHA_PLANT:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Piranha_Plant(num1, num2, bit1);
    case Enemy_Item::LAKITU:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Lakitu(num1, num2, bit1);
    case Enemy_Item::SPINY:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Spiny(num1, num2, bit1);
    case Enemy_Item::BOWSER_FIRE_SPAWNER:
        if (elements.size() != 3 || !isNum1Valid) return false;
        if (!this->Parse_Difficulty(var2, bit1)) return false;
        return this->args->enemyBuffer->Bowser_Fire_Spawner(num1, bit1);
    case Enemy_Item::CHEEP_CHEEP_SPAWNER:
        if (elements.size() != 4 || !isNum1Valid) return false;
        if (var2 == Enemy_Item::STRING_LEAPING) bit1 = true;
        else if (var2 == Enemy_Item::STRING_FLYING) bit1 = false;
        else return false; //invalid movement type
        if (!this->Parse_Difficulty(var3, bit2)) return false;
        if (!bit1 && this->args->levelAttribute != Level_Attribute::UNDERWATER) return false;
        return this->args->enemyBuffer->Swimming_Cheep_Cheep_Spawner(num1, bit1, bit2);
    case Enemy_Item::BULLET_BILL_SPAWNER:
        if (elements.size() != 3 || !isNum1Valid) return false;
        if (this->args->levelAttribute == Level_Attribute::UNDERWATER) return false;
        if (!this->Parse_Difficulty(var2, bit1)) return false;
        return this->args->enemyBuffer->Bullet_Bill_Spawner(num1, bit1);
    case Enemy_Item::FIRE_BAR:
        if (elements.size() != 6 || !isNum1Valid || !isNum2Valid) return false;
        if (var3 == Enemy_Item::STRING_CLOCKWISE) bit1 = true;
        else if (var3 == Enemy_Item::STRING_COUNTER_CLOCKWISE) bit1 = false;
        else return false; //invalid direction
        if (var4 == Enemy_Item::STRING_SLOW) bit2 = false;
        else if (var4 == Enemy_Item::STRING_FAST) bit2 = true;
        else return false; //invalid speed
        if (!this->Parse_Difficulty(var5, bit3)) return false;
        return this->args->enemyBuffer->Fire_Bar(num1, num2, bit1, bit2, bit3);
    case Enemy_Item::LARGE_FIRE_BAR:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Large_Fire_Bar(num1, num2, bit1);
    case Enemy_Item::LIFT:
        if (elements.size() != 5 || !isNum1Valid || !isNum2Valid) return false;
        if (var3 == Enemy_Item::STRING_VERTICAL) bit1 = true;
        else if (var3 == Enemy_Item::STRING_HORIZONTAL) bit1 = false;
        else return false; //invalid movement type
        if (!this->Parse_Difficulty(var4, bit2)) return false;
        return this->args->enemyBuffer->Lift(num1, num2, bit1, bit2);
    case Enemy_Item::FALLING_LIFT:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Falling_Lift(num1, num2, bit1);
    case Enemy_Item::BALANCE_LIFT:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Balance_Lift(num1, num2, bit1);
    case Enemy_Item::SURFING_LIFT:
        if (elements.size() != 4 || !isNum1Valid || !isNum2Valid) return false;
        if (!this->Parse_Difficulty(var3, bit1)) return false;
        return this->args->enemyBuffer->Surfing_Lift(num1, num2, bit1);
    case Enemy_Item::LIFT_SPAWNER:
        if (elements.size() != 6 || !isNum1Valid || !isNum2Valid) return false;
        if (var3 == Enemy_Item::STRING_UP) bit1 = true;
        else if (var3 == Enemy_Item::STRING_DOWN) bit1 = false;
        else return false; //invalid direction
        if (var4 == Enemy_Item::STRING_LARGE) bit2 = false;
        else if (var4 == Enemy_Item::STRING_SMALL) bit2 = true;
        else return false; //invalid size
        if (!this->Parse_Difficulty(var5, bit3)) return false;
        return this->args->enemyBuffer->Lift_Spawner(num1, num2, bit1, bit2, bit3);
    case Enemy_Item::BOWSER:
        if (elements.size() != 3 || !isNum1Valid) return false;
        if (!this->Parse_Difficulty(var2, bit1)) return false;
        return this->args->enemyBuffer->Bowser(num1, bit1);
    case Enemy_Item::WARP_ZONE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->enemyBuffer->Warp_Zone(num1);
    case Enemy_Item::PIPE_POINTER:
        if (elements.size() != 5 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        levelIter = this->levelsMap->find(var3);
        if (levelIter == this->levelsMap->end()) return false; //not found
        return this->args->enemyBuffer->Pipe_Pointer(num1, levelIter.value(), num2, num3);
    case Enemy_Item::TOAD:
        if (elements.size() != 3 || !isNum1Valid) return false;
        if (!this->Parse_Difficulty(var2, bit1)) return false;
        return this->args->enemyBuffer->Toad(num1, bit1);
    case Enemy_Item::GOOMBA_GROUP:
        if (elements.size() != 5 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        if (!this->Parse_Difficulty(var4, bit1)) return false;
        return this->args->enemyBuffer->Goomba_Group(num1, num2, num3, bit1);
    case Enemy_Item::KOOPA_GROUP:
        if (elements.size() != 5 || !isNum1Valid || !isNum2Valid || !isNum3Valid) return false;
        if (!this->Parse_Difficulty(var4, bit1)) return false;
        return this->args->enemyBuffer->Koopa_Group(num1, num2, num3, bit1);
    case Enemy_Item::PAGE_CHANGE:
        if (elements.size() != 2 || !isNum1Valid) return false;
        return this->args->enemyBuffer->Page_Change(num1);
    case Enemy_Item::NOTHING:
        return false; //not supported
    }
    assert(false); return false;
}

bool Level_Script_Parser::Parse_Difficulty(const QString &value, bool &onlyHardMode) {
    if (value == Enemy_Item::STRING_NORMAL) {
        onlyHardMode = false;
        return true;
    } else if (value == Enemy_Item::STRING_HARD) {
        onlyHardMode = true;
        return true;
    } else {
        return false; //unable to read difficulty string
    }
}

QString Level_Script_Parser::Parse_Through_Comments_Until_First_Word(const QString &word) {
    do {
        ++this->args->lineNum;
        QString line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        QStringList elements = line.split(' ');
        if (elements.at(0) == word) return line;
    } while (!this->stream->atEnd());
    return QString();
}

QString Level_Script_Parser::Parse_Through_Comments_Until_First_Word_Or_Seperator(const QString &word) {
    do {
        ++this->args->lineNum;
        QString line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line.startsWith("===")) return line;
        QStringList elements = line.split(' ');
        if (elements.at(0) == word) return line;
    } while (!this->stream->atEnd());
    return QString();
}

QString Level_Script_Parser::Parse_Through_Comments_Until_Seperator() {
    do {
        ++this->args->lineNum;
        QString line = this->stream->readLine();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line.startsWith("===")) return line;
    } while (!this->stream->atEnd());
    return QString();
}
