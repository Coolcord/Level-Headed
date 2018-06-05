#include "SMB1_Compliance_Parser.h"
#include "../Common_SMB1_Files/Object_Item_String.h"
#include "../Common_SMB1_Files/Enemy_Item_String.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include "../Common_SMB1_Files/Level_Type_String.h"
#include "../Common_SMB1_Files/Header_String.h"
#include "Object_Handler.h"
#include "Enemy_Handler.h"
#include "Header_Handler.h"
#include <assert.h>
#include <QStringList>
#include <QDebug>

SMB1_Compliance_Parser::SMB1_Compliance_Parser(SMB1_Writer_Interface *writerPlugin) : SMB1_Compliance_Map() {
    assert(this->objects);
    assert(this->enemies);
    assert(writerPlugin);
    this->writerPlugin = writerPlugin;

    //Set up the item handlers
    this->objectHandler = new Object_Handler(this->writerPlugin);
    this->enemyHandler = new Enemy_Handler(this->writerPlugin);
}

SMB1_Compliance_Parser::~SMB1_Compliance_Parser() {
    delete this->objectHandler;
    delete this->enemyHandler;
}

int SMB1_Compliance_Parser::Parse_Level(const QString &fileLocation) {
    int lineNum = 1;
    return this->Parse_Level(fileLocation, lineNum);
}

int SMB1_Compliance_Parser::Parse_Level(const QString &fileLocation, int &lineNum) {
    //Open the file for reading
    QFile file(fileLocation);
    if (!file.open(QFile::ReadOnly)) {
        return 1;
    }
    lineNum = 1;
    int errorCode = 2;

    //Parse the Header
    if (!this->Parse_Header(&file, lineNum, errorCode)) return errorCode;

    //Make sure that there are objects to parse
    if (file.atEnd()) return 2;

    //Parse all of the Objects and Enemies
    if (!this->Parse_Items(&file, lineNum, errorCode)) return errorCode;

    //Make sure everything was parsed
    if (!file.atEnd()) return 2;

    return 0;
}

bool SMB1_Compliance_Parser::Parse_Header(QFile *file, int &lineNum, int &errorCode) {
    assert(file);
    Header_Handler headerHandler(this->writerPlugin, file);
    if (!headerHandler.Parse_Header(lineNum, errorCode)) return false;

    return true;
}

bool SMB1_Compliance_Parser::Parse_Items(QFile *file, int &lineNum, int &errorCode) {
    assert(file);
    QByteArray line;

    //Read the Objects
    bool success = false;
    do {
        ++lineNum;
        line = file->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line.startsWith("===")) {
            success = true;
            break;
        } else {
            if (!this->Parse_Object(line, errorCode)) {
                return false;
            }
        }
    } while (!file->atEnd());

    //The seperator could not be found
    if (!success) return false; //TODO: Add an error here

    //Read the Enemies
    success = false;
    do {
        ++lineNum;
        line = file->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line.startsWith("===")) {
            success = true;
            break;
        } else {
            if (!this->Parse_Enemy(line, errorCode)) {
                return false;
            }
        }
    } while (!file->atEnd());

    //The seperator could not be found
    if (!success) return false; //TODO: Add an error here

    return true;
}

