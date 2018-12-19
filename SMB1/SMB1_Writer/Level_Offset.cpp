#include "Level_Offset.h"
#include "ROM_Checksum.h"
#include "Room_ID_Handler.h"
#include "Room_Address_Writer.h"
#include <assert.h>

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
    unsigned int offset = this->roomAddressWriter->Get_Room_ID_Object_Offset_From_Table(roomID);
    offset -= 0x7FEE; //convert from RAM value to ROM value
    return this->Fix_Offset(offset);
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
    unsigned int offset = this->roomAddressWriter->Get_Room_ID_Enemy_Offset_From_Table(roomID);
    offset -= 0x7FF0; //convert from RAM value to ROM value
    return this->Fix_Offset(offset);
}

qint64 Level_Offset::Fix_Offset(qint64 offset) {
    switch (this->romType) {
    case ROM_Type::DEFAULT: return offset; //nothing to do
    case ROM_Type::EUROPE:
        if (offset < 0x6A27) return offset;
        else return offset + 0x7;
    case ROM_Type::TRACK: return offset + 0x8000; //increment for Track combo cart
    case ROM_Type::FDS:
        if (offset < 0x0489) return offset + 0x2153;
        if (offset < 0x11DD) return offset + 0x2157;
        if (offset < 0x2EEB) return offset + 0x2155;
        if (offset < 0x6A33) return offset + 0x2162;
        else return offset + 0x2155; //increment for Famicom
    case ROM_Type::INVALID:
        assert(false);
        break; //this should never happen
    }
    return BAD_OFFSET;
}

ROM_Type::ROM_Type Level_Offset::Get_ROM_Type() {
    return this->romType;
}
