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
    this->file->seek(0); //make sure to start at the beginning of the file
    QByteArray buffer = this->file->readAll();
    if (buffer == NULL || buffer.size() == 0) return INVALID;

    //Verify if the ROM is a supported version via checksum
    QString hash = QString(QCryptographicHash::hash(buffer, QCryptographicHash::Sha512).toHex().toUpper());
    if (hash == ROM_Checksum::FDS) return FDS;
    else if (hash == ROM_Checksum::TRACK) return TRACK;
    else if (ROM_Checksum::Is_ROM_DEFAULT(hash)) return DEFAULT;
    else return INVALID;
}

bool Level_Offset::Check_ROM_Header() {
    if (!this->file->seek(0)) return false;
    QByteArray buffer(4, ' ');
    if (this->file->read(buffer.data(), 4) != 4) return false;
    if (buffer.data() == NULL || buffer.size() != 4) return false;

    if (this->Check_NES_ROM_Header(&buffer)) return true;
    return this->Check_FDS_ROM_Header(&buffer);
}

bool Level_Offset::Check_NES_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "NES"
    if (static_cast<unsigned char>(buffer->at(0)) != 0x4E) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x45) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x53) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x1A;
}

bool Level_Offset::Check_FDS_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "FDS"
    if (static_cast<unsigned char>(buffer->at(0)) != 0x46) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x44) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x53) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x1A;
}

int Level_Offset::Get_Level_Object_Offset(Level::Level level) {
    switch (level) {
    case Level::WORLD_1_LEVEL_1:   return this->Fix_Offset(0x26A0);
    case Level::WORLD_1_LEVEL_2:   return this->Fix_Offset(0x2C47);
    case Level::WORLD_1_LEVEL_3:
    case Level::WORLD_5_LEVEL_3:   return this->Fix_Offset(0x2705);
    case Level::WORLD_1_LEVEL_4:
    case Level::WORLD_6_LEVEL_4:   return this->Fix_Offset(0x21C1);
    case Level::WORLD_2_LEVEL_1:   return this->Fix_Offset(0x27DF);
    case Level::WORLD_2_LEVEL_2:
    case Level::WORLD_7_LEVEL_2:   return this->Fix_Offset(0x2E57);
    case Level::WORLD_2_LEVEL_3:
    case Level::WORLD_7_LEVEL_3:   return this->Fix_Offset(0x275A);
    case Level::WORLD_2_LEVEL_4:
    case Level::WORLD_5_LEVEL_4:   return this->Fix_Offset(0x22A1);
    case Level::WORLD_3_LEVEL_1:   return this->Fix_Offset(0x262B);
    case Level::WORLD_3_LEVEL_2:   return this->Fix_Offset(0x2C14);
    case Level::WORLD_3_LEVEL_3:   return this->Fix_Offset(0x247D);
    case Level::WORLD_3_LEVEL_4:   return this->Fix_Offset(0x2314);
    case Level::WORLD_4_LEVEL_1:   return this->Fix_Offset(0x2549);
    case Level::WORLD_4_LEVEL_2:   return this->Fix_Offset(0x2CEA);
    case Level::WORLD_4_LEVEL_3:   return this->Fix_Offset(0x28A1);
    case Level::WORLD_4_LEVEL_4:   return this->Fix_Offset(0x2222);
    case Level::WORLD_5_LEVEL_1:   return this->Fix_Offset(0x284D);
    case Level::WORLD_5_LEVEL_2:   return this->Fix_Offset(0x2AA4);
    case Level::WORLD_6_LEVEL_1:   return this->Fix_Offset(0x296D);
    case Level::WORLD_6_LEVEL_2:   return this->Fix_Offset(0x259C);
    case Level::WORLD_6_LEVEL_3:   return this->Fix_Offset(0x2908);
    case Level::WORLD_7_LEVEL_1:   return this->Fix_Offset(0x2B90);
    case Level::WORLD_7_LEVEL_4:   return this->Fix_Offset(0x2381);
    case Level::WORLD_8_LEVEL_1:   return this->Fix_Offset(0x2A11);
    case Level::WORLD_8_LEVEL_2:   return this->Fix_Offset(0x2B17);
    case Level::WORLD_8_LEVEL_3:   return this->Fix_Offset(0x24E0);
    case Level::WORLD_8_LEVEL_4:   return this->Fix_Offset(0x240C);
    case Level::PIPE_INTRO:        return this->Fix_Offset(0x2844);
    case Level::UNDERGROUND_BONUS: return this->Fix_Offset(0x2D8B);
    case Level::CLOUD_BONUS_1:     return this->Fix_Offset(0x288C);
    case Level::CLOUND_BONUS_2:    return this->Fix_Offset(0x2BE9);
    case Level::UNDERWATER_BONUS:  return this->Fix_Offset(0x29E0);
    case Level::WARP_ZONE:         return this->Fix_Offset(0x2E18);
    case Level::UNDERWATER_CASTLE: return this->Fix_Offset(0x2ED2);
    default:
        assert(false);
    }
    return BAD_OFFSET;
}

