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

bool Byte_Writer::Write_Bytes_To_Offset(qint64 offset, const QByteArray &bytes) {
    offset = this->levelOffset->Fix_Offset(offset);
    if (!this->file->seek(offset)) return false;
    return (this->file->write(bytes) == bytes.size());
}
