#include "SMB1_Writer.h"
#include <assert.h>

SMB1_Writer::SMB1_Writer(QString romLocation) {
    //Open the ROM file
    this->file = new QFile(romLocation);
    assert(file);
    if (!this->file->open(QFile::ReadWrite)) {
        throw "Unable to get read/write permissions";
    }

    //Set class variables
    this->headerBuffer = NULL;
    this->objectsBuffer = NULL;
    this->enemiesBuffer = NULL;
    this->objectOffset = BAD_OFFSET;
    this->enemyOffset = BAD_OFFSET;
}

SMB1_Writer::~SMB1_Writer() {
    if (this->Are_Buffers_Allocated()) {
        if (!this->Write_Level()) { //try to flush the buffers
            this->Deallocate_Buffers(); //deallocate memory manually
        }
    }
    assert(!this->Are_Buffers_Allocated()); //make sure memory leaks never happen
    this->file->close();
    delete this->file;
}

bool SMB1_Writer::New_Level(const int objectOffset, const int enemyOffset) {
    //Make sure that the buffers are empty
    if (this->Are_Buffers_Allocated()) return false;

    //Allocate Memory
    this->objectOffset = objectOffset;
    this->enemyOffset = enemyOffset;
    this->headerBuffer = new QByteArray();
    this->objectsBuffer = new QByteArray();
    this->enemiesBuffer = new QByteArray();
    return true;
}

bool SMB1_Writer::Write_Level() {
    //Make sure the offsets have been set
    if (this->objectOffset == BAD_OFFSET) return false;
    if (this->enemyOffset == BAD_OFFSET) return false;

    //Write Header
    if (!this->Write_Buffer(this->objectOffset-2, this->headerBuffer)) return false;

    //Write Objects
    if (!this->Write_Buffer(this->objectOffset, this->objectsBuffer)) return false;

    //Write Enemies
    if (!this->Write_Buffer(this->enemyOffset, this->enemiesBuffer)) return false;

    //Deallocate memory
    this->Deallocate_Buffers();
    return true;
}

bool SMB1_Writer::Write_Buffer(const int offset, QByteArray *buffer) {
    if (buffer == NULL) return false;
    if (!this->file->seek(offset)) return false;
    return this->file->write(buffer->data(), buffer->length()) == buffer->length();
}

bool SMB1_Writer::Read_Level_Header() {
    if (!this->Are_Buffers_Allocated()) return false;
    if (!this->file->seek(objectOffset-2)) return false;
    if (this->file->read(this->headerBuffer->data(), 2) != 2) return false;
    assert(this->headerBuffer->size() == 2);
    return true;
}

bool SMB1_Writer::Read_Objects() {
    if (!this->Are_Buffers_Allocated()) return false;
    if (!this->file->seek(this->objectOffset)) return false;

    QByteArray buffer;
    qint64 ret = this->file->read(buffer.data(), 2);
    while (ret == 2 && buffer.data() != NULL
           && static_cast<unsigned char>(buffer.data()[0]) != 0xFD) {
        this->objectsBuffer->append(buffer);
        ret = this->file->read(buffer.data(), 2);
    }

    return true;
}

bool SMB1_Writer::Read_Enemies() {
    if (!this->Are_Buffers_Allocated()) return false;
    if (this->enemyOffset == 0) return true; //nothing to read
    if (!this->file->seek(this->enemyOffset)) return false;

    QByteArray buffer;
    qint64 ret = this->file->read(buffer.data(), 2);
    while (ret == 2 && buffer.data() != NULL
           && static_cast<unsigned char>(buffer.data()[0]) != 0xFF) {
        this->enemiesBuffer->append(buffer);
        //TODO: Handle Pipe pointers here by checking if the second nibble of the first byte is 0xE
        ret = this->file->read(buffer.data(), 2);
    }

    return true;
}

bool SMB1_Writer::Are_Buffers_Allocated() {
    return (this->headerBuffer != NULL && this->objectsBuffer != NULL && this->enemiesBuffer != NULL
            && this->objectOffset != BAD_OFFSET && this->enemyOffset != BAD_OFFSET);
}

void SMB1_Writer::Deallocate_Buffers() {
    delete this->headerBuffer;
    delete this->objectsBuffer;
    delete this->enemiesBuffer;
    this->headerBuffer = NULL;
    this->objectsBuffer = NULL;
    this->enemiesBuffer = NULL;
    this->objectOffset = BAD_OFFSET;
    this->enemyOffset = BAD_OFFSET;
}
