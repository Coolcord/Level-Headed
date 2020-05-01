#include "Enemy_Buffer.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include "../Common_SMB1_Files/Level_String.h"
#include "Room_ID_Handler.h"
#include <QDebug>
#include <assert.h>

Enemy_Buffer::Enemy_Buffer(QByteArray *b, Header_Writer *hw, Room_ID_Handler *ridh) : Item_Buffer(b, hw, ridh) {
    this->groupPageFlag = false;
    this->levelSlots = new QMap<QString, Level::Level>();
    this->Populate_Level_Slots();
}

Enemy_Buffer::~Enemy_Buffer() {
    delete this->levelSlots;
}

bool Enemy_Buffer::Write_Enemy(int x, int y, int enemyByte, bool onlyHardMode) {
    if (y > 0xF || y == 0xE) return false; //enemies must exist between 0x0 and 0xD. 0xF is allowed for "Nothing"
    assert(enemyByte >= 0x0);
    if (onlyHardMode) {
        enemyByte += 64; //trigger the hard mode bit
        assert(enemyByte <= 0x7F);
    }

    //Fix the coordinates if a group was spawned previously
    x = this->Handle_Group_Page_Flag(x);

    return this->Write_Item(x, y, enemyByte);
}

bool Enemy_Buffer::Write_Group(int x, int y, int enemyByte, bool onlyHardMode) {
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

int Enemy_Buffer::Handle_Group_Page_Flag(int x) {
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

int Enemy_Buffer::Get_Random_Air_Y() {
    return Random::Get_Instance().Get_Num(1, 10);
}

bool Enemy_Buffer::Fill_Buffer() {
    while (this->Is_Safe_To_Write_Item()) {
        assert(this->Write_Byte_To_Buffer(0xFF));
        assert(this->Write_Byte_To_Buffer(0xFF));
    }
    if (this->How_Many_Bytes_Left() == 1) {
        assert(this->Write_Byte_To_Buffer(0xFF)); //fill the buffer with the terminator byte
    }
    return true;
}

bool Enemy_Buffer::Random_Continous_Enemy_Spawner(int x, bool underwater, bool onlyHardMode) {
    //Only do Bullet Bills or Cheep-Cheeps. Don't do Lakitus here
    if (underwater) {
        return this->Swimming_Cheep_Cheep_Spawner(x, false, onlyHardMode);
    } else {
        if (Random::Get_Instance().Get_Num(1)) return this->Bullet_Bill_Spawner(x, onlyHardMode);
        else return this->Swimming_Cheep_Cheep_Spawner(x, true, onlyHardMode);
    }
}

bool Enemy_Buffer::Random_Enemy(int x, int y, bool onlyHardMode, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners) {
    Level_Attribute::Level_Attribute attribute = this->roomIDHandler->Get_Level_Attribute_From_Current_Level();
    if (allowLakitus && Random::Get_Instance().Get_Num(10) == 0) return this->Lakitu(x, this->Get_Random_Air_Y(), onlyHardMode);
    if (allowContinousEnemySpawners && attribute != Level_Attribute::UNDERWATER && Random::Get_Instance().Get_Num(10) == 0) return this->Random_Continous_Enemy_Spawner(x, false, onlyHardMode);
    int maxValue = 9;
    if (allowHammerBros) ++maxValue;
    int value = Random::Get_Instance().Get_Num(maxValue);
    switch (value) {
    default:    assert(false); return false;
    case 0:     return this->Green_Koopa(x, y, true, onlyHardMode);
    case 1:     return this->Red_Koopa(x, y, onlyHardMode);
    case 2:     return this->Buzzy_Beetle(x, y, onlyHardMode);
    case 3:     return this->Goomba(x, y, onlyHardMode);
    case 4:     return this->Blooper(x, this->Get_Random_Air_Y(), onlyHardMode);
    case 5:     return this->Bullet_Bill(x, this->Get_Random_Air_Y(), onlyHardMode);
    case 6:     return this->Green_Paratroopa(x, y, true, true, onlyHardMode);
    case 7:     return this->Green_Paratroopa(x, this->Get_Random_Air_Y(), true, false, onlyHardMode);
    case 8:     return this->Red_Paratroopa(x, 3, onlyHardMode);
    case 9:     return this->Podoboo(x, onlyHardMode);
    case 10:    return this->Hammer_Bro(x, y, onlyHardMode);
    }
}

bool Enemy_Buffer::Random_Enemy_Group(int x, int y, int num, bool onlyHardMode) {
    if (Random::Get_Instance().Get_Num(1)) return this->Goomba_Group(x, y, num, onlyHardMode);
    else return this->Koopa_Group(x, y, num, onlyHardMode);
}

bool Enemy_Buffer::Random_Fire_Bar(int x, int y, bool onlyHardMode) {
    switch (Random::Get_Instance().Get_Num(4)) {
    default:    assert(false); return false;
    case 0:     return this->Fire_Bar(x, y, false, false, onlyHardMode);
    case 1:     return this->Fire_Bar(x, y, false, true, onlyHardMode);
    case 2:     return this->Fire_Bar(x, y, true, false, onlyHardMode);
    case 3:     return this->Fire_Bar(x, y, true, true, onlyHardMode);
    case 4:     return this->Large_Fire_Bar(x, y, onlyHardMode);
    }
}

bool Enemy_Buffer::Random_Flying_Enemy(int x, bool onlyHardMode) {
    switch (Random::Get_Instance().Get_Num(4)) {
    default:    assert(false); return false;
    case 0:     return this->Blooper(x, this->Get_Random_Air_Y(), onlyHardMode);
    case 1:     return this->Bullet_Bill(x, this->Get_Random_Air_Y(), onlyHardMode);
    case 2:     return this->Green_Paratroopa(x, this->Get_Random_Air_Y(), true, false, onlyHardMode);
    case 3:     return this->Red_Paratroopa(x, 3, onlyHardMode);
    case 4:     return this->Podoboo(x, onlyHardMode);
    }
}

bool Enemy_Buffer::Random_Underwater_Enemy(int x, int y, bool onlyHardMode, bool allowHammerBros) {
    int maxValue = 4;
    if (allowHammerBros) ++maxValue;
    int value = Random::Get_Instance().Get_Num(maxValue);
    switch (value) {
    default:    assert(false); return false;
    case 0:     return this->Blooper(x, this->Get_Random_Air_Y(), onlyHardMode);
    case 1:     return this->Bullet_Bill(x, this->Get_Random_Air_Y(), onlyHardMode);
    case 2:     return this->Green_Paratroopa(x, this->Get_Random_Air_Y(), true, false, onlyHardMode);
    case 3:     return this->Red_Paratroopa(x, 3, onlyHardMode);
    case 4:     return this->Podoboo(x, onlyHardMode);
    case 5:     return this->Hammer_Bro(x, y, onlyHardMode);
    }
}

bool Enemy_Buffer::Green_Koopa(int x, int y, bool moving, bool onlyHardMode) {
    if (moving) {
        return this->Write_Enemy(x, y+1, 0x00, onlyHardMode);
    } else {
        return this->Write_Enemy(x, y+1, 0x04, onlyHardMode);
    }
}

bool Enemy_Buffer::Red_Koopa(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x03, onlyHardMode);
}

bool Enemy_Buffer::Buzzy_Beetle(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x02, onlyHardMode);
}