bool SMB1_Compliance_Parser::Parse_Object(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() < 2) return false; //line is invalid
    QString object = elements.at(0);
    QMap<QString, Object_Item::Object_Item>::iterator iter = this->objects->find(object);
    if (iter == this->objects->end()) return false; //not found
    switch (iter.value()) {
    case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
        return this->objectHandler->Question_Block_With_Mushroom(line, errorCode);
    case Object_Item::QUESTION_BLOCK_WITH_COIN:
        return this->objectHandler->Question_Block_With_Coin(line, errorCode);
    case Object_Item::HIDDEN_BLOCK_WITH_COIN:
        return this->objectHandler->Hidden_Block_With_Coin(line, errorCode);
    case Object_Item::HIDDEN_BLOCK_WITH_1UP:
        return this->objectHandler->Hidden_Block_With_1up(line, errorCode);
    case Object_Item::BRICK_WITH_MUSHROOM:
        return this->objectHandler->Brick_With_Mushroom(line, errorCode);
    case Object_Item::BRICK_WITH_VINE:
        return this->objectHandler->Brick_With_Vine(line, errorCode);
    case Object_Item::BRICK_WITH_STAR:
        return this->objectHandler->Brick_With_Star(line, errorCode);
    case Object_Item::BRICK_WITH_10_COINS:
        return this->objectHandler->Brick_With_10_Coins(line, errorCode);
    case Object_Item::BRICK_WITH_1UP:
        return this->objectHandler->Brick_With_1up(line, errorCode);
    case Object_Item::UNDERWATER_SIDEWAYS_PIPE:
        return this->objectHandler->Underwater_Sideways_Pipe(line, errorCode);
    case Object_Item::USED_BLOCK:
        return this->objectHandler->Used_Block(line, errorCode);
    case Object_Item::TRAMPOLINE:
        return this->objectHandler->Trampoline(line, errorCode);
    case Object_Item::CANNON:
        return this->objectHandler->Cannon(line, errorCode);
    case Object_Item::ISLAND:
        return this->objectHandler->Island(line, errorCode);
    case Object_Item::HORIZONTAL_BRICKS:
        return this->objectHandler->Horizontal_Bricks(line, errorCode);
    case Object_Item::HORIZONTAL_BLOCKS:
        return this->objectHandler->Horizontal_Blocks(line, errorCode);
    case Object_Item::HORIZONTAL_COINS:
        return this->objectHandler->Horizontal_Coins(line, errorCode);
    case Object_Item::VERTICAL_BRICKS:
        return this->objectHandler->Vertical_Bricks(line, errorCode);
    case Object_Item::VERTICAL_BLOCKS:
        return this->objectHandler->Vertical_Blocks(line, errorCode);
    case Object_Item::CORRAL:
        return this->objectHandler->Corral(line, errorCode);
    case Object_Item::PIPE:
        return this->objectHandler->Pipe(line, false, errorCode);
    case Object_Item::ENTERABLE_PIPE:
        return this->objectHandler->Pipe(line, true, errorCode);
    case Object_Item::HOLE:
        return this->objectHandler->Hole(line, false, errorCode);
    case Object_Item::HOLE_WITH_WATER:
        return this->objectHandler->Hole(line, true, errorCode);
    case Object_Item::BALANCE_LIFT_VERTICAL_ROPE:
        return this->objectHandler->Balance_Lift_Vertical_Rope(line, errorCode);
    case Object_Item::BRIDGE:
        return this->objectHandler->Bridge(line, errorCode);
    case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:
        return this->objectHandler->Horizontal_Question_Blocks_With_Coins(line, errorCode);
    case Object_Item::PAGE_CHANGE:
        return this->objectHandler->Page_Change(line, errorCode);
    case Object_Item::REVERSE_L_PIPE:
        return this->objectHandler->Reverse_L_Pipe(line, errorCode);
    case Object_Item::FLAGPOLE:
        return this->objectHandler->Flagpole(line, errorCode);
    case Object_Item::CASTLE:
        return this->objectHandler->Castle(line, errorCode);
    case Object_Item::BIG_CASTLE:
        return this->objectHandler->Big_Castle(line, errorCode);
    case Object_Item::AXE:
        return this->objectHandler->Axe(line, errorCode);
    case Object_Item::AXE_ROPE:
        return this->objectHandler->Axe_Rope(line, errorCode);
    case Object_Item::BOWSER_BRIDGE:
        return this->objectHandler->Bowser_Bridge(line, errorCode);
    case Object_Item::SCROLL_STOP:
        return this->objectHandler->Scroll_Stop(line, false, errorCode);
    case Object_Item::SCROLL_STOP_WARP_ZONE:
        return this->objectHandler->Scroll_Stop(line, true, errorCode);
    case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:
        return this->objectHandler->Flying_Cheep_Cheep_Spawner(line, errorCode);
    case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:
        return this->objectHandler->Swimming_Cheep_Cheep_Spawner(line, errorCode);
    case Object_Item::BULLET_BILL_SPAWNER:
        return this->objectHandler->Bullet_Bill_Spawner(line, errorCode);
    case Object_Item::CANCEL_SPAWNER:
        return this->objectHandler->Cancel_Spawner(line, errorCode);
    case Object_Item::LOOP_COMMAND:
        return this->objectHandler->Loop_Command(line, errorCode);
    case Object_Item::CHANGE_BRICK_AND_SCENERY:
        return this->objectHandler->Change_Brick_And_Scenery(line, errorCode);
    case Object_Item::CHANGE_BACKGROUND:
        return this->objectHandler->Change_Background(line, errorCode);
    case Object_Item::LIFT_ROPE:
        return this->objectHandler->Lift_Rope(line, errorCode);
    case Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE:
        return this->objectHandler->Balance_Lift_Horizontal_Rope(line, errorCode);
    case Object_Item::STEPS:
        return this->objectHandler->Steps(line, errorCode);
    case Object_Item::END_STEPS:
        return this->objectHandler->End_Steps(line, errorCode);
    case Object_Item::TALL_REVERSE_L_PIPE:
        return this->objectHandler->Tall_Reverse_L_Pipe(line, errorCode);
    case Object_Item::PIPE_WALL:
        return this->objectHandler->Pipe_Wall(line, errorCode);
    case Object_Item::NOTHING:
        return this->objectHandler->Nothing(line, errorCode);
    default:
        return false; //invalid object
    }
}