int Level_Offset::Get_Level_Enemy_Offset(Level::Level level) {
    switch (level) {
    case Level::WORLD_1_LEVEL_1:   return this->Fix_Offset(0x1F11);
    case Level::WORLD_1_LEVEL_2:   return this->Fix_Offset(0x20E8);
    case Level::WORLD_1_LEVEL_3:
    case Level::WORLD_5_LEVEL_3:   return this->Fix_Offset(0x1F2F);
    case Level::WORLD_1_LEVEL_4:
    case Level::WORLD_6_LEVEL_4:   return this->Fix_Offset(0x1D80);
    case Level::WORLD_2_LEVEL_1:   return this->Fix_Offset(0x1F61);
    case Level::WORLD_2_LEVEL_2:
    case Level::WORLD_7_LEVEL_2:   return this->Fix_Offset(0x2181);
    case Level::WORLD_2_LEVEL_3:
    case Level::WORLD_7_LEVEL_3:   return this->Fix_Offset(0x1F4C);
    case Level::WORLD_2_LEVEL_4:
    case Level::WORLD_5_LEVEL_4:   return this->Fix_Offset(0x1DC0);
    case Level::WORLD_3_LEVEL_1:   return this->Fix_Offset(0x1EE0);
    case Level::WORLD_3_LEVEL_2:   return this->Fix_Offset(0x20C3);
    case Level::WORLD_3_LEVEL_3:   return this->Fix_Offset(0x1E69);
    case Level::WORLD_3_LEVEL_4:   return this->Fix_Offset(0x1DEF);
    case Level::WORLD_4_LEVEL_1:   return this->Fix_Offset(0x1EAB);
    case Level::WORLD_4_LEVEL_2:   return this->Fix_Offset(0x2115);
    case Level::WORLD_4_LEVEL_3:   return this->Fix_Offset(0x1FB9);
    case Level::WORLD_4_LEVEL_4:   return this->Fix_Offset(0x1DA7);
    case Level::WORLD_5_LEVEL_1:   return this->Fix_Offset(0x1F8C);
    case Level::WORLD_5_LEVEL_2:   return this->Fix_Offset(0x2045);
    case Level::WORLD_6_LEVEL_1:   return this->Fix_Offset(0x2001);
    case Level::WORLD_6_LEVEL_2:   return this->Fix_Offset(0x1EB9);
    case Level::WORLD_6_LEVEL_3:   return this->Fix_Offset(0x1FDE);
    case Level::WORLD_7_LEVEL_1:   return this->Fix_Offset(0x209E);
    case Level::WORLD_7_LEVEL_4:   return this->Fix_Offset(0x1E1A);
    case Level::WORLD_8_LEVEL_1:   return this->Fix_Offset(0x200B);
    case Level::WORLD_8_LEVEL_2:   return this->Fix_Offset(0x2070);
    case Level::WORLD_8_LEVEL_3:   return this->Fix_Offset(0x1E8E);
    case Level::WORLD_8_LEVEL_4:   return this->Fix_Offset(0x1E2F);
    case Level::UNDERGROUND_BONUS: return this->Fix_Offset(0x2143);
    case Level::CLOUD_BONUS_1:     return this->Fix_Offset(0x1FB0);
    case Level::CLOUND_BONUS_2:    return this->Fix_Offset(0x20BA);
    case Level::UNDERWATER_BONUS:  return this->Fix_Offset(0x2170);
    case Level::UNDERWATER_CASTLE: return this->Fix_Offset(0x21AB);
    case Level::PIPE_INTRO:
    case Level::WARP_ZONE:         return BAD_OFFSET; //these levels don't have enemies
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