bool Enemy_Buffer::Hammer_Bro(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x05, onlyHardMode);
}

bool Enemy_Buffer::Goomba(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x06, onlyHardMode);
}

bool Enemy_Buffer::Blooper(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x07, onlyHardMode);
}

bool Enemy_Buffer::Bullet_Bill(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x08, onlyHardMode);
}

bool Enemy_Buffer::Green_Paratroopa(int x, int y, bool moving, bool leaping, bool onlyHardMode) {
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

bool Enemy_Buffer::Red_Paratroopa(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x0F, onlyHardMode);
}

bool Enemy_Buffer::Green_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x0A, onlyHardMode);
}

bool Enemy_Buffer::Red_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x0B, onlyHardMode);
}

bool Enemy_Buffer::Podoboo(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, 0xC, 0x0C, onlyHardMode);
}

bool Enemy_Buffer::Piranha_Plant(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x0D, onlyHardMode);
}

bool Enemy_Buffer::Lakitu(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x11, onlyHardMode);
}

bool Enemy_Buffer::Spiny(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x12, onlyHardMode);
}

bool Enemy_Buffer::Bowser_Fire_Spawner(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, 0xD, 0x15, onlyHardMode);
}

bool Enemy_Buffer::Swimming_Cheep_Cheep_Spawner(int x, bool leaping, bool onlyHardMode) {
    if (leaping) {
        return this->Write_Enemy(x, 0xD, 0x14, onlyHardMode);
    } else {
        if (this->roomIDHandler->Get_Level_Attribute_From_Current_Level() != Level_Attribute::UNDERWATER) return false;
        return this->Write_Enemy(x, 0xD, 0x17, onlyHardMode);
    }
}

