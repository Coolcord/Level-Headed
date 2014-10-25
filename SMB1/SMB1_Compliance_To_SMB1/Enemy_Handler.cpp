#include "Enemy_Handler.h"
#include "../Common SMB1 Files/Enemy_Item_String.h"
#include <QStringList>
#include <assert.h>

Enemy_Handler::Enemy_Handler(SMB1_Writer_Interface *writerPlugin) : Item_Handler::Item_Handler(writerPlugin) {
    assert(writerPlugin);
    this->writerPlugin = writerPlugin;
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

bool Enemy_Handler::Green_Koopa(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 6) return false;
    int x = 0; int y = 0; bool moving = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(5), onlyHardMode)) return false;

    //Parse the movement type
    if (elements.at(4) == Enemy_Item::STRING_MOVING) moving = true;
    else if (elements.at(4) == Enemy_Item::STRING_STOPPED) moving = false;
    else return false; //invalid movement type
    return this->writerPlugin->Enemy_Green_Koopa(x, y, moving, onlyHardMode);
}

bool Enemy_Handler::Red_Koopa(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Red_Koopa(x, y, onlyHardMode);
}

bool Enemy_Handler::Buzzy_Beetle(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Buzzy_Beetle(x, y, onlyHardMode);
}

bool Enemy_Handler::Hammer_Bro(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Hammer_Bro(x, y, onlyHardMode);
}

bool Enemy_Handler::Goomba(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Goomba(x, y, onlyHardMode);
}

bool Enemy_Handler::Blooper(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Blooper(x, y, onlyHardMode);
}

bool Enemy_Handler::Bullet_Bill(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Bullet_Bill(x, y, onlyHardMode);
}

bool Enemy_Handler::Green_Paratroopa(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 6) return false;
    int x = 0; int y = 0;
    bool moving = false; bool leaping = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(5), onlyHardMode)) return false;

    //Parse the movement type
    if (elements.at(4) == Enemy_Item::STRING_LEAPING) {
        moving = true;
        leaping = true;
    } else if (elements.at(4) == Enemy_Item::STRING_FLYING) {
        moving = true;
        leaping = false;
    } else if (elements.at(4) == Enemy_Item::STRING_STOPPED) {
        moving = false;
        leaping = false;
    } else {
        return false; //invalid movement type
    }
    return this->writerPlugin->Enemy_Green_Paratroopa(x, y, moving, leaping, onlyHardMode);
}

bool Enemy_Handler::Red_Paratroopa(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Red_Paratroopa(x, y, onlyHardMode);
}

bool Enemy_Handler::Green_Cheep_Cheep(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Green_Cheep_Cheep(x, y, onlyHardMode);
}

bool Enemy_Handler::Red_Cheep_Cheep(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Red_Cheep_Cheep(x, y, onlyHardMode);
}

bool Enemy_Handler::Podoboo(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Podoboo(x, y, onlyHardMode);
}

bool Enemy_Handler::Pirana_Plant(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Pirana_Plant(x, y, onlyHardMode);
}

bool Enemy_Handler::Lakitu(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Lakitu(x, y, onlyHardMode);
}

bool Enemy_Handler::Spiny(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Spiny(x, y, onlyHardMode);
}

bool Enemy_Handler::Bowser_Fire_Spawner(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Bowser_Fire_Spawner(x, onlyHardMode);
}

bool Enemy_Handler::Cheep_Cheep_Spawner(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; bool leaping = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;

    //Parse the movement type
    if (elements.at(3) == Enemy_Item::STRING_LEAPING) leaping = true;
    else if (elements.at(3) == Enemy_Item::STRING_FLYING) leaping = false;
    else return false; //invalid movement type
    return this->writerPlugin->Enemy_Cheep_Cheep_Spawner(x, leaping, onlyHardMode);
}

bool Enemy_Handler::Bullet_Bill_Spawner(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Bullet_Bill_Spawner(x, onlyHardMode);
}

