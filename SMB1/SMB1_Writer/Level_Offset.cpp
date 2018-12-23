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
    default:                    offset -= 0x7FEE; break;
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
    default:                    offset -= 0x7FF0; break;
    case ROM_Type::COOP_CGTI_1: offset += 0x10; break;
    }
    return offset;
}

qint64 Level_Offset::Fix_Offset(qint64 offset) {
    switch (this->romType) {
    case ROM_Type::DEFAULT: return offset; //nothing to do
    case ROM_Type::DUCK:
        if (offset < 0x9EA7) return offset;
        return offset + 0x8000;
    case ROM_Type::TRACK:
        if (offset < 0x9EA7) return offset + 0x8000;
        return offset + 0x20000; //increment for Track combo cart
    case ROM_Type::COOP_CGTI_1:
        if (offset < 0x02B4) return offset + 0x7E0B;
        if (offset < 0x0419) return offset + 0x7E57;
        if (offset < 0x047B) return offset + 0x7E61;
        if (offset < 0x0D22) return offset + 0x7E75;
        if (offset < 0x105B) return offset + 0x7E01;
        if (offset < 0x107E) return offset + 0x7DFD;
        if (offset < 0x11DE) return offset + 0x7DFE;
        if (offset < 0x1206) return offset + 0x7E1D;
        if (offset < 0x1308) return offset + 0x7DEC;
        if (offset < 0x161A) return offset + 0x7DF0;
        if (offset < 0x1C80) return offset + 0x7DD3;
        if (offset < 0x2EEB) return offset + 0x7DD1;
        if (offset < 0x3F2F) return offset + 0x802E;
        if (offset < 0x4450) return offset + 0x805D;
        if (offset < 0x450B) return offset + 0x8065;
        if (offset < 0x5130) return offset + 0x8090;
        if (offset < 0x5CFA) return offset + 0x81DB;
        if (offset < 0x6509) return offset + 0x826D;
        if (offset < 0x6A29) return offset + 0x8201;
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