bool Enemy_Buffer::Bullet_Bill_Spawner(int x, bool onlyHardMode) {
    if (this->roomIDHandler->Get_Level_Attribute_From_Current_Level() == Level_Attribute::UNDERWATER) return false;
    return this->Write_Enemy(x, 0xD, 0x17, onlyHardMode);
}

bool Enemy_Buffer::Fire_Bar(int x, int y, bool clockwise, bool fast, bool onlyHardMode) {
    if (clockwise) {
        if (fast) {
            return this->Write_Enemy(x, y+2, 0x1C, onlyHardMode);
        } else {
            return this->Write_Enemy(x, y+2, 0x1B, onlyHardMode);
        }
    } else {
        if (fast) {
            return this->Write_Enemy(x, y+2, 0x1E, onlyHardMode);
        } else {
            return this->Write_Enemy(x, y+2, 0x1D, onlyHardMode);
        }
    }
}

bool Enemy_Buffer::Large_Fire_Bar(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+2, 0x1F, onlyHardMode);
}

bool Enemy_Buffer::Lift(int x, int y, bool vertical, bool onlyHardMode) {
    if (vertical) {
        return this->Write_Enemy(x, y+1, 0x25, onlyHardMode);
    } else {
        return this->Write_Enemy(x, y+1, 0x28, onlyHardMode);
    }
}

bool Enemy_Buffer::Falling_Lift(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x29, onlyHardMode);
}

bool Enemy_Buffer::Balance_Lift(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x24, onlyHardMode);
}

bool Enemy_Buffer::Surfing_Lift(int x, int y, bool onlyHardMode) {
    return this->Write_Enemy(x, y+1, 0x2A, onlyHardMode);
}

bool Enemy_Buffer::Lift_Spawner(int x, int y, bool up, bool small, bool onlyHardMode) {
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

bool Enemy_Buffer::Bowser(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, 0x8, 0x2D, onlyHardMode);
}

bool Enemy_Buffer::Warp_Zone(int x) {
    return this->Write_Enemy(x, 0x7, 0x34, false);
}

bool Enemy_Buffer::Toad(int x, bool onlyHardMode) {
    return this->Write_Enemy(x, 0x0, 0x35, onlyHardMode);
}

