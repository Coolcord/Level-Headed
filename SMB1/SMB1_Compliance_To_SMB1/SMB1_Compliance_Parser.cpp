#include "SMB1_Compliance_Parser.h"
#include "../Common SMB1 Files/Object_Item_String.h"
#include "../Common SMB1 Files/Enemy_Item_String.h"
#include "../Common SMB1 Files/Level_Type.h"
#include "../Common SMB1 Files/Level_Type_String.h"
#include "../Common SMB1 Files/Header_String.h"
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

bool SMB1_Compliance_Parser::Parse_Level(const QString &fileLocation) {
    //Open the file for reading
    QFile file(fileLocation);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Unable to open file";
        return false;
    }

    //Parse the Header
    if (!this->Parse_Header(&file)) {
        qDebug() << "Unable to parse the header";
        return false;
    }

    if (file.atEnd()) {
        qDebug() << "There are no objects to parse!";
        return false;
    }

    //Parse all of the Objects
    if (!this->Parse_Items(&file)) {
        qDebug() << "Failed to parse an object";
        return false;
    }

    //Make sure everything was parsed
    if (!file.atEnd()) return false;

    return true;
}

bool SMB1_Compliance_Parser::Parse_Header(QFile *file) {
    assert(file);
    Header_Handler headerHandler(this->writerPlugin, file);
    int lineNum = 1; //TODO: Make use of this later
    if (!headerHandler.Parse_Header(lineNum)) return false;

    return true;
}

bool SMB1_Compliance_Parser::Parse_Items(QFile *file) {
    assert(file);
    QByteArray line;

    //Read the Objects
    bool success = false;
    do {
        line = file->readLine();
        line.chop(1); //remove the new line character
        QList<QByteArray> elements = line.split(' ');
        if (elements.at(0) == "O:") {
            if (!this->Parse_Object(line)) {
                return false;
            }
        } else if (line == Level_Type::STRING_BREAK) {
            success = true;
            break;
        } else {
            return false; //line is invalid
        }
    } while (line != NULL && !file->atEnd());

    //The seperator could not be found
    if (!success) return false;

    //Read the Enemies
    success = false;
    do {
        line = file->readLine();
        line.chop(1); //remove the new line character
        QList<QByteArray> elements = line.split(' ');
        if (elements.at(0) == "E:") {
            if (!this->Parse_Enemy(line)) {
                return false;
            }
        } else if (line == Level_Type::STRING_BREAK || line + "=" == Level_Type::STRING_BREAK) {
            success = true;
            break;
        } else {
            return false; //line is invalid
        }
    } while (line != NULL && !file->atEnd());

    //The seperator could not be found
    if (!success) return false;

    return true;
}

