#include "Level_Pointers_Writer.h"
#include "Level_Offset.h"
#include "assert.h"

Level_Pointers_Writer::Level_Pointers_Writer(QFile *file, Level_Offset *levels)
{
    assert(file);
    assert(levels);
    this->file = file;
    this->levels = levels;

    //Allocate the buffers
    this->roomOrder = new QByteArray();
    this->objectHeader = new QByteArray();
    this->enemyHeader = new QByteArray();
    this->objectLowByte = new QByteArray();
    this->objectHighByte = new QByteArray();
    this->enemyLowByte = new QByteArray();
    this->enemyHighByte = new QByteArray();

    //Read into the buffers
    if (!this->file->seek(levels->Fix_Offset(this->ROOM_ORDER_TABLE)) ||
            this->file->read(this->roomOrder->data(), 36) != 36) throw "Unable to read from the room order table!";
    if (!this->file->seek(levels->Fix_Offset(this->ENEMY_POINTER_HEADER)) ||
            this->file->read(this->enemyHeader->data(), 4) != 4) throw "Unable to read from the enemy pointer header!";
    if (!this->file->seek(levels->Fix_Offset(this->ENEMY_POINTER_LOW_BYTE)) ||
            this->file->read(this->enemyLowByte->data(), 22) != 22) throw "Unable to read from the enemy low byte pointer table!";
    if (!this->file->seek(levels->Fix_Offset(this->ENEMY_POINTER_HIGH_BYTE)) ||
            this->file->read(this->enemyHighByte->data(), 22) != 22) throw "Unable to read from the enemy high byte pointer table!";
    if (!this->file->seek(levels->Fix_Offset(this->OBJECT_POINTER_HEADER)) ||
            this->file->read(this->objectHeader->data(), 4) != 4) throw "Unable to read from the object pointer header!";
    if (!this->file->seek(levels->Fix_Offset(this->OBJECT_POINTER_LOW_BYTE)) ||
            this->file->read(this->objectLowByte->data(), 22) != 22) throw "Unable to read from the object low byte pointer table!";
    if (!this->file->seek(levels->Fix_Offset(this->OBJECT_POINTER_HIGH_BYTE)) ||
            this->file->read(this->objectHighByte->data(), 22) != 22) throw "Unable to read from the object high byte pointer table!";
}

Level_Pointers_Writer::~Level_Pointers_Writer() {
    //Write and deallocate all buffers
    this->Write_And_Deallocate_Buffer(this->ROOM_ORDER_TABLE, this->roomOrder);
    this->Write_And_Deallocate_Buffer(this->ENEMY_POINTER_HEADER, this->enemyHeader);
    this->Write_And_Deallocate_Buffer(this->ENEMY_POINTER_LOW_BYTE, this->enemyLowByte);
    this->Write_And_Deallocate_Buffer(this->ENEMY_POINTER_HIGH_BYTE, this->enemyHighByte);
    this->Write_And_Deallocate_Buffer(this->OBJECT_POINTER_HEADER, this->objectHeader);
    this->Write_And_Deallocate_Buffer(this->OBJECT_POINTER_LOW_BYTE, this->objectLowByte);
    this->Write_And_Deallocate_Buffer(this->OBJECT_POINTER_HIGH_BYTE, this->objectHighByte);
}

bool Level_Pointers_Writer::Write_And_Deallocate_Buffer(const int offset, QByteArray *buffer) {
    if (buffer == NULL) return false;
    if (!this->file->seek(offset)) return false;
    if (this->file->write(buffer->data(), buffer->length()) != buffer->length()) return false;
    delete buffer;
    return true;
}
