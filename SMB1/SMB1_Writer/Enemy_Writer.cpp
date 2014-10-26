#include "Enemy_Writer.h"
#include "../Common SMB1 Files/Level_Attribute.h"
#include "Room_ID_Handler.h"
#include <QDebug>
#include <assert.h>

Enemy_Writer::Enemy_Writer(QByteArray *buffer, Header_Writer *headerWriter, Room_ID_Handler *roomIDHandler) : Item_Writer(buffer, headerWriter, roomIDHandler) {
    this->groupPageFlag = false;
}

bool Enemy_Writer::Write_Enemy(int x, int y, int enemyByte, bool onlyHardMode) {
    if (y > 0xD) return false; //enemies must exist between 0x0 and 0xD
    assert(enemyByte >= 0x0);
    if (onlyHardMode) {
        enemyByte += 64; //trigger the hard mode bit
        assert(enemyByte <= 0x7F);
    }

    //Fix the coordinates if a group was spawned previously
    x = this->Handle_Group_Page_Flag(x);

    return this->Write_Item(x, y, enemyByte);
}

bool Enemy_Writer::Write_Group(int x, int y, int enemyByte, bool onlyHardMode) {
    if (y < 0x0 || y > 0xD) return false; //enemies must exist between 0x0 and 0xD
    assert(enemyByte >= 0x0);
    if (onlyHardMode) {
        enemyByte += 64; //trigger the hard mode bit
        assert(enemyByte <= 0x7F);
    }
    if (!this->Is_Safe_To_Write_Item()) return false;

    //Handle the x coordinate
    x = this->Handle_Group_Page_Flag(x);
    x += 3;
    int tmpX = this->currentX + x;
    bool tmpPageFlag = this->pageFlag;
    if (tmpX > 0xF) { //set the page flag if necessary
        if (tmpPageFlag) return false;
        tmpPageFlag = true;
        tmpX -= 0x10;
    }
    if (tmpX > 0xF) return false; //the x coordinate cannot be off the next page

    //Set the member variables
    this->currentY = y;
    this->pageFlag = tmpPageFlag;
    this->currentX += x-3;
    if (this->currentX > 0xF) this->currentX -= 0x10;
    if (this->currentX >= 0xD) this->groupPageFlag = true;

    //Write the position byte
    QBitArray positionBits(8, false);
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(positionBits, 0, static_cast<unsigned char>(tmpX)); //x
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(positionBits, 4, static_cast<unsigned char>(y)); //y
    int positionByte = Binary_Manipulator::BitArray_To_Hex(positionBits); //get the byte
    if (!this->Write_Byte_To_Buffer(positionByte)) return false;

    //Write the item byte
    if (this->pageFlag) {
        this->pageFlag = false;
        enemyByte += 128; //set the page flag in the byte
        assert(enemyByte <= 0xFF);
    }
    return this->Write_Byte_To_Buffer(enemyByte);
}

int Enemy_Writer::Handle_Group_Page_Flag(int x) {
    if (this->groupPageFlag) {
        if (this->currentX + x > 0xF) {
            this->groupPageFlag = false;
            this->currentX += x;
            this->currentX -= 0x10; //roll current X over manually to prevent triggering the page flag
            return 0;
        }
    }
    return x;
}

bool Enemy_Writer::Fill_Buffer() {
    while (this->Is_Safe_To_Write_Item()) {
        assert(this->Nothing(0));
    }
    if (this->How_Many_Bytes_Left() == 1) {
        assert(this->Write_Byte_To_Buffer(0xFF)); //fill the buffer with the terminator byte
    }
    return true;
}

bool Enemy_Writer::Green_Koopa(int x, int y, bool moving, bool onlyHardMode) {
    if (moving) {
        return this->Write_Enemy(x, y+1, 0x00, onlyHardMode);
    } else {
        return this->Write_Enemy(x, y+1, 0x04, onlyHardMode);
    }
}

bool Enemy_Writer::Red_Koopa(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x03, onlyHardMode);
}

bool Enemy_Writer::Buzzy_Beetle(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x02, onlyHardMode);
}

bool Enemy_Writer::Hammer_Bro(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x05, onlyHardMode);
}

bool Enemy_Writer::Goomba(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x06, onlyHardMode);
}

bool Enemy_Writer::Blooper(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x07, onlyHardMode);
}

bool Enemy_Writer::Bullet_Bill(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x08, onlyHardMode);
}

bool Enemy_Writer::Green_Paratroopa(int x, int y, bool moving, bool leaping, bool onlyHardMode) {
    if (moving) {
        if (leaping) {
            return this->Write_Enemy(x, y+1, 0x0E, onlyHardMode);
        } else {
            return this->Write_Enemy(x, y+1, 0x10, onlyHardMode);
        }
    } else {
        return this->Write_Enemy(x, y+1, 0x09, onlyHardMode);
    }
}

bool Enemy_Writer::Red_Paratroopa(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x0F, onlyHardMode);
}

bool Enemy_Writer::Green_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x0A, onlyHardMode);
}

bool Enemy_Writer::Red_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x0B, onlyHardMode);
}

bool Enemy_Writer::Podoboo(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x0C, onlyHardMode);
}

bool Enemy_Writer::Pirana_Plant(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x0D, onlyHardMode);
}

bool Enemy_Writer::Lakitu(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x11, onlyHardMode);
}

