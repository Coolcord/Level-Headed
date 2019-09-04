#include "Level_Offset.h"
#include "ROM_Checksum.h"
#include "Room_ID_Handler.h"
#include "Room_Address_Writer.h"
#include <assert.h>

#include <QDebug>

Level_Offset::Level_Offset(QFile *file, ROM_Type::ROM_Type romType) {
    assert(file);
    if (!file->isOpen()) {
        throw "File is not open";
    }
    this->file = file;
    this->romType = romType;
    this->roomIDHandler = nullptr;
    this->roomAddressWriter = nullptr;
}

void Level_Offset::Set_Extras(Room_ID_Handler *roomIDHandler, Room_Address_Writer *roomAddressWriter) {
    assert(roomIDHandler);
    assert(roomAddressWriter);
    this->roomIDHandler = roomIDHandler;
    this->roomAddressWriter = roomAddressWriter;
}

qint64 Level_Offset::Get_Level_Object_Offset(Level::Level level) {
    assert(this->roomIDHandler);
    assert(this->roomAddressWriter);

    unsigned char roomID = 0;
    assert(this->roomIDHandler->Get_Room_ID_From_Level(level, roomID));
    qint64 offset = this->roomAddressWriter->Get_Room_ID_Object_Offset_From_Table(roomID);

    //Convert from RAM value to ROM value
    switch (this->romType) {
    case ROM_Type::INVALID:     assert(false); break;
    case ROM_Type::FDS:         offset -= 0x3EA1; break;
    case ROM_Type::EUROPE:      offset -= 0x7FEE; break;
    case ROM_Type::DEFAULT:     offset -= 0x7FEE; break;
    case ROM_Type::BILL_KILL_2: offset -= 0x7FEE; break;
    case ROM_Type::COOP_CGTI_1: offset += 0x12; break;
    }
    return offset;
}

qint64 Level_Offset::Get_Level_Enemy_Offset(Level::Level level) {
    assert(this->roomIDHandler);
    assert(this->roomAddressWriter);

    switch (level) {
    case Level::PIPE_INTRO:
    case Level::WARP_ZONE:         return BAD_OFFSET; //these levels don't have enemies
    default: break;
    }
    unsigned char roomID = 0;
    assert(this->roomIDHandler->Get_Room_ID_From_Level(level, roomID));
    qint64 offset = this->roomAddressWriter->Get_Room_ID_Enemy_Offset_From_Table(roomID);

    //Convert from RAM value to ROM value
    switch (this->romType) {
    case ROM_Type::INVALID:     assert(false); break;
    case ROM_Type::FDS:         offset -= 0x3EA3; break;
    case ROM_Type::EUROPE:      offset -= 0x7FF0; break;
    case ROM_Type::DEFAULT:     offset -= 0x7FF0; break;
    case ROM_Type::BILL_KILL_2: offset -= 0x7FF0; break;
    case ROM_Type::COOP_CGTI_1: offset += 0x10; break;
    }
    return offset;
}

