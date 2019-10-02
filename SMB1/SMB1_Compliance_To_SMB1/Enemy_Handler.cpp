#include "Enemy_Handler.h"
#include "../../Common_Files/Random.h"
#include "../Common_SMB1_Files/Enemy_Item_String.h"
#include <QStringList>
#include <assert.h>

Enemy_Handler::Enemy_Handler(SMB1_Writer_Interface *wp, bool randomEnemies, bool allowHammerBrosInRandomEnemies) : Item_Handler(wp) {
    assert(wp);
    this->writerPlugin = wp;
    this->useRandomEnemies = randomEnemies;
    this->allowHammerBrosInRandomEnemies = allowHammerBrosInRandomEnemies;
}

bool Enemy_Handler::Parse_Difficulty(const QString &value, bool &onlyHardMode) {
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

bool Enemy_Handler::Green_Koopa(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool moving = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;

    //Parse the movement type
    if (elements.at(3) == Enemy_Item::STRING_MOVING) moving = true;
    else if (elements.at(3) == Enemy_Item::STRING_STOPPED) moving = false;
    else return false; //invalid movement type

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Enemy(x, y, onlyHardMode, this->allowHammerBrosInRandomEnemies);
    else success = this->writerPlugin->Enemy_Green_Koopa(x, y, moving, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Red_Koopa(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Enemy(x, y, onlyHardMode, this->allowHammerBrosInRandomEnemies);
    else success = this->writerPlugin->Enemy_Red_Koopa(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Buzzy_Beetle(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Enemy(x, y, onlyHardMode, this->allowHammerBrosInRandomEnemies);
    else success = this->writerPlugin->Enemy_Buzzy_Beetle(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Hammer_Bro(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Hammer_Bro(x, y, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Goomba(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Enemy(x, y, onlyHardMode, this->allowHammerBrosInRandomEnemies);
    else success = this->writerPlugin->Enemy_Goomba(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Blooper(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Underwater_Enemy(x, y, onlyHardMode, this->allowHammerBrosInRandomEnemies);
    else success = this->writerPlugin->Enemy_Blooper(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Bullet_Bill(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Flying_Enemy(x, onlyHardMode);
    else success = this->writerPlugin->Enemy_Bullet_Bill(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Green_Paratroopa(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0;
    bool moving = false; bool leaping = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;

    //Parse the movement type
    if (elements.at(3) == Enemy_Item::STRING_LEAPING) {
        moving = true;
        leaping = true;
    } else if (elements.at(3) == Enemy_Item::STRING_FLYING) {
        moving = true;
        leaping = false;
    } else if (elements.at(3) == Enemy_Item::STRING_STOPPED) {
        moving = false;
        leaping = false;
    } else {
        return false; //invalid movement type
    }

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies && leaping) success = this->writerPlugin->Enemy_Random_Enemy(x, y, onlyHardMode, this->allowHammerBrosInRandomEnemies);
    else if (this->useRandomEnemies && !leaping) success = this->writerPlugin->Enemy_Random_Flying_Enemy(x, onlyHardMode);
    else success = this->writerPlugin->Enemy_Green_Paratroopa(x, y, moving, leaping, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Red_Paratroopa(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Flying_Enemy(x, onlyHardMode);
    else success = this->writerPlugin->Enemy_Red_Paratroopa(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Green_Cheep_Cheep(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Flying_Enemy(x, onlyHardMode);
    else success = this->writerPlugin->Enemy_Green_Cheep_Cheep(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Red_Cheep_Cheep(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Flying_Enemy(x, onlyHardMode);
    else success = this->writerPlugin->Enemy_Red_Cheep_Cheep(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Podoboo(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Difficulty(elements.at(2), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Flying_Enemy(x, onlyHardMode);
    else success = this->writerPlugin->Enemy_Podoboo(x, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Piranha_Plant(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Piranha_Plant(x, y, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Lakitu(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Lakitu(x, y, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Spiny(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Enemy(x, y, onlyHardMode, this->allowHammerBrosInRandomEnemies);
    else success = this->writerPlugin->Enemy_Spiny(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Bowser_Fire_Spawner(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Difficulty(elements.at(2), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Bowser_Fire_Spawner(x, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Swimming_Cheep_Cheep_Spawner(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; bool leaping = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Parse the movement type
    if (elements.at(2) == Enemy_Item::STRING_LEAPING) leaping = true;
    else if (elements.at(2) == Enemy_Item::STRING_FLYING) leaping = false;
    else return false; //invalid movement type
    if (!this->writerPlugin->Enemy_Swimming_Cheep_Cheep_Spawner(x, leaping, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Bullet_Bill_Spawner(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Difficulty(elements.at(2), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Bullet_Bill_Spawner(x, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Fire_Bar(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 6) return false;
    int x = 0; int y = 0;
    bool clockwise = false; bool fast = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(5), onlyHardMode)) return false;

    //Parse the movement direction
    if (elements.at(3) == Enemy_Item::STRING_CLOCKWISE) clockwise = true;
    else if (elements.at(3) == Enemy_Item::STRING_COUNTER_CLOCKWISE) clockwise = false;
    else return false; //invalid direction

    //Parse the speed
    if (elements.at(4) == Enemy_Item::STRING_SLOW) fast = false;
    else if (elements.at(4) == Enemy_Item::STRING_FAST) fast = true;
    else return false; //invalid speed

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Fire_Bar(x, y, onlyHardMode);
    else success = this->writerPlugin->Enemy_Fire_Bar(x, y, clockwise, fast, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Large_Fire_Bar(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Fire_Bar(x, y, onlyHardMode);
    else success = this->writerPlugin->Enemy_Large_Fire_Bar(x, y, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Lift(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool vertical = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;

    //Parse the movement type
    if (elements.at(3) == Enemy_Item::STRING_VERTICAL) vertical = true;
    else if (elements.at(3) == Enemy_Item::STRING_HORIZONTAL) vertical = false;
    else return false; //invalid movement type
    if (!this->writerPlugin->Enemy_Lift(x, y, vertical, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Falling_Lift(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Falling_Lift(x, y, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Balance_Lift(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Balance_Lift(x, y, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Surfing_Lift(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Surfing_Lift(x, y, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Lift_Spawner(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 6) return false;
    int x = 0; int y = 0;
    bool up = false; bool small = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Difficulty(elements.at(5), onlyHardMode)) return false;

    //Parse the movement direction
    if (elements.at(3) == Enemy_Item::STRING_UP) up = true;
    else if (elements.at(3) == Enemy_Item::STRING_DOWN) up = false;
    else return false; //invalid direction

    //Parse the size
    if (elements.at(4) == Enemy_Item::STRING_LARGE) small = false;
    else if (elements.at(4) == Enemy_Item::STRING_SMALL) small = true;
    else return false; //invalid size

    if (!this->writerPlugin->Enemy_Lift_Spawner(x, y, up, small, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Bowser(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Difficulty(elements.at(2), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Bowser(x, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Warp_Zone(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->writerPlugin->Enemy_Warp_Zone(x)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Pipe_Pointer(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0, world = 0, page = 0;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(3), world)) return false;
    if (!this->Parse_Num(elements.at(4), page)) return false;
    if (!this->writerPlugin->Enemy_Pipe_Pointer(x, elements.at(2), world, page)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Toad(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Difficulty(elements.at(2), onlyHardMode)) return false;
    if (!this->writerPlugin->Enemy_Toad(x, onlyHardMode)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}

bool Enemy_Handler::Goomba_Group(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; int num = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), num)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Enemy_Group(x, y, num, onlyHardMode);
    else success = this->writerPlugin->Enemy_Goomba_Group(x, y, num, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Koopa_Group(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; int num = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(1), x)) return false;
    if (!this->Parse_Num(elements.at(2), y)) return false;
    if (!this->Parse_Num(elements.at(3), num)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;

    //Write the enemy
    bool success = false;
    if (this->useRandomEnemies) success = this->writerPlugin->Enemy_Random_Enemy_Group(x, y, num, onlyHardMode);
    else success = this->writerPlugin->Enemy_Koopa_Group(x, y, num, onlyHardMode);
    if (!success) errorCode = 3;
    return success;
}

bool Enemy_Handler::Page_Change(const QString &line, int &errorCode) {
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    int page = 0;
    if (!this->Parse_Num(elements.at(1), page)) return false;
    if (!this->writerPlugin->Enemy_Page_Change(page)) {
        errorCode = 3;
        return false;
    } else {
        return true;
    }
}
