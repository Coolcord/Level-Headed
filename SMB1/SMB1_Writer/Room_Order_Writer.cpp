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
    int offset = this->levelOffset->Fix_Offset(0x1CCC);
    if (!this->file->seek(offset)) return false;

    //Read the Room Order Table from the ROM
    qint64 ret = this->file->read(this->buffer->data(), 36);
    if (ret != 36 || this->buffer == NULL || this->buffer->size() != 36) return false;

    this->Populate_Midpoint_Indexes_In_Handler();
    return true;
}

bool Room_Order_Writer::Write_Room_Order_Table() {
    assert(this->buffer);
    int offset = this->levelOffset->Fix_Offset(0x1CCC);
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

bool Room_Order_Writer::Set_Number_Of_Worlds(int value) {
    if (value < 0 || value > 8) return false;
    --value; //value should be 0 based
    QByteArray worldByte;
    worldByte.append(static_cast<char>(value));

    //Three different offsets must be patched for Number of Worlds to be set properly
    if (!Write_Number_Of_Worlds_To_Offset(0x0438, worldByte)) return false;
    if (!Write_Number_Of_Worlds_To_Offset(0x047A, worldByte)) return false;
    if (!Write_Number_Of_Worlds_To_Offset(0x6A27, worldByte)) return false;

    return true;
}

QVector<unsigned char> *Room_Order_Writer::Get_Midpoints_From_Room_Order_Table(unsigned char id) {
    QVector<unsigned char> *midpoints = new QVector<unsigned char>();
    int midpointIndex = 0;
    for (int i = 0; i < 36; ++i) {
        unsigned char roomID = static_cast<unsigned char>(this->buffer->data()[i]);
        if (roomID == id) midpoints->append(midpointIndex);
        if (roomID != this->roomIDHandler->roomIDs->value(Level::PIPE_INTRO)) ++midpointIndex; //don't count the pipe cutscenes
    }
    return midpoints;
}

bool Room_Order_Writer::Write_Number_Of_Worlds_To_Offset(int offset, const QByteArray &worldByte) {
    offset = this->levelOffset->Fix_Offset(offset);
    if (!this->file->seek(offset)) return false;
    return (this->file->write(worldByte) == 1);
}

void Room_Order_Writer::Populate_Midpoint_Indexes_In_Handler() {
    this->roomIDHandler->midpointIndexes->clear();
    foreach (unsigned char value, this->roomIDHandler->roomIDs->values()) {
        this->roomIDHandler->midpointIndexes->insert(static_cast<int>(value), this->Get_Midpoints_From_Room_Order_Table(value));
    }
}
