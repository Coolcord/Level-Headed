#include "Room_Address_Writer.h"
#include "Level_Offset.h"
#include <assert.h>

Room_Address_Writer::Room_Address_Writer(QFile *file, Level_Offset *levelOffset) {
    assert(file);
    assert(levelOffset);
    this->file = file;
    this->levelOffset = levelOffset;
    this->objectsHeaderBuffer = new QByteArray(4, ' ');
    this->lowObjectBuffer = new QByteArray(34, ' ');
    this->highObjectBuffer = new QByteArray(34, ' ');
    this->enemiesHeaderBuffer = new QByteArray(4, ' ');
    this->lowEnemyBuffer = new QByteArray(34, ' ');
    this->highEnemyBuffer = new QByteArray(34, ' ');
}

Room_Address_Writer::~Room_Address_Writer() {
    delete this->objectsHeaderBuffer;
    delete this->lowObjectBuffer;
    delete this->highObjectBuffer;
    delete this->enemiesHeaderBuffer;
    delete this->lowEnemyBuffer;
    delete this->highEnemyBuffer;
}

bool Room_Address_Writer::Read_Room_Address_Tables() {
    //Read the Objects Address Header
    if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_HEADER, 4, this->objectsHeaderBuffer)) return false;

    //Read the Objects Address Low Byte Table
    if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_LOW_BYTE, 34, this->lowObjectBuffer)) return false;

    //Read the Objects Address High Byte Table
    if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_HIGH_BYTE, 34, this->highObjectBuffer)) return false;

    //Read the Enemies Address Header
    if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_HEADER, 4, this->enemiesHeaderBuffer)) return false;

    //Read the Enemies Address Low Byte Table
    if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_LOW_BYTE, 34, this->lowEnemyBuffer)) return false;

    //Read the Enemies Address High Byte Table
    if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_HIGH_BYTE, 34, this->highEnemyBuffer)) return false;

    return true;
}

bool Room_Address_Writer::Write_Room_Address_Tables() {
    //Write the Objects Address Header
    if (!this->Write_Buffer(this->OBJECT_ADDRESS_HEADER, this->objectsHeaderBuffer)) return false;

    //Write the Objects Address Low Byte Table
    if (!this->Write_Buffer(this->OBJECT_ADDRESS_LOW_BYTE, this->lowObjectBuffer)) return false;

    //Write the Objects Address High Byte Table
    if (!this->Write_Buffer(this->OBJECT_ADDRESS_HIGH_BYTE, this->highObjectBuffer)) return false;

    //Write the Enemies Address Header
    if (!this->Write_Buffer(this->ENEMY_ADDRESS_HEADER, this->enemiesHeaderBuffer)) return false;

    //Write the Enemies Address Low Byte Table
    if (!this->Write_Buffer(this->ENEMY_ADDRESS_LOW_BYTE, this->lowEnemyBuffer)) return false;

    //Write the Enemies Address High Byte Table
    if (!this->Write_Buffer(this->ENEMY_ADDRESS_HIGH_BYTE, this->highEnemyBuffer)) return false;

    return true;
}

bool Room_Address_Writer::Read_Into_Buffer(int offset, int amount, QByteArray *buffer) {
    if (!buffer) return false;
    if (!this->file->seek(this->levelOffset->Fix_Offset(offset))) return false;

    //Read the data from the ROM
    qint64 ret = this->file->read(buffer->data(), amount);
    if (ret != amount || buffer == NULL || buffer->size() != amount) return false;
    return true;
}

bool Room_Address_Writer::Write_Buffer(int offset, QByteArray *buffer) {
    assert(this->file);
    if (!buffer) return false;
    if (!this->file->seek(this->levelOffset->Fix_Offset(offset))) return false;
    return this->file->write(buffer->data(), buffer->length()) == buffer->length();
}
