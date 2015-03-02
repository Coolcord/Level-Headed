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
    //Read the Object Address Header
    if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_HEADER, 4, this->objectsHeaderBuffer)) return false;

    //Read the Objects Address Low Byte Table
    if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_LOW_BYTE, 34, this->lowObjectBuffer)) return false;

    //Read the Objects Address High Byte Table
    if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_HIGH_BYTE, 34, this->highObjectBuffer)) return false;

    //Read the Enemy Address Header
    if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_HEADER, 4, this->enemiesHeaderBuffer)) return false;

    //Read the Enemies Address Low Byte Table
    if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_LOW_BYTE, 34, this->lowEnemyBuffer)) return false;

    //Read the Enemies Address High Byte Table
    if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_HIGH_BYTE, 34, this->highEnemyBuffer)) return false;

    return true;
}

bool Room_Address_Writer::Write_Room_Address_Tables() {
    //Write the Object Address Header
    if (!this->Write_Buffer(this->OBJECT_ADDRESS_HEADER, this->objectsHeaderBuffer)) return false;

    //Write the Objects Address Low Byte Table
    if (!this->Write_Buffer(this->OBJECT_ADDRESS_LOW_BYTE, this->lowObjectBuffer)) return false;

    //Write the Objects Address High Byte Table
    if (!this->Write_Buffer(this->OBJECT_ADDRESS_HIGH_BYTE, this->highObjectBuffer)) return false;

    //Write the Enemy Address Header
    if (!this->Write_Buffer(this->ENEMY_ADDRESS_HEADER, this->enemiesHeaderBuffer)) return false;

    //Write the Enemies Address Low Byte Table
    if (!this->Write_Buffer(this->ENEMY_ADDRESS_LOW_BYTE, this->lowEnemyBuffer)) return false;

    //Write the Enemies Address High Byte Table
    if (!this->Write_Buffer(this->ENEMY_ADDRESS_HIGH_BYTE, this->highEnemyBuffer)) return false;

    return true;
}

unsigned int Room_Address_Writer::Get_Room_ID_Object_Offset_From_Table(unsigned char roomID) {
    //Get the index in the table
    unsigned char attribute = ((roomID>>5)&0x3);
    unsigned char index = static_cast<unsigned char>(this->objectsHeaderBuffer->data()[attribute]);
    index += (roomID&0x1F);

    //Calculate the offset
    unsigned int offset = static_cast<unsigned int>(static_cast<unsigned char>(this->highObjectBuffer->data()[index]));
    offset *= 0x100; //move to the high byte
    offset += static_cast<unsigned int>(static_cast<unsigned char>(this->lowObjectBuffer->data()[index]));
    return offset;
}

unsigned int Room_Address_Writer::Get_Room_ID_Enemy_Offset_From_Table(unsigned char roomID) {
    //Get the index in the table
    unsigned char attribute = ((roomID>>5)&0x3);
    unsigned char index = static_cast<unsigned char>(this->enemiesHeaderBuffer->data()[attribute]);
    index += (roomID&0x1F);

    //Calculate the offset
    unsigned int offset = static_cast<unsigned int>(static_cast<unsigned char>(this->highEnemyBuffer->data()[index]));
    offset *= 0x100; //move to the high byte
    offset += static_cast<unsigned int>(static_cast<unsigned char>(this->lowEnemyBuffer->data()[index]));
    return offset;
}

bool Room_Address_Writer::Fix_Level_Addresses(int fromOffset, int toOffset, int numBytes) {
    if (!this->Fix_Level_Address_Buffer(fromOffset, toOffset, numBytes, this->lowObjectBuffer, this->highObjectBuffer, false)) return false;
    return this->Fix_Level_Address_Buffer(fromOffset, toOffset, numBytes, this->lowEnemyBuffer, this->highEnemyBuffer, true);
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

bool Room_Address_Writer::Fix_Level_Address_Buffer(int fromOffset, int toOffset, int numBytes, QByteArray *lowByteBuffer, QByteArray *highByteBuffer, bool enemies) {
    assert(lowByteBuffer);
    assert(highByteBuffer);
    int byteBufferSize = lowByteBuffer->size();
    assert(byteBufferSize == highByteBuffer->size());

    //Determine which direction to increment
    bool add = false;
    assert(fromOffset != toOffset);
    if (toOffset < fromOffset) add = true;

    //Fix all of the offsets in the buffers
    for (int i = 0; i < byteBufferSize; ++i) {
        //Calculate the offset
        unsigned int offset = static_cast<unsigned int>(static_cast<unsigned char>(highByteBuffer->data()[i]));
        offset *= 0x100; //move to the high byte
        offset += static_cast<unsigned int>(static_cast<unsigned char>(lowByteBuffer->data()[i]));

        //Convert from RAM value to ROM value
        unsigned int fixedOffset = offset;
        if (enemies) fixedOffset -= 0x7FF0;
        else fixedOffset -= 0x7FEE;
        fixedOffset = static_cast<unsigned int>(this->levelOffset->Fix_Offset(fixedOffset));

        //Determine if any calculations are necessary
        if (fixedOffset < static_cast<unsigned int>(fromOffset) && fixedOffset < static_cast<unsigned int>(toOffset)) continue;
        if (fixedOffset > static_cast<unsigned int>(fromOffset) && fixedOffset > static_cast<unsigned int>(toOffset)) continue;
        if (add) {
            if (fixedOffset == static_cast<unsigned int>(toOffset)) continue;
        } else {
            if (fixedOffset == static_cast<unsigned int>(fromOffset)) continue;
        }

        //Fix the offset
        if (add) {
            offset += numBytes;
        } else {
            assert(offset >= static_cast<unsigned int>(numBytes));
            offset -= numBytes;
        }

        //Update the offset in the buffer
        lowByteBuffer->data()[i] = static_cast<char>(static_cast<unsigned char>(offset&0x00FF));
        highByteBuffer->data()[i] = static_cast<char>(static_cast<unsigned char>((offset&0xFF00)/0x100));
    }

    return true;
}