bool Enemy_Writer::Spiny(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x12, onlyHardMode);
}

bool Enemy_Writer::Bowser_Fire_Spawner(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, 0xD, 0x15, onlyHardMode);
}

bool Enemy_Writer::Cheep_Cheep_Spawner(int x, bool leaping, bool onlyHardMode) {
    if (leaping) {
        return this->Write_Enemy(x, 0xD, 0x14, onlyHardMode);
    } else {
        if (this->roomIDHandler->Get_Level_Attribute_From_Current_Level() != Level_Attribute::UNDERWATER) return false;
        return this->Write_Enemy(x, 0xD, 0x17, onlyHardMode);
    }
}

bool Enemy_Writer::Bullet_Bill_Spawner(int x, bool onlyHardMode) {
    if (this->roomIDHandler->Get_Level_Attribute_From_Current_Level() == Level_Attribute::UNDERWATER) return false;
    return this->Write_Enemy(x, 0xD, 0x17, onlyHardMode);
}

bool Enemy_Writer::Fire_Bar(int x, int y, bool clockwise, bool fast, bool onlyHardMode) {
    if (clockwise) {
        if (fast) {
            return this->Write_Enemy(x, y+1, 0x1C, onlyHardMode);
        } else {
            return this->Write_Enemy(x, y+1, 0x1B, onlyHardMode);
        }
    } else {
        if (fast) {
            return this->Write_Enemy(x, y+1, 0x1E, onlyHardMode);
        } else {
            return this->Write_Enemy(x, y+1, 0x1D, onlyHardMode);
        }
    }
}

bool Enemy_Writer::Large_Fire_Bar(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x1F, onlyHardMode);
}

bool Enemy_Writer::Lift(int x, int y, bool vertical, bool onlyHardMode) {
    if (vertical) {
        return this->Write_Enemy(x, y+1, 0x25, onlyHardMode);
    } else {
        return this->Write_Enemy(x, y+1, 0x28, onlyHardMode);
    }
}

bool Enemy_Writer::Falling_Lift(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x29, onlyHardMode);
}

bool Enemy_Writer::Balance_Lift(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x24, onlyHardMode);
}

bool Enemy_Writer::Surfing_Lift(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x2A, onlyHardMode);
}

bool Enemy_Writer::Lift_Spawner(int x, int y, bool up, bool small, bool onlyHardMode) {
    if (up) {
        if (small) {
            return this->Write_Enemy(x, y+1, 0x2B, onlyHardMode);
        } else {
            return this->Write_Enemy(x, y+1, 0x26, onlyHardMode);
        }
    } else {
        if (small) {
            return this->Write_Enemy(x, y+1, 0x2C, onlyHardMode);
        } else {
            return this->Write_Enemy(x, y+1, 0x27, onlyHardMode);
        }
    }
}

bool Enemy_Writer::Bowser(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, 0x8, 0x2D, onlyHardMode);
}

bool Enemy_Writer::Warp_Zone(int x) {
    return this->Write_Enemy(x, 0x7, 0x34, false);
}

bool Enemy_Writer::Toad(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, 0x0, 0x35, onlyHardMode);
}

bool Enemy_Writer::Goomba_Group(int x, int y, int num, bool onlyHardMode) {
    if (y == 0x6) {
        if (num == 2) {
            return this->Write_Group(x, y, 0x39, onlyHardMode);
        } else if (num == 3) {
            return this->Write_Group(x, y, 0x3A, onlyHardMode);
        } else {
            return false;
        }
    } else if (y == 0xA) {
        if (num == 2) {
            return this->Write_Group(x, y, 0x37, onlyHardMode);
        } else if (num == 3) {
            return this->Write_Group(x, y, 0x38, onlyHardMode);
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Enemy_Writer::Koopa_Group(int x, int y, int num, bool onlyHardMode) {
    if (y == 0x6) {
        if (num == 2) {
            return this->Write_Group(x, y, 0x3D, onlyHardMode);
        } else if (num == 3) {
            return this->Write_Group(x, y, 0x3E, onlyHardMode);
        } else {
            return false;
        }
    } else if (y == 0xA) {
        if (num == 2) {
            return this->Write_Group(x, y, 0x3B, onlyHardMode);
        } else if (num == 3) {
            return this->Write_Group(x, y, 0x3C, onlyHardMode);
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Enemy_Writer::Page_Change(int page) {
    if (page < 0x0 || page > 0xFF) return false;
    if (!this->Is_Safe_To_Write_Item()) return false;
    if (!this->Write_Byte_To_Buffer(0x0F)) return false;
    if (!this->Write_Byte_To_Buffer(page)) return false;
    this->currentPage = page;
    this->currentX = 0;
    this->currentY = 0;
    this->groupPageFlag = false;
    return true;
}

bool Enemy_Writer::Pipe_Pointer(int x, int room, int page) {
    if (this->How_Many_Bytes_Left() < 3) return false;
    x = this->Handle_Group_Page_Flag(x);
    if (!this->Write_Coordinates(x, 0xE)) return false;
    if (!this->Write_Byte_To_Buffer(room)) return false;
    return this->Write_Byte_To_Buffer(page);
}

bool Enemy_Writer::Nothing(int x) {
    return this->Write_Enemy(x, 0xD, 0x23, false);
}
