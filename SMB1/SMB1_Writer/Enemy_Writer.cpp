#include "Enemy_Writer.h"

bool Enemy_Writer::Write_Enemy(int x, int y, int enemyByte) {
    if (y > 0xD) return false; //enemies must exist between 0x0 and 0xD
    return this->Write_Item(x, y, enemyByte);
}

bool Enemy_Writer::Green_Koopa(int x, int y, bool moving = true) {
    if (moving) {
        return this->Write_Enemy(x, y, 0x00);
    } else {
        return this->Write_Enemy(x, y, 0x04);
    }
}

bool Enemy_Writer::Red_Koopa(int x, int y) {
    return this->Write_Enemy(x, y, 0x03);
}

bool Enemy_Writer::Buzzy_Beetle(int x, int y) {
    return this->Write_Enemy(x, y, 0x02);
}

bool Enemy_Writer::Hammer_Bro(int x, int y){
    return this->Write_Enemy(x, y, 0x05);
}

bool Enemy_Writer::Goomba(int x, int y) {
    return this->Write_Enemy(x, y, 0x06);
}

bool Enemy_Writer::Blooper(int x, int y) {
    return this->Write_Enemy(x, y, 0x07);
}

bool Enemy_Writer::Bullet_Bill(int x, int y) {
    return this->Write_Enemy(x, y, 0x08);
}

bool Enemy_Writer::Green_Paratroopa(int x, int y, bool moving = true, bool leaping = true) {
    if (moving) {
        if (leaping) {
            return this->Write_Enemy(x, y, 0x0E);
        } else {
            return this->Write_Enemy(x, y, 0x10);
        }
    } else {
        return this->Write_Enemy(x, y, 0x09);
    }
}

bool Enemy_Writer::Red_Paratroopa(int x, int y) {
    return this->Write_Enemy(x, y, 0x0F);
}

bool Enemy_Writer::Green_Cheep_Cheep(int x, int y) {
    return this->Write_Enemy(x, y, 0x0A);
}

bool Enemy_Writer::Red_Cheep_Cheep(int x, int y) {
    return this->Write_Enemy(x, y, 0x0B);
}

bool Enemy_Writer::Podoboo(int x, int y) {
    return this->Write_Enemy(x, y, 0x0C);
}

bool Enemy_Writer::Pirana_Plant(int x, int y) {
    return this->Write_Enemy(x, y, 0x0D);
}

bool Enemy_Writer::Lakitu(int x, int y) {
    return this->Write_Enemy(x, y, 0x11);
}

bool Enemy_Writer::Spiny(int x, int y) {
    return this->Write_Enemy(x, y, 0x12);
}

bool Enemy_Writer::Bowser_Fire(int x, int y) {
    return this->Write_Enemy(x, y, 0x15);
}

bool Enemy_Writer::Cheep_Cheep_Spawner(int x, int y, bool leaping) {

}

bool Enemy_Writer::Bullet_Bill_Spawner(int x, int y) {

}

bool Enemy_Writer::Fire_Bar(int x, int y, bool clockwise = true, bool fast = false) {
    if (clockwise) {
        if (fast) {
            return this->Write_Enemy(x, y, 0x1C);
        } else {
            return this->Write_Enemy(x, y, 0x1B);
        }
    } else {
        if (fast) {
            return this->Write_Enemy(x, y, 0x1E);
        } else {
            return this->Write_Enemy(x, y, 0x1D);
        }
    }
}

bool Enemy_Writer::Large_Fire_Bar(int x, int y) {
    return this->Write_Enemy(x, y, 0x1F);
}

bool Enemy_Writer::Lift(int x, int y, bool vertical = true) {
    if (vertical) {
        return this->Write_Enemy(x, y, 0x25);
    } else {
        return this->Write_Enemy(x, y, 0x28);
    }
}

bool Enemy_Writer::Falling_Lift(int x, int y) {
    return this->Write_Enemy(x, y, 0x29);
}

bool Enemy_Writer::Balance_Lift(int x, int y) {
    return this->Write_Enemy(x, y, 0x24);
}

bool Enemy_Writer::Surfing_Lift(int x, int y) {
    return this->Write_Enemy(x, y, 0x2A);
}

bool Enemy_Writer::Lift_Spawner(int x, int y, bool up = true, bool small = false) {
    if (up) {
        if (small) {
            return this->Write_Enemy(x, y, 0x2B);
        } else {
            return this->Write_Enemy(x, y, 0x26);
        }
    } else {
        if (small) {
            return this->Write_Enemy(x, y, 0x2C);
        } else {
            return this->Write_Enemy(x, y, 0x27);
        }
    }
}

bool Enemy_Writer::Bowser(int x) {

}

bool Enemy_Writer::Warp_Zone(int x, int y) {

}

bool Enemy_Writer::Toad(int x, int y) {

}

bool Enemy_Writer::Goomba_Group(int x, int y, int num) {

}

bool Enemy_Writer::Koopa_Group(int x, int y, int num) {

}

bool Enemy_Writer::Page_Skip(int x, int amount = 1) {

}

bool Enemy_Writer::Nothing(int x) {
    this->Write_Enemy(x, 0xD, 0xFF);
}
