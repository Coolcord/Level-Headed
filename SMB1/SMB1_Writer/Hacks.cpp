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
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    watermarkBytes.append(static_cast<char>(0x24));
    return this->Write_Bytes_To_Offset(0x0DBB, watermarkBytes);
}

bool Hacks::Infinite_Lives() {
    return this->Write_Bytes_To_Offset(0x11E9, QByteArray(1, 0xAD));
}

bool Hacks::Set_Starting_Lives(int lives) {
    if (lives <= 0 && lives > 0x80) return false;
    return this->Write_Bytes_To_Offset(0x107A, QByteArray(1, static_cast<char>(lives-1)));
}

bool Hacks::Play_As_Luigi() {
    //Play as Luigi
    if (!this->Write_Bytes_To_Offset(0x609, QByteArray(1, 0xA9))) return false;
    if (!this->Write_Bytes_To_Offset(0x870, QByteArray(1, 0x72))) return false;
    if (!this->Write_Bytes_To_Offset(0x42C, QByteArray(1, 0x5A))) return false;
    if (!this->Write_Bytes_To_Offset(0x42E, QByteArray(1, 0x70))) return false;

    QByteArray luigiText;
    luigiText.append(static_cast<char>(0x15));
    luigiText.append(static_cast<char>(0x1E));
    luigiText.append(static_cast<char>(0x12));
    luigiText.append(static_cast<char>(0x10));
    luigiText.append(static_cast<char>(0x12));
    return this->Write_Bytes_To_Offset(0x765, luigiText); //change name above score
}

bool Hacks::Invincibility() {
    if (!this->Write_Bytes_To_Offset(0x399D, QByteArray(1, 0xFF))) return false;
    if (!this->Write_Bytes_To_Offset(0x589B, QByteArray(1, 0xFF))) return false;
    if (!this->Write_Bytes_To_Offset(0x119E, QByteArray(1, 0xC8))) return false; //gives Mario Star Power, but it wears off after a while
    return this->Write_Bytes_To_Offset(0x5945, QByteArray(1, 0x03)); //if Mario gets damaged, revert to Super Mario
}

bool Hacks::Moon_Jump() {
    //if (!this->Write_Bytes_To_Offset(0x2F78, QByteArray(1, 0x6A))) return false; //turbo button presses
    return this->Write_Bytes_To_Offset(0x3497, QByteArray(1, 0x13)); //jump while in midair
}

bool Hacks::Star_Color_Mario() {
    return this->Write_Bytes_To_Offset(0x2F53, QByteArray(1, 0xE7));
}

bool Hacks::Fireballs_Kill_Everything_Onscreen() {
    return this->Write_Bytes_To_Offset(0x572D, QByteArray(1, 0x8F));
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
