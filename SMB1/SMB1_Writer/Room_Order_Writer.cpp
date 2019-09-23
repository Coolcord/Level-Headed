#include "Room_Order_Writer.h"
#include "Level_Offset.h"
#include "Room_ID_Handler.h"
#include <assert.h>

Room_Order_Writer::Room_Order_Writer(QFile *file, Level_Offset *levelOffsets, Room_ID_Handler *roomIDHandler) {
    assert(file);
    assert(roomIDHandler);
    assert(levelOffsets);
    this->buffer = new QByteArray(36, ' ');
    this->file = file;
    this->roomIDHandler = roomIDHandler;
    this->levelOffset = levelOffsets;
    this->currentByte = 0;
}

Room_Order_Writer::~Room_Order_Writer() {
    delete this->buffer;
}

bool Room_Order_Writer::Read_Room_Order_Table() {
    assert(this->buffer);
    qint64 offset = this->levelOffset->Fix_Offset(0x1CCC);
    if (!this->file->seek(offset)) return false;

    //Read the Room Order Table from the ROM
    qint64 ret = this->file->read(this->buffer->data(), 36);
    if (ret != 36 || this->buffer == nullptr || this->buffer->size() != 36) return false;

    this->Populate_Midpoint_Indexes_In_Handler();
    return true;
}

bool Room_Order_Writer::Write_Room_Order_Table() {
    assert(this->buffer);

    //Fix the Room Order Table Header First
    if (!this->Fix_Room_Order_Table_Header()) return false;

    //Get the Room Order Table Offset
    qint64 offset = this->levelOffset->Fix_Offset(0x1CCC);
    if (!this->file->seek(offset)) return false;

    //Write the Room Order Table to the ROM
    return this->file->write(this->buffer->data(), this->buffer->length()) == this->buffer->length();
}

bool Room_Order_Writer::Set_Next_Level(Level::Level level) {
    if (this->currentByte >= 36) return false; //no more space!
    unsigned char roomID = 0;
    if (!this->roomIDHandler->Get_Room_ID_From_Level(level, roomID)) return false;
    this->buffer->data()[this->currentByte] = static_cast<char>(roomID); //write the byte
    ++this->currentByte;
    this->Populate_Midpoint_Indexes_In_Handler();
    return true;
}

QVector<unsigned char> *Room_Order_Writer::Get_Midpoints_From_Room_Order_Table(unsigned char id) {
    QVector<unsigned char> *midpoints = new QVector<unsigned char>();
    int midpointIndex = 0;
    for (int i = 0; i < 36 && midpointIndex < 32; ++i) {
        unsigned char roomID = static_cast<unsigned char>(this->buffer->data()[i]);
        if (roomID == id) midpoints->append(static_cast<unsigned char>(midpointIndex));
        if (roomID != this->roomIDHandler->roomIDs->value(Level::PIPE_INTRO)) ++midpointIndex; //don't count the pipe cutscenes
    }
    return midpoints;
}

bool Room_Order_Writer::Write_Bytes_To_Offset(qint64 offset, const QByteArray &bytes) {
    offset = this->levelOffset->Fix_Offset(offset);
    if (!this->file->seek(offset)) return false;
    return (this->file->write(bytes) == bytes.size());
}

void Room_Order_Writer::Populate_Midpoint_Indexes_In_Handler() {
    //Deallocate all of the QVectors
    foreach (QVector<unsigned char> *vector, this->roomIDHandler->midpointIndexes->values()) {
        delete vector;
    }
    this->roomIDHandler->midpointIndexes->clear();
    foreach (unsigned char value, this->roomIDHandler->roomIDs->values()) {
        this->roomIDHandler->midpointIndexes->insert(static_cast<unsigned char>(value), this->Get_Midpoints_From_Room_Order_Table(value));
    }
}

bool Room_Order_Writer::Fix_Room_Order_Table_Header() {
    QByteArray header(8, ' ');
    //Get the Room Order Table Header Offset
    qint64 offset = this->levelOffset->Fix_Offset(0x1CC4);
    if (!this->file->seek(offset)) return false;

    //Read the Room Order Table Header from the ROM
    qint64 ret = this->file->read(header.data(), 8);
    if (ret != 8 || header.size() != 8) return false;

    //Scan each level for Axes or Flagpoles to determine where the end of each world is
    unsigned char levels = 0;
    unsigned char world = 1;
    //Assume first byte is always 0
    header.data()[0] = static_cast<char>(0);
    for (int i = 0; i < 36; ++i) {
        Level::Level level = Level::WORLD_1_LEVEL_1;
        bool endOfWorld = false;
        assert(this->roomIDHandler->Get_Level_From_Room_ID(static_cast<unsigned char>(this->buffer->data()[i]&0x7F), level));
        assert(this->Scan_Level_For_End_Objects(level, endOfWorld));
        ++levels;
        if (endOfWorld) {
            if (world < 8) header.data()[world] = static_cast<char>(levels);
            ++world;
        }
    }
    //Fill the unused bytes
    for (unsigned char i = world; i < 8; ++i) {
        header.data()[i] = static_cast<char>(levels);
    }


    //Write the Room Order Table to the ROM
    if (!this->file->seek(offset)) return false;
    return this->file->write(header.data(), header.length()) == header.length();
}

bool Room_Order_Writer::Scan_Level_For_End_Objects(Level::Level level, bool &endOfWorld) {
    qint64 offset = this->levelOffset->Get_Level_Object_Offset(level);
    if (offset == BAD_OFFSET) return false;
    if (!this->file->seek(offset)) return false;

    QByteArray buffer(2, ' ');
    qint64 ret = this->file->read(buffer.data(), 2);
    while (ret == 2 && buffer.data() != nullptr
           && static_cast<unsigned char>(buffer.data()[0]) != 0xFD) {
        switch (static_cast<unsigned char>(buffer.data()[1])) {
        //FlagPole
        case 0x8D:
        case 0x0D:
            if ((static_cast<unsigned char>(buffer.data()[0])&0x0F) < 0x0C) {
                endOfWorld = false;
                return true;
            }
            break;
        //FlagPole
        case 0xC1:
        case 0x41:
            if ((static_cast<unsigned char>(buffer.data()[0])&0x0F) == 0x0D) {
                endOfWorld = false;
                return true;
            }
            break;
        //Axe
        case 0xC2:
        case 0x42:
            if ((static_cast<unsigned char>(buffer.data()[0])&0x0D) == 0x0D) {
                endOfWorld = true;
                return true;
            }
            break;
        default:
            break;
        }
        ret = this->file->read(buffer.data(), 2);
    }
    endOfWorld = false;
    return true; //unable to find an end object, so assume that it is a normal level
}
