#include "Enemy_Writer.h"
#include "../Common_SMB1_Files/Enemy_Item_String.h"
#include <QDebug>
#include <assert.h>

Enemy_Writer::Enemy_Writer(int nbl) : Item_Writer(nbl) {
    this->firstEnemy = true;
    this->lakituIsActive = false;
    this->wasLakituSpawned = false;
}

bool Enemy_Writer::Get_First_Enemy() {
    return this->firstEnemy;
}

void Enemy_Writer::Set_First_Enemy(bool value) {
    this->firstEnemy = value;
}

bool Enemy_Writer::Is_Lakitu_Active() {
    return this->lakituIsActive;
}

bool Enemy_Writer::Was_Lakitu_Spawned() {
    return this->wasLakituSpawned;
}

bool Enemy_Writer::Write_Enemy(int x, const QString &enemy) {
    if (this->firstEnemy && x < 16) x += 16;
    if (this->Write_Item(x, QString(enemy+" "+QString::number(x)))) {
        this->firstEnemy = false;
        return true;
    } else {
        return false;
    }
}

bool Enemy_Writer::Write_Enemy(int x, bool onlyHardMode, const QString &enemy) {
    if (this->firstEnemy && x < 16) x += 16;
    if (this->Write_Item(x, QString(enemy+" "+QString::number(x)+" "+this->Get_Difficulty_String(onlyHardMode)))) {
        this->firstEnemy = false;
        return true;
    } else {
        return false;
    }
}

bool Enemy_Writer::Write_Enemy(int x, bool onlyHardMode, const QString &enemy, const QString &parameters) {
    if (this->firstEnemy && x < 16) x += 16;
    if (this->Write_Item(x, QString(enemy+" "+QString::number(x)+" "+parameters+" "+this->Get_Difficulty_String(onlyHardMode)))) {
        this->firstEnemy = false;
        return true;
    } else {
        return false;
    }
}

bool Enemy_Writer::Write_Enemy(int x, int y, bool onlyHardMode, const QString &enemy) {
    if (y > 0xD) return false;
    if (this->firstEnemy && x < 16) x += 16;
    if (this->Write_Item(x, QString(enemy+" "+QString::number(x)+" "+QString::number(y)+" "+this->Get_Difficulty_String(onlyHardMode)))) {
        this->firstEnemy = false;
        this->currentY = y;
        return true;
    } else {
        return false;
    }
}

bool Enemy_Writer::Write_Enemy(int x, int y, bool onlyHardMode, const QString &enemy, const QString &parameters) {
    if (y > 0xD) return false;
    if (this->firstEnemy && x < 16) x += 16;
    if (this->Write_Item(x, QString(enemy+" "+QString::number(x)+" "+QString::number(y)+" "+parameters+" "+this->Get_Difficulty_String(onlyHardMode)))) {
        this->firstEnemy = false;
        this->currentY = y;
        return true;
    } else {
        return false;
    }
}

QString Enemy_Writer::Get_Difficulty_String(bool onlyHardMode) {
    if (onlyHardMode) {
        return Enemy_Item::STRING_HARD;
    } else {
        return Enemy_Item::STRING_NORMAL;
    }
}

bool Enemy_Writer::Is_Coordinate_Valid(int coordinate) {
    if (this->firstEnemy) return (coordinate >= 0x10 && coordinate <= 0x1F);
    return (coordinate >= 0x0 && coordinate <= 0x10);
}

bool Enemy_Writer::Green_Koopa(int x, int y, bool moving, bool onlyHardMode) {
    if (moving) {
        return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_GREEN_KOOPA, Enemy_Item::STRING_MOVING);
    } else {
        return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_GREEN_KOOPA, Enemy_Item::STRING_STOPPED);
    }
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
    if (!moving) {
        return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_GREEN_PARATROOPA, Enemy_Item::STRING_STOPPED);
    } else {
        if (leaping) {
            return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_GREEN_PARATROOPA, Enemy_Item::STRING_LEAPING);
        } else {
            return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_GREEN_PARATROOPA, Enemy_Item::STRING_FLYING);
        }
    }
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