bool Enemy_Buffer::Goomba_Group(int x, int y, int num, bool onlyHardMode) {
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

bool Enemy_Buffer::Koopa_Group(int x, int y, int num, bool onlyHardMode) {
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

bool Enemy_Buffer::Page_Change(int page) {
    if (page < 0x0 || page > 0xFF) return false;
    if (!this->Is_Safe_To_Write_Item()) return false;
    if (!this->Write_Byte_To_Buffer(0x0F)) return false;
    if (!this->Write_Byte_To_Buffer(page)) return false;
    this->currentPage = page;
    this->currentX = 0;
    this->currentY = 0;
    this->groupPageFlag = false;
    this->pageFlag = true;
    return true;
}

bool Enemy_Buffer::Pipe_Pointer(int x, int room, int world, int page) {
    if (this->How_Many_Bytes_Left() < 3) return false;
    if (page < 0 || page > 0x1F) return false;
    if (world < 1 || world > 8) return false;
    x = this->Handle_Group_Page_Flag(x);
    if (!this->Write_Coordinates(x, 0xE)) return false;
    room = room&0x7F;
    if (this->pageFlag) {
        this->pageFlag = false;
        room += 128; //set the page flag in the room
    }
    page += (world-1)*0x20;
    if (!this->Write_Byte_To_Buffer(room)) return false;
    return this->Write_Byte_To_Buffer(page);
}

bool Enemy_Buffer::Pipe_Pointer(int x, const QString &levelSlot, int world, int page) {
    QMap<QString, Level::Level>::Iterator iter = this->levelSlots->find(levelSlot);
    if (iter == this->levelSlots->end()) return false;
    Level::Level level = iter.value();
    unsigned char roomID = ' ';
    if (!this->roomIDHandler->Get_Room_ID_From_Level(level, roomID)) return false;
    int room = static_cast<int>(roomID);
    return this->Pipe_Pointer(x, room, world, page);
}

void Enemy_Buffer::Populate_Level_Slots() {
    this->levelSlots->clear();
    this->levelSlots->insert(Level::STRING_WORLD_1_LEVEL_1, Level::WORLD_1_LEVEL_1);
    this->levelSlots->insert(Level::STRING_WORLD_1_LEVEL_2, Level::WORLD_1_LEVEL_2);
    this->levelSlots->insert(Level::STRING_WORLD_1_LEVEL_3, Level::WORLD_1_LEVEL_3);
    this->levelSlots->insert(Level::STRING_WORLD_1_LEVEL_4, Level::WORLD_1_LEVEL_4);
    this->levelSlots->insert(Level::STRING_WORLD_2_LEVEL_1, Level::WORLD_2_LEVEL_1);
    this->levelSlots->insert(Level::STRING_WORLD_2_LEVEL_2, Level::WORLD_2_LEVEL_2);
    this->levelSlots->insert(Level::STRING_WORLD_2_LEVEL_3, Level::WORLD_2_LEVEL_3);
    this->levelSlots->insert(Level::STRING_WORLD_2_LEVEL_4, Level::WORLD_2_LEVEL_4);
    this->levelSlots->insert(Level::STRING_WORLD_3_LEVEL_1, Level::WORLD_3_LEVEL_1);
    this->levelSlots->insert(Level::STRING_WORLD_3_LEVEL_2, Level::WORLD_3_LEVEL_2);
    this->levelSlots->insert(Level::STRING_WORLD_3_LEVEL_3, Level::WORLD_3_LEVEL_3);
    this->levelSlots->insert(Level::STRING_WORLD_3_LEVEL_4, Level::WORLD_3_LEVEL_4);
    this->levelSlots->insert(Level::STRING_WORLD_4_LEVEL_1, Level::WORLD_4_LEVEL_1);
    this->levelSlots->insert(Level::STRING_WORLD_4_LEVEL_2, Level::WORLD_4_LEVEL_2);
    this->levelSlots->insert(Level::STRING_WORLD_4_LEVEL_3, Level::WORLD_4_LEVEL_3);
    this->levelSlots->insert(Level::STRING_WORLD_4_LEVEL_4, Level::WORLD_4_LEVEL_4);
    this->levelSlots->insert(Level::STRING_WORLD_5_LEVEL_1, Level::WORLD_5_LEVEL_1);
    this->levelSlots->insert(Level::STRING_WORLD_5_LEVEL_2, Level::WORLD_5_LEVEL_2);
    this->levelSlots->insert(Level::STRING_WORLD_5_LEVEL_3, Level::WORLD_5_LEVEL_3);
    this->levelSlots->insert(Level::STRING_WORLD_5_LEVEL_4, Level::WORLD_5_LEVEL_4);
    this->levelSlots->insert(Level::STRING_WORLD_6_LEVEL_1, Level::WORLD_6_LEVEL_1);
    this->levelSlots->insert(Level::STRING_WORLD_6_LEVEL_2, Level::WORLD_6_LEVEL_2);
    this->levelSlots->insert(Level::STRING_WORLD_6_LEVEL_3, Level::WORLD_6_LEVEL_3);
    this->levelSlots->insert(Level::STRING_WORLD_6_LEVEL_4, Level::WORLD_6_LEVEL_4);
    this->levelSlots->insert(Level::STRING_WORLD_7_LEVEL_1, Level::WORLD_7_LEVEL_1);
    this->levelSlots->insert(Level::STRING_WORLD_7_LEVEL_2, Level::WORLD_7_LEVEL_2);
    this->levelSlots->insert(Level::STRING_WORLD_7_LEVEL_3, Level::WORLD_7_LEVEL_3);
    this->levelSlots->insert(Level::STRING_WORLD_7_LEVEL_4, Level::WORLD_7_LEVEL_4);
    this->levelSlots->insert(Level::STRING_WORLD_8_LEVEL_1, Level::WORLD_8_LEVEL_1);
    this->levelSlots->insert(Level::STRING_WORLD_8_LEVEL_2, Level::WORLD_8_LEVEL_2);
    this->levelSlots->insert(Level::STRING_WORLD_8_LEVEL_3, Level::WORLD_8_LEVEL_3);
    this->levelSlots->insert(Level::STRING_WORLD_8_LEVEL_4, Level::WORLD_8_LEVEL_4);
    this->levelSlots->insert(Level::STRING_PIPE_INTRO, Level::PIPE_INTRO);
    this->levelSlots->insert(Level::STRING_UNDERGROUND_BONUS, Level::UNDERGROUND_BONUS);
    this->levelSlots->insert(Level::STRING_CLOUD_BONUS_1, Level::CLOUD_BONUS_1);
    this->levelSlots->insert(Level::STRING_CLOUD_BONUS_2, Level::CLOUD_BONUS_2);
    this->levelSlots->insert(Level::STRING_UNDERWATER_BONUS, Level::UNDERWATER_BONUS);
    this->levelSlots->insert(Level::STRING_WARP_ZONE, Level::WARP_ZONE);
    this->levelSlots->insert(Level::STRING_UNDERWATER_CASTLE, Level::UNDERWATER_CASTLE);
}
