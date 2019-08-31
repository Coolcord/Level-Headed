#include "Byte_Writer.h"
#include "Level_Offset.h"
#include <assert.h>

Byte_Writer::Byte_Writer(QFile *file, Level_Offset *levelOffset) {
    assert(file);
    assert(levelOffset);
    this->file = file;
    this->levelOffset = levelOffset;
}

Byte_Writer::~Byte_Writer() {
    this->file = nullptr;
    this->levelOffset = nullptr;
}

bool Byte_Writer::Increment_Byte_At_Offset(qint64 offset, int incrementAmount) {
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(offset, 1, bytes)) return false;
    int value = static_cast<int>(bytes.data()[0]);
    value += incrementAmount;
    if (value < 0x00 || value > 0xFF) return false;
    bytes.data()[0] = static_cast<char>(value);
    return this->Write_Bytes_To_Offset(offset, bytes);
}

bool Byte_Writer::Read_Bytes_From_Offset(qint64 offset, int amount, QByteArray &bytes) {
    bytes = QByteArray();
    offset = this->levelOffset->Fix_Offset(offset);
    if (!this->file->seek(offset)) return false;
    bytes = this->file->read(amount);
    return bytes.size() == amount;
}

bool Byte_Writer::Write_Bytes_To_Offset(qint64 offset, const QByteArray &bytes) {
    offset = this->levelOffset->Fix_Offset(offset);
    if (!this->file->seek(offset)) return false;
    return (this->file->write(bytes) == bytes.size());
}
