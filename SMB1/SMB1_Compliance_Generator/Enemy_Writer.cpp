#include "Enemy_Writer.h"
#include "../Common SMB1 Files/Enemy_Item_String.h"

bool Enemy_Writer::Write_Enemy(int x, bool onlyHardMode, const QString &enemy) {
    if (!this->Is_Coordinate_Valid(x)) return false;
    return this->Write_Item(ENEMY, QString(enemy+" "+QString(x)+" "+this->Get_Difficulty_String(onlyHardMode)));
}

bool Enemy_Writer::Write_Enemy(int x, bool onlyHardMode, const QString &enemy, const QString &parameters) {
    if (!this->Is_Coordinate_Valid(x)) return false;
    return this->Write_Item(ENEMY, QString(enemy+" "+QString(x)+" "+parameters+" "+this->Get_Difficulty_String(onlyHardMode)));
}

bool Enemy_Writer::Write_Enemy(int x, int y, bool onlyHardMode, const QString &enemy) {
    if (!this->Is_Coordinate_Valid(x)) return false;
    if (y > 0xD) return false;
    return this->Write_Item(ENEMY, QString(enemy+" "+QString(x)+" "+QString(y)+" "+this->Get_Difficulty_String(onlyHardMode)));
}

bool Enemy_Writer::Write_Enemy(int x, int y, bool onlyHardMode, const QString &enemy, const QString &parameters) {
    if (!this->Is_Coordinate_Valid(x)) return false;
    if (y > 0xD) return false;
    return this->Write_Item(ENEMY, QString(enemy+" "+QString(x)+" "+QString(y)+" "+parameters+" "+this->Get_Difficulty_String(onlyHardMode)));
}

QString Enemy_Writer::Get_Difficulty_String(bool onlyHardMode) {
    if (onlyHardMode) {
        return Enemy_Item::STRING_NORMAL;
    } else {
        return Enemy_Item::STRING_HARD;
    }
}

bool Enemy_Writer::Green_Koopa(int x, int y, bool moving, bool onlyHardMode) {

}

bool Enemy_Writer::Red_Koopa(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_RED_KOOPA);
}

bool Enemy_Writer::Buzzy_Beetle(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_BUZZY_BEETLE);
}

bool Enemy_Writer::Hammer_Bro(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_HAMMER_BRO);
}

bool Enemy_Writer::Goomba(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_GOOMBA);
}

bool Enemy_Writer::Blooper(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_BLOOPER);
}

bool Enemy_Writer::Bullet_Bill(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_BULLET_BILL);
}

bool Enemy_Writer::Green_Paratroopa(int x, int y, bool moving, bool leaping, bool onlyHardMode) {

}

bool Enemy_Writer::Red_Paratroopa(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_RED_PARATROOPA);
}

bool Enemy_Writer::Green_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_GREEN_CHEEP_CHEEP);
}

bool Enemy_Writer::Red_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_RED_CHEEP_CHEEP);
}

bool Enemy_Writer::Podoboo(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_PODOBOO);
}

bool Enemy_Writer::Pirana_Plant(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_PIRANA_PLANT);
}

bool Enemy_Writer::Lakitu(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_LAKITU);
}

bool Enemy_Writer::Spiny(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_SPINY);
}

bool Enemy_Writer::Bowser_Fire_Spawner(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_BOWSER_FIRE_SPAWNER);
}

bool Enemy_Writer::Cheep_Cheep_Spawner(int x, int y, bool leaping, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_CHEEP_CHEEP_SPAWNER);
}

bool Enemy_Writer::Bullet_Bill_Spawner(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_BULLET_BILL_SPAWNER);
}

bool Enemy_Writer::Fire_Bar(int x, int y, bool clockwise, bool fast, bool onlyHardMode) {

}

bool Enemy_Writer::Large_Fire_Bar(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_LARGE_FIRE_BAR);
}

bool Enemy_Writer::Lift(int x, int y, bool vertical, bool onlyHardMode) {

}

bool Enemy_Writer::Falling_Lift(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_FALLING_LIFT);
}

bool Enemy_Writer::Balance_Lift(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_BALANCE_LIFT);
}

bool Enemy_Writer::Surfing_Lift(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_SURFING_LIFT);
}

bool Enemy_Writer::Lift_Spawner(int x, int y, bool up, bool small, bool onlyHardMode) {

}

bool Enemy_Writer::Bowser(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, onlyHardMode, Enemy_Item::STRING_BOWSER);
}

bool Enemy_Writer::Warp_Zone(int x) {
    return this->Write_Enemy(x, false, Enemy_Item::STRING_WARP_ZONE);
}

bool Enemy_Writer::Toad(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, onlyHardMode, Enemy_Item::STRING_TOAD);
}

bool Enemy_Writer::Goomba_Group(int x, int y, int num, bool onlyHardMode) {
    if (y == 0x6 || y == 0xA) {
        if (num == 2 || num == 3) {
            return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_GOOMBA_GROUP, QString(num));
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Enemy_Writer::Koopa_Group(int x, int y, int num, bool onlyHardMode) {
    if (y == 0x6 || y == 0xA) {
        if (num == 2 || num == 3) {
            return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_KOOPA_GROUP, QString(num));
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Enemy_Writer::Page_Change(int x, int page) {
    if (page < 0x0 || page > 0xFF) return false;
    return this->Write_Item(ENEMY, QString(Enemy_Item::STRING_PAGE_CHANGE+" "+QString(x)+" "+QString(page)));
}

bool Enemy_Writer::Nothing(int x) {
    return this->Write_Enemy(x, false, Enemy_Item::STRING_NOTHING);
}
