#include "Level_Offset.h"
#include "ROM_Checksum.h"
#include <QCryptographicHash>
#include <QList>
#include <assert.h>

Level_Offset::Level_Offset(QFile *file) {
    assert(file);
    if (!file->isOpen()) {
        throw "File is not open";
    }
    this->file = file;
    this->romType = this->Determine_ROM_Type();
    if (this->romType == INVALID) {
        throw "ROM is not a valid SMB1 ROM";
    }
}

ROM_Type Level_Offset::Determine_ROM_Type() {
    if (!this->Check_ROM_Header()) return INVALID; //bad header
    if (this->file->size() > (1048576)) return INVALID; //ROM is too big
    QByteArray buffer = this->file->readAll();
    if (buffer == NULL || buffer.size() == 0) return INVALID;

    //Verify if the ROM is a supported version via checksum
    QString hash = QString(QCryptographicHash::hash(buffer, QCryptographicHash::Sha512));
    if (hash == ROM_Checksum::FDS) return FDS;
    else if (hash == ROM_Checksum::TRACK) return TRACK;
    else if (ROM_Checksum::Is_ROM_DEFAULT(hash)) return DEFAULT;
    else return INVALID;
}

bool Level_Offset::Check_ROM_Header() {
    if (!this->file->seek(0)) return false;
    QByteArray buffer;
    if (!this->file->read(buffer.data(), 4) != 4) return false;
    if (buffer.data() == NULL || buffer.size() != 4) return false;
    buffer = buffer.toHex();

    if (this->Check_NES_ROM_Header(&buffer)) return true;
    return this->Check_FDS_ROM_Header(&buffer);
}

bool Level_Offset::Check_NES_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "NES"
    if (buffer->at(0) != 0x4E) return false;
    if (buffer->at(1) != 0x45) return false;
    if (buffer->at(2) != 0x53) return false;
    return buffer->at(3) == 0x1A;
}

bool Level_Offset::Check_FDS_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "FDS"
    if (buffer->at(0) != 0x46) return false;
    if (buffer->at(1) != 0x44) return false;
    if (buffer->at(2) != 0x53) return false;
    return buffer->at(3) == 0x1A;
}

int Level_Offset::Get_Level_Object_Offset(Level level) {
    switch (level) {
    case WORLD_1_LEVEL_1:   return this->Fix_Offset(0x26A0);
    case WORLD_1_LEVEL_2:   return this->Fix_Offset(0x2C47);
    case WORLD_1_LEVEL_3:
    case WORLD_5_LEVEL_3:   return this->Fix_Offset(0x2705);
    case WORLD_1_LEVEL_4:
    case WORLD_6_LEVEL_4:   return this->Fix_Offset(0x21C1);
    case WORLD_2_LEVEL_1:   return this->Fix_Offset(0x27DF);
    case WORLD_2_LEVEL_2:
    case WORLD_7_LEVEL_2:   return this->Fix_Offset(0x2E57);
    case WORLD_2_LEVEL_3:
    case WORLD_7_LEVEL_3:   return this->Fix_Offset(0x275A);
    case WORLD_2_LEVEL_4:
    case WORLD_5_LEVEL_4:   return this->Fix_Offset(0x22A1);
    case WORLD_3_LEVEL_1:   return this->Fix_Offset(0x262B);
    case WORLD_3_LEVEL_2:   return this->Fix_Offset(0x2C14);
    case WORLD_3_LEVEL_3:   return this->Fix_Offset(0x247D);
    case WORLD_3_LEVEL_4:   return this->Fix_Offset(0x2314);
    case WORLD_4_LEVEL_1:   return this->Fix_Offset(0x2549);
    case WORLD_4_LEVEL_2:   return this->Fix_Offset(0x2CEA);
    case WORLD_4_LEVEL_3:   return this->Fix_Offset(0x28A1);
    case WORLD_4_LEVEL_4:   return this->Fix_Offset(0x2222);
    case WORLD_5_LEVEL_1:   return this->Fix_Offset(0x284D);
    case WORLD_5_LEVEL_2:   return this->Fix_Offset(0x2AA4);
    case WORLD_6_LEVEL_1:   return this->Fix_Offset(0x296D);
    case WORLD_6_LEVEL_2:   return this->Fix_Offset(0x259C);
    case WORLD_6_LEVEL_3:   return this->Fix_Offset(0x2908);
    case WORLD_7_LEVEL_1:   return this->Fix_Offset(0x2B90);
    case WORLD_7_LEVEL_4:   return this->Fix_Offset(0x2381);
    case WORLD_8_LEVEL_1:   return this->Fix_Offset(0x2A11);
    case WORLD_8_LEVEL_2:   return this->Fix_Offset(0x2B17);
    case WORLD_8_LEVEL_3:   return this->Fix_Offset(0x24E0);
    case WORLD_8_LEVEL_4:   return this->Fix_Offset(0x240C);
    case PIPE_INTRO:        return this->Fix_Offset(0x2844);
    case UNDERGROUND_BONUS: return this->Fix_Offset(0x2D8B);
    case CLOUD_BONUS_1:     return this->Fix_Offset(0x288C);
    case CLOUND_BONUS_2:    return this->Fix_Offset(0x2BE9);
    case UNDERWATER_BONUS:  return this->Fix_Offset(0x29E0);
    case WARP_ZONE:         return this->Fix_Offset(0x2E18);
    case UNDERWATER_CASTLE: return this->Fix_Offset(0x2ED2);
    default:
        assert(false);
    }
    return BAD_OFFSET;
}