bool SMB1_Compliance_Parser::Parse_Enemy(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() < 2) return false; //line is invalid
    QString enemy = elements.at(0);
    QMap<QString, Enemy_Item::Enemy_Item>::iterator iter = this->enemies->find(enemy);
    if (iter == this->enemies->end()) return false; //not found
    switch (iter.value()) {
    case Enemy_Item::GREEN_KOOPA:           return this->enemyHandler->Green_Koopa(line, errorCode);
    case Enemy_Item::RED_KOOPA:             return this->enemyHandler->Red_Koopa(line, errorCode);
    case Enemy_Item::BUZZY_BEETLE:          return this->enemyHandler->Buzzy_Beetle(line, errorCode);
    case Enemy_Item::HAMMER_BRO:            return this->enemyHandler->Hammer_Bro(line, errorCode);
    case Enemy_Item::GOOMBA:                return this->enemyHandler->Goomba(line, errorCode);
    case Enemy_Item::BLOOPER:               return this->enemyHandler->Blooper(line, errorCode);
    case Enemy_Item::BULLET_BILL:           return this->enemyHandler->Bullet_Bill(line, errorCode);
    case Enemy_Item::GREEN_PARATROOPA:      return this->enemyHandler->Green_Paratroopa(line, errorCode);
    case Enemy_Item::RED_PARATROOPA:        return this->enemyHandler->Red_Paratroopa(line, errorCode);
    case Enemy_Item::GREEN_CHEEP_CHEEP:     return this->enemyHandler->Green_Cheep_Cheep(line, errorCode);
    case Enemy_Item::RED_CHEEP_CHEEP:       return this->enemyHandler->Red_Cheep_Cheep(line, errorCode);
    case Enemy_Item::PODOBOO:               return this->enemyHandler->Podoboo(line, errorCode);
    case Enemy_Item::PIRANA_PLANT:          return this->enemyHandler->Pirana_Plant(line, errorCode);
    case Enemy_Item::LAKITU:                return this->enemyHandler->Lakitu(line, errorCode);
    case Enemy_Item::SPINY:                 return this->enemyHandler->Spiny(line, errorCode);
    case Enemy_Item::BOWSER_FIRE_SPAWNER:   return this->enemyHandler->Bowser_Fire_Spawner(line, errorCode);
    case Enemy_Item::CHEEP_CHEEP_SPAWNER:   return this->enemyHandler->Swimming_Cheep_Cheep_Spawner(line, errorCode);
    case Enemy_Item::BULLET_BILL_SPAWNER:   return this->enemyHandler->Bullet_Bill_Spawner(line, errorCode);
    case Enemy_Item::FIRE_BAR:              return this->enemyHandler->Fire_Bar(line, errorCode);
    case Enemy_Item::LARGE_FIRE_BAR:        return this->enemyHandler->Large_Fire_Bar(line, errorCode);
    case Enemy_Item::LIFT:                  return this->enemyHandler->Lift(line, errorCode);
    case Enemy_Item::FALLING_LIFT:          return this->enemyHandler->Falling_Lift(line, errorCode);
    case Enemy_Item::BALANCE_LIFT:          return this->enemyHandler->Balance_Lift(line, errorCode);
    case Enemy_Item::SURFING_LIFT:          return this->enemyHandler->Surfing_Lift(line, errorCode);
    case Enemy_Item::LIFT_SPAWNER:          return this->enemyHandler->Lift_Spawner(line, errorCode);
    case Enemy_Item::BOWSER:                return this->enemyHandler->Bowser(line, errorCode);
    case Enemy_Item::WARP_ZONE:             return this->enemyHandler->Warp_Zone(line, errorCode);
    case Enemy_Item::PIPE_POINTER:          return this->enemyHandler->Pipe_Pointer(line, errorCode);
    case Enemy_Item::TOAD:                  return this->enemyHandler->Toad(line, errorCode);
    case Enemy_Item::GOOMBA_GROUP:          return this->enemyHandler->Goomba_Group(line, errorCode);
    case Enemy_Item::KOOPA_GROUP:           return this->enemyHandler->Koopa_Group(line, errorCode);
    case Enemy_Item::PAGE_CHANGE:           return this->enemyHandler->Page_Change(line, errorCode);
    default:                                return false; //invalid enemy
    }
}
