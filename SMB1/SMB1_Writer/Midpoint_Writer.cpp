#include "Midpoint_Writer.h"
#include "Level_Offset.h"
#include "Room_ID_Handler.h"
#include <QVector>
#include <assert.h>

Midpoint_Writer::Midpoint_Writer(QFile *file, Level_Offset *levelOffsets, Room_ID_Handler *roomIDHandler) {
    assert(file);
    assert(levelOffsets);
    assert(roomIDHandler);
    this->buffer = new QByteArray(16, ' ');
    this->file = file;
    this->levelOffsets = levelOffsets;
    this->roomIDHandler = roomIDHandler;
    this->currentLevel = Level::WORLD_1_LEVEL_1;
}

Midpoint_Writer::~Midpoint_Writer() {
    delete this->buffer;
}

bool Midpoint_Writer::Read_Midpoints() {
    assert(this->buffer);
    int offset = this->levelOffsets->Fix_Offset(0x11CD);
    if (!this->file->seek(offset)) return false;

    //Read the midpoints from the ROM
    qint64 ret = this->file->read(this->buffer->data(), 16);
    if (ret != 16 || this->buffer == NULL || this->buffer->size() != 16) return false;

    return true;
}

bool Midpoint_Writer::Write_Midpoints() {
    assert(this->buffer);
    int offset = this->levelOffsets->Fix_Offset(0x11CD);
    if (!this->file->seek(offset)) return false;

    //Write the midpoints to the ROM
    return this->file->write(this->buffer->data(), this->buffer->length()) == this->buffer->length();
}

void Midpoint_Writer::Set_Current_Level(Level::Level level) {
    this->currentLevel = level;
}

bool Midpoint_Writer::Set_Midpoint(int value) {
    if (value < 0x0 || value > 0xF) return false;
    QVector<int> *midpointIndexes = this->roomIDHandler->Get_Midpoint_Indexes_From_Level(this->currentLevel);
    if (!midpointIndexes || midpointIndexes->isEmpty()) return false;
    for (int i = 0; i < midpointIndexes->size(); ++i) {
        int index = midpointIndexes->at(i);
        bool highNibble = (index % 2 == 0);
        int properIndex = index / 2;

        //Write the nibble into the byte
        QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->at(properIndex)));
        if (highNibble) {
            Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 0, value);
        } else {
            Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 4, value);
        }

        unsigned char byte = Binary_Manipulator::BitArray_To_Hex(bits);
        this->buffer->data()[0] = static_cast<char>(byte);
    }
    return true;
}