bool Enemy_Handler::Fire_Bar(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 7) return false;
    int x = 0; int y = 0;
    bool clockwise = false; bool fast = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(6), onlyHardMode)) return false;

    //Parse the movement direction
    if (elements.at(4) == Enemy_Item::STRING_CLOCKWISE) clockwise = true;
    else if (elements.at(4) == Enemy_Item::STRING_COUNTER_CLOCKWISE) clockwise = false;
    else return false; //invalid direction

    //Parse the speed
    if (elements.at(5) == Enemy_Item::STRING_SLOW) fast = false;
    else if (elements.at(5) == Enemy_Item::STRING_FAST) fast = true;
    else return false; //invalid speed

    return this->writerPlugin->Enemy_Fire_Bar(x, y, clockwise, fast, onlyHardMode);
}

bool Enemy_Handler::Large_Fire_Bar(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Large_Fire_Bar(x, y, onlyHardMode);
}

bool Enemy_Handler::Lift(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 6) return false;
    int x = 0; int y = 0; bool vertical = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(5), onlyHardMode)) return false;

    //Parse the movement type
    if (elements.at(4) == Enemy_Item::STRING_VERTICAL) vertical = true;
    else if (elements.at(4) == Enemy_Item::STRING_HORIZONTAL) vertical = false;
    else return false; //invalid movement type
    return this->writerPlugin->Enemy_Lift(x, y, vertical, onlyHardMode);
}

bool Enemy_Handler::Falling_Lift(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Falling_Lift(x, y, onlyHardMode);
}

bool Enemy_Handler::Balance_Lift(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Balance_Lift(x, y, onlyHardMode);
}

bool Enemy_Handler::Surfing_Lift(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int y = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(4), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Surfing_Lift(x, y, onlyHardMode);
}

bool Enemy_Handler::Lift_Spawner(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 7) return false;
    int x = 0; int y = 0;
    bool up = false; bool small = false; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Difficulty(elements.at(6), onlyHardMode)) return false;

    //Parse the movement direction
    if (elements.at(4) == Enemy_Item::STRING_VERTICAL) up = true;
    else if (elements.at(4) == Enemy_Item::STRING_HORIZONTAL) up = false;
    else return false; //invalid direction

    //Parse the size
    if (elements.at(5) == Enemy_Item::STRING_LARGE) small = false;
    else if (elements.at(5) == Enemy_Item::STRING_SMALL) small = true;
    else return false; //invalid size

    return this->writerPlugin->Enemy_Lift_Spawner(x, y, up, small, onlyHardMode);
}

bool Enemy_Handler::Bowser(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Bowser(x, onlyHardMode);
}

bool Enemy_Handler::Warp_Zone(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    return this->writerPlugin->Enemy_Warp_Zone(x);
}

bool Enemy_Handler::Pipe_Pointer(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 5) return false;
    int x = 0; int room = 0; int page = 0;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), room)) return false;
    if (!this->Parse_Num(elements.at(4), page)) return false;
    return this->writerPlugin->Enemy_Pipe_Pointer(x, room, page);
}

bool Enemy_Handler::Toad(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 4) return false;
    int x = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Difficulty(elements.at(3), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Toad(x, onlyHardMode);
}

bool Enemy_Handler::Goomba_Group(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 6) return false;
    int x = 0; int y = 0; int num = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Num(elements.at(4), num)) return false;
    if (!this->Parse_Difficulty(elements.at(5), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Goomba_Group(x, y, num, onlyHardMode);
}

bool Enemy_Handler::Koopa_Group(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 6) return false;
    int x = 0; int y = 0; int num = 0; bool onlyHardMode = false;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    if (!this->Parse_Num(elements.at(3), y)) return false;
    if (!this->Parse_Num(elements.at(4), num)) return false;
    if (!this->Parse_Difficulty(elements.at(5), onlyHardMode)) return false;
    return this->writerPlugin->Enemy_Koopa_Group(x, y, num, onlyHardMode);
}

bool Enemy_Handler::Page_Change(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int page = 0;
    if (!this->Parse_Num(elements.at(2), page)) return false;
    return this->writerPlugin->Enemy_Page_Change(page);
}

bool Enemy_Handler::Nothing(const QString &line) {
    QStringList elements = line.split(' ');
    if (elements.size() != 3) return false;
    int x = 0;
    if (!this->Parse_Num(elements.at(2), x)) return false;
    return this->writerPlugin->Enemy_Nothing(x);
}
