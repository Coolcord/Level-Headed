#include "Midpoint_Writer.h"
#include "Level_Offset.h"
#include "Room_ID_Handler.h"
#include <QVector>
#include <QDebug>
#include <assert.h>

Midpoint_Writer::Midpoint_Writer(QFile *file, Level_Offset *levelOffsets, Room_ID_Handler *roomIDHandler) {
    assert(file);
    assert(levelOffsets);
    assert(roomIDHandler);
    this->buffer = new QByteArray(16, 0x00);
    this->midpointMap = new QMap<Level::Level, int>();
    this->file = file;
    this->levelOffsets = levelOffsets;
    this->roomIDHandler = roomIDHandler;
    this->moreThan4LevelsPerWorld = false;
}

Midpoint_Writer::~Midpoint_Writer() {
    delete this->buffer;
    delete this->midpointMap;
}

bool Midpoint_Writer::Read_Midpoints() {
    assert(this->buffer);
    qint64 offset = this->levelOffsets->Fix_Offset(0x11CD);
    if (!this->file->seek(offset)) return false;

    //Read the midpoints from the ROM
    qint64 ret = this->file->read(this->buffer->data(), 16);
    if (ret != 16 || this->buffer == nullptr || this->buffer->size() != 16) return false;

    return true;
}

bool Midpoint_Writer::Write_Midpoints() {
    assert(this->buffer);
    qint64 offset = this->levelOffsets->Fix_Offset(0x11CD);
    if (!this->file->seek(offset)) return false;

    //Write the midpoints to the ROM
    return this->file->write(this->buffer->data(), this->buffer->length()) == this->buffer->length();
}

bool Midpoint_Writer::Get_Midpoint_At_Level(Level::Level level, int &midpoint) {
    QMap<Level::Level, int>::iterator iter = this->midpointMap->find(level);
    if (iter == this->midpointMap->end()) return false;
    midpoint = iter.value();
    return true;
}

bool Midpoint_Writer::Set_Midpoint(Level::Level level, int worldNum, int levelNum, int value) {
    //Swap worldNum with levelNum if using more than 4 levels per world
    if (this->moreThan4LevelsPerWorld) {
        int tmp = worldNum;
        worldNum = levelNum;
        levelNum = tmp;
    }

    //Make sure values are valid
    assert(levelNum > 0 && levelNum <= 8);
    assert(worldNum > 0 && worldNum <= 8);
    assert(levelNum <= 4 || worldNum <= 4);
    assert(worldNum*levelNum <= 32);
    if (value < 0 || value > 0xF) return false;

    //Perform calculations to get the index
    --worldNum;
    int index = (worldNum*2)+((levelNum-1)/2);
    bool highNibble = (levelNum%2==1);

    //Set the byte
    char byte = this->buffer->at(index);
    if (highNibble) byte = (byte&0x0F)+static_cast<char>(value*0x10);
    else byte = (byte&0xF0)+static_cast<char>(value);
    this->buffer->data()[index] = byte;
    this->midpointMap->insert(level, value);
    return true;
}

void Midpoint_Writer::Set_More_Than_4_Levels_Per_World(bool value) {
    this->moreThan4LevelsPerWorld = value;
}