bool Enemy_Writer::Podoboo(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, onlyHardMode, Enemy_Item::STRING_PODOBOO);
}

bool Enemy_Writer::Piranha_Plant(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_PIRANHA_PLANT);
}

bool Enemy_Writer::Lakitu(int x, int y, bool onlyHardMode) {
    bool success = this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_LAKITU);
    if (success) {
        this->wasLakituSpawned = true;
        this->lakituIsActive = true;
    }
    return success;
}

bool Enemy_Writer::Spiny(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_SPINY);
}

bool Enemy_Writer::Bowser_Fire_Spawner(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, onlyHardMode, Enemy_Item::STRING_BOWSER_FIRE_SPAWNER);
}

bool Enemy_Writer::Swimming_Cheep_Cheep_Spawner(int x, bool leaping, bool onlyHardMode) {
    if (leaping) {
        return this->Write_Enemy(x, onlyHardMode, Enemy_Item::STRING_CHEEP_CHEEP_SPAWNER, Enemy_Item::STRING_LEAPING);
    } else {
        return this->Write_Enemy(x, onlyHardMode, Enemy_Item::STRING_CHEEP_CHEEP_SPAWNER, Enemy_Item::STRING_FLYING);
    }
}

bool Enemy_Writer::Bullet_Bill_Spawner(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, onlyHardMode, Enemy_Item::STRING_BULLET_BILL_SPAWNER);
}

bool Enemy_Writer::Fire_Bar(int x, int y, bool clockwise, bool fast, bool onlyHardMode) {
    QString property = "";
    if (clockwise) {
        property = Enemy_Item::STRING_CLOCKWISE;
    } else {
        property = Enemy_Item::STRING_COUNTER_CLOCKWISE;
    }
    property += " ";
    if (fast) {
        property += Enemy_Item::STRING_FAST;
    } else {
        property += Enemy_Item::STRING_SLOW;
    }
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_FIRE_BAR, property);
}

bool Enemy_Writer::Large_Fire_Bar(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_LARGE_FIRE_BAR);
}

bool Enemy_Writer::Lift(int x, int y, bool vertical, bool onlyHardMode) {
    if (vertical) {
        return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_LIFT, Enemy_Item::STRING_VERTICAL);
    } else {
        return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_LIFT, Enemy_Item::STRING_HORIZONTAL);
    }
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
    QString property = "";
    if (up) {
        property = Enemy_Item::STRING_UP;
    } else {
        property = Enemy_Item::STRING_DOWN;
    }
    property += " ";
    if (small) {
        property += Enemy_Item::STRING_SMALL;
    } else {
        property += Enemy_Item::STRING_LARGE;
    }
    return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_LIFT_SPAWNER, property);
}

bool Enemy_Writer::Bowser(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, onlyHardMode, Enemy_Item::STRING_BOWSER);
}

bool Enemy_Writer::Warp_Zone(int x) {
    return this->Write_Enemy(x, Enemy_Item::STRING_WARP_ZONE);
}

bool Enemy_Writer::Toad(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, onlyHardMode, Enemy_Item::STRING_TOAD);
}

bool Enemy_Writer::Goomba_Group(int x, int y, int num, bool onlyHardMode) {
    if (y == 0x6 || y == 0xA) {
        if (num == 2 || num == 3) {
            return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_GOOMBA_GROUP, QString::number(num));
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
            return this->Write_Enemy(x, y, onlyHardMode, Enemy_Item::STRING_KOOPA_GROUP, QString::number(num));
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Enemy_Writer::Page_Change(int page) {
    assert(page >= 0x00 && page <= 0xFF);
    int tmpX = this->currentX;
    int tmpPage = this->currentPage;
    int tmpLevelLength = this->levelLength;
    assert(this->Handle_Level_Length_On_Page_Change(page));
    if (this->Write_Item(0x0, QString(Enemy_Item::STRING_PAGE_CHANGE+" "+QString::number(page)))) {
        this->firstEnemy = false;
        return true; //page skip successful
    } else {
        assert(false);
        //Restore previous stats
        this->currentX = tmpX;
        this->currentPage = tmpPage;
        this->levelLength = tmpLevelLength;
        return false;
    }
}
