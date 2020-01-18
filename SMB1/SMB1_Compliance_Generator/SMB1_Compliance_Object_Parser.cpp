#include "SMB1_Compliance_Object_Parser.h"

bool SMB1_Compliance_Object_Parser::Parse_Line(const QString &line, Object_Item::Object_Item &objectItem, int &x, int &y, int &length, QString &attributes) {
    if (line.isEmpty()) return false;
    x = 0; y = 0; length = 0; attributes = QString();

    QStringList elements = line.split(' ');
    QMap<QString, Object_Item::Object_Item>::iterator iter = this->objectsMap->find(elements.at(0));
    if (iter == this->objectsMap->end()) return false; //not found
    objectItem = iter.value();
    switch (objectItem) {
    case Object_Item::AXE:
    case Object_Item::AXE_ROPE:
    case Object_Item::BOWSER_BRIDGE:
    case Object_Item::CANCEL_SPAWNER:
    case Object_Item::CASTLE:
    case Object_Item::BIG_CASTLE:
    case Object_Item::END_STEPS:
    case Object_Item::FLAGPOLE:
    case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:
    case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:
    case Object_Item::BULLET_BILL_SPAWNER:
    case Object_Item::LIFT_ROPE:
    case Object_Item::LOOP_COMMAND:
    case Object_Item::NOTHING:
    case Object_Item::PIPE_WALL:
    case Object_Item::REVERSE_L_PIPE:
    case Object_Item::SCROLL_STOP:
    case Object_Item::SCROLL_STOP_WARP_ZONE:
    case Object_Item::TOGGLE_AUTO_SCROLL: //x
        if (elements.size() != 2) return false;
        if (!this->Get_Number_From_String(elements.at(1), x)) return false;
        break;
    case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
    case Object_Item::QUESTION_BLOCK_WITH_COIN:
    case Object_Item::BRICK_WITH_MUSHROOM:
    case Object_Item::BRICK_WITH_VINE:
    case Object_Item::BRICK_WITH_STAR:
    case Object_Item::BRICK_WITH_10_COINS:
    case Object_Item::BRICK_WITH_1UP:
    case Object_Item::HIDDEN_BLOCK_WITH_COIN:
    case Object_Item::HIDDEN_BLOCK_WITH_1UP:
    case Object_Item::USED_BLOCK:
    case Object_Item::UNDERWATER_SIDEWAYS_PIPE:
    case Object_Item::TRAMPOLINE: //x y
        if (elements.size() != 3) return false;
        if (!this->Get_Number_From_String(elements.at(1), x)) return false;
        if (!this->Get_Number_From_String(elements.at(2), y)) return false;
        break;
    case Object_Item::BRIDGE:
    case Object_Item::ISLAND:
    case Object_Item::HORIZONTAL_BRICKS:
    case Object_Item::HORIZONTAL_BLOCKS:
    case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:
    case Object_Item::CANNON:
    case Object_Item::CORRAL:
    case Object_Item::VERTICAL_BRICKS:
    case Object_Item::VERTICAL_BLOCKS:
    case Object_Item::HORIZONTAL_COINS:
    case Object_Item::PIPE:
    case Object_Item::ENTERABLE_PIPE:
    case Object_Item::TALL_REVERSE_L_PIPE: //x y length
        if (elements.size() != 4) return false;
        if (!this->Get_Number_From_String(elements.at(1), x)) return false;
        if (!this->Get_Number_From_String(elements.at(2), y)) return false;
        if (!this->Get_Number_From_String(elements.at(3), length)) return false;
        break;
    case Object_Item::BALANCE_LIFT_VERTICAL_ROPE:
    case Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE:
    case Object_Item::HOLE:
    case Object_Item::HOLE_WITH_WATER:
    case Object_Item::STEPS: //x length
        if (elements.size() != 3) return false;
        if (!this->Get_Number_From_String(elements.at(1), x)) return false;
        if (!this->Get_Number_From_String(elements.at(2), length)) return false;
        break;
    case Object_Item::PAGE_CHANGE: //length
        if (elements.size() != 2) return false;
        if (!this->Get_Number_From_String(elements.at(1), length)) return false;
        break;
    case Object_Item::CHANGE_BACKGROUND: //x attribute
        if (elements.size() != 3) return false;
        if (!this->Get_Number_From_String(elements.at(1), x)) return false;
        attributes = elements.at(2);
        break;
    case Object_Item::CHANGE_BRICK_AND_SCENERY: //x attributes
        if (elements.size() != 4) return false;
        if (!this->Get_Number_From_String(elements.at(1), x)) return false;
        attributes = elements.at(2) + " " + elements.at(3);
        break;
    }
    return true;
}

bool SMB1_Compliance_Object_Parser::Get_Number_From_String(const QString &string, int &num) {
    bool valid = false;
    num = string.toInt(&valid);
    return valid;
}