bool SMB1_Compliance_Parser::Parse_Object(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() < 2) return false; //line is invalid
    QString object = elements.at(1);
    QMap<QString, Object_Item::Object_Item>::iterator iter = this->objects->find(object);
    if (iter == this->objects->end()) return false; //not found
    switch (iter.value()) {
    case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
        return this->objectHandler->Question_Block_With_Mushroom(line);
    case Object_Item::QUESTION_BLOCK_WITH_COIN:
        return this->objectHandler->Question_Block_With_Coin(line);
    case Object_Item::HIDDEN_BLOCK_WITH_COIN:
        return this->objectHandler->Hidden_Block_With_Coin(line);
    case Object_Item::HIDDEN_BLOCK_WITH_1UP:
        return this->objectHandler->Hidden_Block_With_1up(line);
    case Object_Item::BRICK_WITH_MUSHROOM:
        return this->objectHandler->Brick_With_Mushroom(line);
    case Object_Item::BRICK_WITH_VINE:
        return this->objectHandler->Brick_With_Vine(line);
    case Object_Item::BRICK_WITH_STAR:
        return this->objectHandler->Brick_With_Star(line);
    case Object_Item::BRICK_WITH_10_COINS:
        return this->objectHandler->Brick_With_10_Coins(line);
    case Object_Item::BRICK_WITH_1UP:
        return this->objectHandler->Brick_With_1up(line);
    case Object_Item::UNDERWATER_SIDEWAYS_PIPE:
        return this->objectHandler->Underwater_Sideways_Pipe(line);
    case Object_Item::USED_BLOCK:
        return this->objectHandler->Used_Block(line);
    case Object_Item::TRAMPOLINE:
        return this->objectHandler->Trampoline(line);
    case Object_Item::CANNON:
        return this->objectHandler->Cannon(line);
    case Object_Item::ISLAND:
        return this->objectHandler->Island(line);
    case Object_Item::HORIZONTAL_BRICKS:
        return this->objectHandler->Horizontal_Bricks(line);
    case Object_Item::HORIZONTAL_BLOCKS:
        return this->objectHandler->Horizontal_Blocks(line);
    case Object_Item::HORIZONTAL_COINS:
        return this->objectHandler->Horizontal_Coins(line);
    case Object_Item::VERTICAL_BRICKS:
        return this->objectHandler->Vertical_Bricks(line);
    case Object_Item::VERTICAL_BLOCKS:
        return this->objectHandler->Vertical_Blocks(line);
    case Object_Item::PIPE:
        return this->objectHandler->Pipe(line, false);
    case Object_Item::ENTERABLE_PIPE:
        return this->objectHandler->Pipe(line, true);
    case Object_Item::HOLE:
        return this->objectHandler->Hole(line, false);
    case Object_Item::HOLE_WITH_WATER:
        return this->objectHandler->Hole(line, true);
    case Object_Item::BALANCE_ROPE:
        return this->objectHandler->Balance_Rope(line);
    case Object_Item::BRIDGE:
        return this->objectHandler->Bridge(line);
    case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:
        return this->objectHandler->Horizontal_Question_Blocks_With_Coins(line);
    case Object_Item::PAGE_CHANGE:
        return this->objectHandler->Page_Change(line);
    case Object_Item::REVERSE_L_PIPE:
        return this->objectHandler->Reverse_L_Pipe(line);
    case Object_Item::FLAGPOLE:
        return this->objectHandler->Flagpole(line);
    case Object_Item::CASTLE:
        return this->objectHandler->Castle(line);
    case Object_Item::BIG_CASTLE:
        return this->objectHandler->Big_Castle(line);
    case Object_Item::AXE:
        return this->objectHandler->Axe(line);
    case Object_Item::AXE_ROPE:
        return this->objectHandler->Axe_Rope(line);
    case Object_Item::BOWSER_BRIDGE:
        return this->objectHandler->Bowser_Bridge(line);
    case Object_Item::SCROLL_STOP:
        return this->objectHandler->Scroll_Stop(line, false);
    case Object_Item::SCROLL_STOP_WARP_ZONE:
        return this->objectHandler->Scroll_Stop(line, true);
    case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:
        return this->objectHandler->Flying_Cheep_Cheep_Spawner(line);
    case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:
        return this->objectHandler->Swimming_Cheep_Cheep_Spawner(line);
    case Object_Item::BULLET_BILL_SPAWNER:
        return this->objectHandler->Bullet_Bill_Spawner(line);
    case Object_Item::CANCEL_SPAWNER:
        return this->objectHandler->Cancel_Spawner(line);
    case Object_Item::LOOP_COMMAND:
        return this->objectHandler->Loop_Command(line);
    case Object_Item::CHANGE_BRICK_AND_SCENERY:
        return this->objectHandler->Change_Brick_And_Scenery(line);
    case Object_Item::CHANGE_BACKGROUND:
        return this->objectHandler->Change_Background(line);
    case Object_Item::LIFT_ROPE:
        return this->objectHandler->Lift_Rope(line);
    case Object_Item::BALANCE_LIFT_ROPE:
        return this->objectHandler->Balance_Lift_Rope(line);
    case Object_Item::STEPS:
        return this->objectHandler->Steps(line);
    case Object_Item::END_STEPS:
        return this->objectHandler->End_Steps(line);
    case Object_Item::TALL_REVERSE_L_PIPE:
        return this->objectHandler->Tall_Reverse_L_Pipe(line);
    case Object_Item::PIPE_WALL:
        return this->objectHandler->Pipe_Wall(line);
    case Object_Item::NOTHING:
        return this->objectHandler->Nothing(line);
    default:
        return false; //invalid object
    }
}