int Level_Offset::Get_Level_Enemy_Offset(Level level) {
    switch (level) {
    case WORLD_1_LEVEL_1:   return this->Fix_Offset(0x1F11);
    case WORLD_1_LEVEL_2:   return this->Fix_Offset(0x20E8);
    case WORLD_1_LEVEL_3:
    case WORLD_5_LEVEL_3:   return this->Fix_Offset(0x1F2F);
    case WORLD_1_LEVEL_4:
    case WORLD_6_LEVEL_4:   return this->Fix_Offset(0x1D80);
    case WORLD_2_LEVEL_1:   return this->Fix_Offset(0x1F61);
    case WORLD_2_LEVEL_2:
    case WORLD_7_LEVEL_2:   return this->Fix_Offset(0x2181);
    case WORLD_2_LEVEL_3:
    case WORLD_7_LEVEL_3:   return this->Fix_Offset(0x1F4C);
    case WORLD_2_LEVEL_4:
    case WORLD_5_LEVEL_4:   return this->Fix_Offset(0x1DC0);
    case WORLD_3_LEVEL_1:   return this->Fix_Offset(0x1EE0);
    case WORLD_3_LEVEL_2:   return this->Fix_Offset(0x20C3);
    case WORLD_3_LEVEL_3:   return this->Fix_Offset(0x1E69);
    case WORLD_3_LEVEL_4:   return this->Fix_Offset(0x1DEF);
    case WORLD_4_LEVEL_1:   return this->Fix_Offset(0x1EAB);
    case WORLD_4_LEVEL_2:   return this->Fix_Offset(0x2115);
    case WORLD_4_LEVEL_3:   return this->Fix_Offset(0x1FB9);
    case WORLD_4_LEVEL_4:   return this->Fix_Offset(0x1DA7);
    case WORLD_5_LEVEL_1:   return this->Fix_Offset(0x1F8C);
    case WORLD_5_LEVEL_2:   return this->Fix_Offset(0x2045);
    case WORLD_6_LEVEL_1:   return this->Fix_Offset(0x2001);
    case WORLD_6_LEVEL_2:   return this->Fix_Offset(0x1EB9);
    case WORLD_6_LEVEL_3:   return this->Fix_Offset(0x1FDE);
    case WORLD_7_LEVEL_1:   return this->Fix_Offset(0x209E);
    case WORLD_7_LEVEL_4:   return this->Fix_Offset(0x1E1A);
    case WORLD_8_LEVEL_1:   return this->Fix_Offset(0x200B);
    case WORLD_8_LEVEL_2:   return this->Fix_Offset(0x2070);
    case WORLD_8_LEVEL_3:   return this->Fix_Offset(0x1E8E);
    case WORLD_8_LEVEL_4:   return this->Fix_Offset(0x1E2F);
    case UNDERGROUND_BONUS: return this->Fix_Offset(0x2143);
    case CLOUD_BONUS_1:     return this->Fix_Offset(0x1FB0);
    case CLOUND_BONUS_2:    return this->Fix_Offset(0x20BA);
    case UNDERWATER_BONUS:  return this->Fix_Offset(0x2170);
    case UNDERWATER_CASTLE: return this->Fix_Offset(0x21AB);
    case PIPE_INTRO:
    case WARP_ZONE:         return BAD_OFFSET; //these levels don't have enemies
    default:
        assert(false);
    }
    return BAD_OFFSET;
}

int Level_Offset::Fix_Offset(int offset) {
    switch (this->romType) {
    case DEFAULT: return offset; //nothing to do
    case FDS: return offset + 0x2155; //increment for Famicom
    case TRACK: return offset + 0x8000; //increment for Track combo cart
    case INVALID: //this should never happen
    default:
        assert(false);
    }
    return BAD_OFFSET;
}
