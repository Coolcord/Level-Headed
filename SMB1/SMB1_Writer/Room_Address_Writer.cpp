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
    //Read the Headers
    if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_HEADER, 4, this->objectsHeaderBuffer)) return false;
    if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_HEADER, 4, this->enemiesHeaderBuffer)) return false;

    //The Co-op ROM handles level pointers slightly differently
    if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
        QByteArray objectBuffer(68, ' ');
        QByteArray enemyBuffer(68, ' ');
        if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_LOW_BYTE, 68, &objectBuffer)) return false;
        if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_LOW_BYTE, 68, &enemyBuffer)) return false;
        assert(objectBuffer.size() == enemyBuffer.size());
        for (int i = 0, j = 0; i < objectBuffer.size(); ++i) {
            if (i%2==0) { //low bytes are even
                this->lowObjectBuffer->data()[j] = objectBuffer.at(i);
                this->lowEnemyBuffer->data()[j] = enemyBuffer.at(i);
            } else { //high bytes are odd
                this->highObjectBuffer->data()[j] = objectBuffer.at(i);
                this->highEnemyBuffer->data()[j] = enemyBuffer.at(i);
                ++j;
            }
        }
    } else { //read the level pointers normally
        if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_LOW_BYTE, 34, this->lowObjectBuffer)) return false;
        if (!this->Read_Into_Buffer(this->OBJECT_ADDRESS_HIGH_BYTE, 34, this->highObjectBuffer)) return false;
        if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_LOW_BYTE, 34, this->lowEnemyBuffer)) return false;
        if (!this->Read_Into_Buffer(this->ENEMY_ADDRESS_HIGH_BYTE, 34, this->highEnemyBuffer)) return false;
    }
    return true;
}

bool Room_Address_Writer::Write_Room_Address_Tables() {
    //Write the Headers
    if (!this->Write_Buffer(this->OBJECT_ADDRESS_HEADER, this->objectsHeaderBuffer)) return false;
    if (!this->Write_Buffer(this->ENEMY_ADDRESS_HEADER, this->enemiesHeaderBuffer)) return false;

    //The Co-op ROM handles level pointers slightly differently
    if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
        QByteArray objectBuffer(68, ' ');
        QByteArray enemyBuffer(68, ' ');
        assert(objectBuffer.size() == enemyBuffer.size());
        for (int i = 0, j = 0; i < objectBuffer.size(); ++i) {
            if (i%2==0) { //low bytes are even
                objectBuffer.data()[i] = this->lowObjectBuffer->data()[j];
                enemyBuffer.data()[i] = this->lowEnemyBuffer->data()[j];
            } else { //high bytes are odd
                objectBuffer.data()[i] = this->highObjectBuffer->data()[j];
                enemyBuffer.data()[i] = this->highEnemyBuffer->data()[j];
                ++j;
            }
        }
        if (!this->Write_Buffer(this->OBJECT_ADDRESS_LOW_BYTE, &objectBuffer)) return false;
        if (!this->Write_Buffer(this->ENEMY_ADDRESS_LOW_BYTE, &enemyBuffer)) return false;
    } else { //write the level pointers normally
        if (!this->Write_Buffer(this->OBJECT_ADDRESS_LOW_BYTE, this->lowObjectBuffer)) return false;
        if (!this->Write_Buffer(this->OBJECT_ADDRESS_HIGH_BYTE, this->highObjectBuffer)) return false;
        if (!this->Write_Buffer(this->ENEMY_ADDRESS_LOW_BYTE, this->lowEnemyBuffer)) return false;
        if (!this->Write_Buffer(this->ENEMY_ADDRESS_HIGH_BYTE, this->highEnemyBuffer)) return false;
    }
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

bool Room_Address_Writer::Fix_Level_Addresses(qint64 fromOffset, qint64 toOffset, int numBytes) {
    if (!this->Fix_Level_Address_Buffer(fromOffset, toOffset, numBytes, this->lowObjectBuffer, this->highObjectBuffer, false)) return false;
    return this->Fix_Level_Address_Buffer(fromOffset, toOffset, numBytes, this->lowEnemyBuffer, this->highEnemyBuffer, true);
}

bool Room_Address_Writer::Read_Into_Buffer(int offset, int amount, QByteArray *buffer) {
    if (!buffer) return false;
    if (!this->file->seek(this->levelOffset->Fix_Offset(offset))) return false;

    //Read the data from the ROM
    qint64 ret = this->file->read(buffer->data(), amount);
    if (ret != amount || buffer == nullptr || buffer->size() != amount) return false;
    return true;
}

bool Room_Address_Writer::Write_Buffer(int offset, QByteArray *buffer) {
    assert(this->file);
    if (!buffer) return false;
    if (!this->file->seek(this->levelOffset->Fix_Offset(offset))) return false;
    return this->file->write(buffer->data(), buffer->length()) == buffer->length();
}

bool Room_Address_Writer::Fix_Level_Address_Buffer(qint64 fromOffset, qint64 toOffset, int numBytes, QByteArray *lowByteBuffer, QByteArray *highByteBuffer, bool enemies) {
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
        qint64 offset = static_cast<unsigned int>(static_cast<unsigned char>(highByteBuffer->data()[i]));
        offset *= 0x100; //move to the high byte
        offset += static_cast<unsigned int>(static_cast<unsigned char>(lowByteBuffer->data()[i]));

        //Convert from RAM value to ROM value
        qint64 fixedOffset = offset;
        if (enemies) {
            switch (this->levelOffset->Get_ROM_Type()) {
            case ROM_Type::INVALID:     assert(false); break;
            case ROM_Type::DEFAULT:     fixedOffset -= 0x7FF0; break;
            case ROM_Type::COOP_CGTI_1: fixedOffset += 0x10; break;
            }
        } else {
            switch (this->levelOffset->Get_ROM_Type()) {
            case ROM_Type::INVALID:     assert(false); break;
            case ROM_Type::DEFAULT:     fixedOffset -= 0x7FEE; break;
            case ROM_Type::COOP_CGTI_1: fixedOffset += 0x12; break;
            }
        }

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
            offset += static_cast<unsigned int>(numBytes);
        } else {
            assert(offset >= static_cast<unsigned int>(numBytes));
            offset -= static_cast<unsigned int>(numBytes);
        }

        //Update the offset in the buffer
        lowByteBuffer->data()[i] = static_cast<char>(static_cast<unsigned char>(offset&0x00FF));
        highByteBuffer->data()[i] = static_cast<char>(static_cast<unsigned char>((offset&0xFF00)/0x100));
    }

    return true;
}