bool SMB1_Compliance_Parser::Parse_Enemy(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() < 2) return false; //line is invalid
    QString enemy = elements.at(1);
    QMap<QString, Enemy_Item::Enemy_Item>::iterator iter = this->enemies->find(enemy);
    if (iter == this->enemies->end()) return false; //not found
    switch (iter.value()) {
    case Enemy_Item::GREEN_KOOPA:           return this->enemyHandler->Green_Koopa(line);
    case Enemy_Item::RED_KOOPA:             return this->enemyHandler->Red_Koopa(line);
    case Enemy_Item::BUZZY_BEETLE:          return this->enemyHandler->Buzzy_Beetle(line);
    case Enemy_Item::HAMMER_BRO:            return this->enemyHandler->Hammer_Bro(line);
    case Enemy_Item::GOOMBA:                return this->enemyHandler->Goomba(line);
    case Enemy_Item::BLOOPER:               return this->enemyHandler->Blooper(line);
    case Enemy_Item::BULLET_BILL:           return this->enemyHandler->Bullet_Bill(line);
    case Enemy_Item::GREEN_PARATROOPA:      return this->enemyHandler->Green_Paratroopa(line);
    case Enemy_Item::RED_PARATROOPA:        return this->enemyHandler->Red_Paratroopa(line);
    case Enemy_Item::GREEN_CHEEP_CHEEP:     return this->enemyHandler->Green_Cheep_Cheep(line);
    case Enemy_Item::RED_CHEEP_CHEEP:       return this->enemyHandler->Red_Cheep_Cheep(line);
    case Enemy_Item::PODOBOO:               return this->enemyHandler->Podoboo(line);
    case Enemy_Item::PIRANA_PLANT:          return this->enemyHandler->Pirana_Plant(line);
    case Enemy_Item::LAKITU:                return this->enemyHandler->Lakitu(line);
    case Enemy_Item::SPINY:                 return this->enemyHandler->Spiny(line);
    case Enemy_Item::BOWSER_FIRE_SPAWNER:   return this->enemyHandler->Bowser_Fire_Spawner(line);
    case Enemy_Item::CHEEP_CHEEP_SPAWNER:   return this->enemyHandler->Cheep_Cheep_Spawner(line);
    case Enemy_Item::BULLET_BILL_SPAWNER:   return this->enemyHandler->Bullet_Bill_Spawner(line);
    case Enemy_Item::FIRE_BAR:              return this->enemyHandler->Fire_Bar(line);
    case Enemy_Item::LARGE_FIRE_BAR:        return this->enemyHandler->Large_Fire_Bar(line);
    case Enemy_Item::LIFT:                  return this->enemyHandler->Lift(line);
    case Enemy_Item::FALLING_LIFT:          return this->enemyHandler->Falling_Lift(line);
    case Enemy_Item::BALANCE_LIFT:          return this->enemyHandler->Balance_Lift(line);
    case Enemy_Item::SURFING_LIFT:          return this->enemyHandler->Surfing_Lift(line);
    case Enemy_Item::LIFT_SPAWNER:          return this->enemyHandler->Lift_Spawner(line);
    case Enemy_Item::BOWSER:                return this->enemyHandler->Bowser(line);
    case Enemy_Item::WARP_ZONE:             return this->enemyHandler->Warp_Zone(line);
    case Enemy_Item::PIPE_POINTER:          return this->enemyHandler->Pipe_Pointer(line);
    case Enemy_Item::TOAD:                  return this->enemyHandler->Toad(line);
    case Enemy_Item::GOOMBA_GROUP:          return this->enemyHandler->Goomba_Group(line);
    case Enemy_Item::KOOPA_GROUP:           return this->enemyHandler->Koopa_Group(line);
    case Enemy_Item::PAGE_CHANGE:           return this->enemyHandler->Page_Change(line);
    case Enemy_Item::NOTHING:               return this->enemyHandler->Nothing(line);
    default:                                return false; //invalid enemy
    }
}
