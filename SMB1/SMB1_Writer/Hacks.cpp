#include "Hacks.h"
#include "Level_Offset.h"
#include <assert.h>

Hacks::Hacks(QFile *file, Level_Offset *levelOffset) {
    assert(file);
    assert(levelOffset);
    this->file = file;
    this->levelOffset = levelOffset;
}

Hacks::~Hacks() {
    this->file = NULL;
    this->levelOffset = NULL;
}

bool Hacks::Write_Watermark() {
    QByteArray watermarkBytes;
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x15));
    watermarkBytes.append(static_cast<char>(0x0E));
    watermarkBytes.append(static_cast<char>(0x1F));
    watermarkBytes.append(static_cast<char>(0x0E));
    watermarkBytes.append(static_cast<char>(0x15));
    watermarkBytes.append(static_cast<char>(0x28));
    watermarkBytes.append(static_cast<char>(0x11));
    watermarkBytes.append(static_cast<char>(0x0E));
    watermarkBytes.append(static_cast<char>(0x0A));
    watermarkBytes.append(static_cast<char>(0x0D));
    watermarkBytes.append(static_cast<char>(0x0E));
    watermarkBytes.append(static_cast<char>(0x0D));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x00));
    watermarkBytes.append(static_cast<char>(0x25));
    watermarkBytes.append(static_cast<char>(0xE3));
    watermarkBytes.append(static_cast<char>(0x1B));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x1F));
    watermarkBytes.append(static_cast<char>(0x12));
    watermarkBytes.append(static_cast<char>(0x1C));
    watermarkBytes.append(static_cast<char>(0x12));
    watermarkBytes.append(static_cast<char>(0x1D));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x0C));
    watermarkBytes.append(static_cast<char>(0x18));
    watermarkBytes.append(static_cast<char>(0x18));
    watermarkBytes.append(static_cast<char>(0x15));
    watermarkBytes.append(static_cast<char>(0x0C));
    watermarkBytes.append(static_cast<char>(0x18));
    watermarkBytes.append(static_cast<char>(0x1B));
    watermarkBytes.append(static_cast<char>(0x0D));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x18));
    watermarkBytes.append(static_cast<char>(0x17));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x10));
    watermarkBytes.append(static_cast<char>(0x12));
    watermarkBytes.append(static_cast<char>(0x1D));
    watermarkBytes.append(static_cast<char>(0x11));
    watermarkBytes.append(static_cast<char>(0x1E));
    watermarkBytes.append(static_cast<char>(0x0B));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x00));
    watermarkBytes.append(static_cast<char>(0x26));
    watermarkBytes.append(static_cast<char>(0x4A));
    watermarkBytes.append(static_cast<char>(0x0D));
    watermarkBytes.append(static_cast<char>(0x0F));
    watermarkBytes.append(static_cast<char>(0x18));
    watermarkBytes.append(static_cast<char>(0x1B));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x1E));
    watermarkBytes.append(static_cast<char>(0x19));
    watermarkBytes.append(static_cast<char>(0x0D));
    watermarkBytes.append(static_cast<char>(0x0A));
    watermarkBytes.append(static_cast<char>(0x1D));
    watermarkBytes.append(static_cast<char>(0x0E));
    watermarkBytes.append(static_cast<char>(0x1C));
    watermarkBytes.append(static_cast<char>(0x2B));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x00));
    watermarkBytes.append(static_cast<char>(0x26));
    watermarkBytes.append(static_cast<char>(0x88));
    watermarkBytes.append(static_cast<char>(0x11));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x28));
    watermarkBytes.append(static_cast<char>(0x0C));
    watermarkBytes.append(static_cast<char>(0x18));
    watermarkBytes.append(static_cast<char>(0x18));
    watermarkBytes.append(static_cast<char>(0x15));
    watermarkBytes.append(static_cast<char>(0x0C));
    watermarkBytes.append(static_cast<char>(0x18));
    watermarkBytes.append(static_cast<char>(0x1B));
    watermarkBytes.append(static_cast<char>(0x0D));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    return this->Write_Bytes_To_Offset(0x0DBB, watermarkBytes);
}

bool Hacks::Invincibility() {
    if (!this->Write_Bytes_To_Offset(0x399D, QByteArray(1, 0xFF))) return false;
    return this->Write_Bytes_To_Offset(0x589B, QByteArray(1, 0xFF));
}

bool Hacks::Moon_Jump() {
    //if (!this->Write_Bytes_To_Offset(0x2F78, QByteArray(1, 0x6A))) return false; //turbo button presses
    return this->Write_Bytes_To_Offset(0x3497, QByteArray(1, 0x13)); //jump while in midair
}

bool Hacks::Star_Color_Mario() {
    return this->Write_Bytes_To_Offset(0x2F53, QByteArray(1, 0xE7));
}

bool Hacks::Fix_Lakitu_Throw_Arc() {
    QByteArray lakituThrowArcBytes;
    lakituThrowArcBytes.append(static_cast<char>(0xA0));
    lakituThrowArcBytes.append(static_cast<char>(0x02));
    lakituThrowArcBytes.append(static_cast<char>(0x95));
    lakituThrowArcBytes.append(static_cast<char>(0x58));
    lakituThrowArcBytes.append(static_cast<char>(0xC9));
    lakituThrowArcBytes.append(static_cast<char>(0x00));
    lakituThrowArcBytes.append(static_cast<char>(0x30));
    lakituThrowArcBytes.append(static_cast<char>(0x01));
    lakituThrowArcBytes.append(static_cast<char>(0x88));
    lakituThrowArcBytes.append(static_cast<char>(0x20));
    lakituThrowArcBytes.append(static_cast<char>(0x46));
    lakituThrowArcBytes.append(static_cast<char>(0xC3));
    return this->Write_Bytes_To_Offset(0x4444, lakituThrowArcBytes);
}

bool Hacks::Fast_Enemies(int speed) {
    if (speed < 0 || speed > 5) return false;
    switch (speed) {
    default:    assert(false);
    case 1:
    case 2:     break;
    case 3:     speed = 0x8; break;
    case 4:     speed = 0xD; break;
    case 5:     speed = 0xB; break;
    }
    return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(speed)));
}

bool Hacks::Write_Bytes_To_Offset(int offset, const QByteArray &bytes) {
    offset = this->levelOffset->Fix_Offset(offset);
    if (!this->file->seek(offset)) return false;
    return (this->file->write(bytes) == bytes.size());
}