qint64 Level_Offset::Fix_Offset(qint64 offset) {
    switch (this->romType) {
    case ROM_Type::BILL_KILL_2:
    case ROM_Type::DEFAULT:
        return offset; //nothing to do
    case ROM_Type::EUROPE:
        if (offset < 0x1AF5) return offset;
        if (offset < 0x1C08) return offset + 0x2;
        if (offset < 0x391A) return offset;
        if (offset < 0x406E) return offset + 0x5;
        if (offset < 0x4F48) return offset + 0x6;
        if (offset < 0x56E6) return offset - 0x38;
        if (offset < 0x594F) return offset;
        if (offset < 0x5D5B) return offset + 0x2;
        if (offset < 0x6453) return offset + 0x8;
        if (offset < 0x72DA) return offset + 0x7;
        if (offset < 0x7F9E) return offset + 0x1;
        return offset;
    case ROM_Type::FDS:
        if (offset < 0x0144) return offset + 0x214C;
        if (offset < 0x1110) return offset + 0x2153;
        if (offset < 0x1CB0) return offset + 0x2157;
        if (offset < 0x4008) return offset + 0x2155;
        if (offset < 0x5A24) return offset + 0x2166;
        if (offset < 0x645C) return offset + 0x2163;
        if (offset < 0x7284) return offset + 0x2162;
        if (offset < 0x8010) return offset + 0x215E;
        if (offset < 0xA010) return offset - 0x7EC4;
        return offset + 0x2155;
    case ROM_Type::COOP_CGTI_1:
        if (offset < 0x0258) return offset + 0x7DFD;
        if (offset < 0x02B4) return offset + 0x7E0B;
        if (offset < 0x02DB) return offset + 0x7E11;
        if (offset < 0x0419) return offset + 0x7E57;
        if (offset < 0x044A) return offset + 0x7E37;
        if (offset < 0x047B) return offset + 0x7E61;
        if (offset < 0x0506) return offset + 0x7E5D;
        if (offset < 0x06F9) return offset + 0x7E5B;
        if (offset < 0x0D22) return offset + 0x7E75;
        if (offset < 0x0E08) return offset + 0x7E6F;
        if (offset < 0x103A) return offset + 0x7E02;
        if (offset < 0x105B) return offset + 0x7E01;
        if (offset < 0x107E) return offset + 0x7DFD;
        if (offset < 0x1120) return offset + 0x7DE2;
        if (offset < 0x1141) return offset + 0x7DE6;
        if (offset < 0x11E0) return offset + 0x7DFE;
        if (offset < 0x11F6) return offset + 0x7E19;
        if (offset < 0x120B) return offset + 0x7E1E;
        if (offset < 0x1264) return offset + 0x7E20;
        if (offset < 0x1308) return offset + 0x7DEC;
        if (offset < 0x148E) return offset + 0x7DF2;
        if (offset < 0x152D) return offset + 0x7DF1;
        if (offset < 0x161A) return offset + 0x7DF0;
        if (offset < 0x176A) return offset + 0x7DEE;
        if (offset < 0x1796) return offset + 0x7DEB;
        if (offset < 0x184B) return offset + 0x7DEA;
        if (offset < 0x18CA) return offset + 0x7DE7;
        if (offset < 0x1959) return offset + 0x7DE5;
        if (offset < 0x1BCE) return offset + 0x7DD6;
        if (offset < 0x1C80) return offset + 0x7DD3;
        if (offset < 0x2EEB) return offset + 0x7DD1;
        if (offset < 0x2F62) return offset + 0x7DF4;
        if (offset < 0x2F74) return offset + 0x7E39;
        if (offset < 0x3C13) return offset + 0x7FCB;
        if (offset < 0x3C37) return offset + 0x7FCC;
        if (offset < 0x3F2F) return offset + 0x802E;
        if (offset < 0x4469) return offset + 0x805D;
        if (offset < 0x450B) return offset + 0x8065;
        if (offset < 0x47FB) return offset + 0x807D;
        if (offset < 0x4850) return offset + 0x8078;
        if (offset < 0x4E4F) return offset + 0x80CD;
        if (offset < 0x5135) return offset + 0x8090;
        if (offset < 0x5159) return offset + 0x80C0;
        if (offset < 0x5562) return offset + 0x808F;
        if (offset < 0x5813) return offset + 0x8124;
        if (offset < 0x5830) return offset + 0x8127;
        if (offset < 0x58A5) return offset + 0x8149;
        if (offset < 0x5CFA) return offset + 0x81DB;
        if (offset < 0x6509) return offset + 0x826D;
        if (offset < 0x6604) return offset + 0x899F;
        if (offset < 0x6B0D) return offset + 0x8200;
        if (offset < 0x6EE3) return offset + 0x81E4;
        return offset + 0x8000;
    case ROM_Type::INVALID:
        assert(false);
        break; //this should never happen
    }
    return BAD_OFFSET;
}

ROM_Type::ROM_Type Level_Offset::Get_ROM_Type() {
    return this->romType;
}
