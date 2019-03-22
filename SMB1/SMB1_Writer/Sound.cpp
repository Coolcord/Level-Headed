#include "Sound.h"
#include "../../Common_Files/Random.h"
#include <assert.h>

bool Sound::Randomize_Sounds() {
    const int CHANCE = 3;
    bool success = false;

    //Set Jump Sounds
    if (!this->Jump_Random()) return false;
    if (Random::Get_Num(1)) {
        if (Random::Get_Num(1)) {
            if (!this->Big_Jump_Is_Small_Jump()) return false;
        } else {
            if (!this->Small_Jump_Is_Big_Jump()) return false;
        }
    }

    //Coin
    if (!this->Coin_Random()) return false;

    //Brick Break
    if (Random::Get_Num(CHANCE)) {
        switch(Random::Get_Num(2)+1) {
        default:    assert(false); break;
        case 1:     success = this->Brick_Break_1(); break;
        case 2:     success = this->Brick_Break_2(); break;
        case 3:     success = this->Brick_Break_3(); break;
        }
        if (!success) return false;
    }

    //Powerups
    switch(Random::Get_Num(2)+1) {
    default:    assert(false); break;
    case 1:     success = this->Powerup_1(); break;
    case 2:     success = this->Powerup_2(); break;
    case 3:     success = this->Powerup_Random_Key(); break;
    }
    if (!success) return false;

    //Stomp
    if (Random::Get_Num(CHANCE)==0) {
        if (!this->Stomp_1()) return false;
    } else {
        if (!this->Stomp_Random()) return false;
    }

    return true;
}

QByteArray Sound::Get_Random_Bytes(int size) {
    if (size <= 0) return QByteArray();
    QByteArray bytes(size, static_cast<char>(0x00));
    for (int i = 0; i < size; ++i) bytes[i] = static_cast<char>(Random::Get_Num(0xFF));
    return bytes;
}

bool Sound::Big_Jump_Is_Small_Jump() { return this->Write_Bytes_To_Offset(0x3522, QByteArray(1, static_cast<char>(0x80))); }
bool Sound::Small_Jump_Is_Big_Jump() { return this->Write_Bytes_To_Offset(0x3529, QByteArray(1, static_cast<char>(0x01))); }
bool Sound::Brick_Break_1() { return this->Write_Bytes_To_Offset(0x763F, QByteArray::fromHex(QString("0C0B0A090807060504030201").toLatin1())); }
bool Sound::Brick_Break_2() { return this->Write_Bytes_To_Offset(0x763F, QByteArray::fromHex(QString("6906CE0FAD09FD0D080D7A").toLatin1())); }
bool Sound::Brick_Break_3() { return this->Write_Bytes_To_Offset(0x763F, QByteArray::fromHex(QString("6A990CF50A090347080D0644").toLatin1())); }
bool Sound::Coin_Random() {
    if (!this->Write_Bytes_To_Offset(0x7529, QByteArray(1, static_cast<char>(Random::Get_Num(0x69)+0x04)))) return false;
    return this->Write_Bytes_To_Offset(0x7536, this->Get_Random_Bytes(1));
}
bool Sound::Jump_Random() {
    if (!this->Write_Bytes_To_Offset(0x73EB, QByteArray(1, static_cast<char>(Random::Get_Num(0x0C)+0x1F)))) return false;
    if (!this->Write_Bytes_To_Offset(0x73F3, QByteArray(1, static_cast<char>(Random::Get_Num(0x07)+0x24)))) return false;
    return this->Write_Bytes_To_Offset(0x73FD, QByteArray(1, static_cast<char>(Random::Get_Num(0x0C)+0x18)));
}
bool Sound::Powerup_1() { return this->Write_Bytes_To_Offset(0x74EA, QByteArray::fromHex(QString("122A426226405A0C223A4E58081E364C061C3444565E18304802122A4200").toLatin1())); }
bool Sound::Powerup_2() { return this->Write_Bytes_To_Offset(0x74EA, QByteArray::fromHex(QString("3052302C3E1A3E1A140C2E502E642032203210241E491E34102210061CF8").toLatin1())); }
bool Sound::Powerup_Random_Key() {
    QByteArray buffer = QByteArray::fromHex(QString("4C524C483E363E3630284A504A643C323C322C243A643A342C222C221C1414").toLatin1());
    int keyChanges = Random::Get_Num(4);
    for (int i = 0; i < keyChanges; ++i) {
        if (Random::Get_Num(1)) {
            for (int j = 0; j < buffer.size(); ++j) buffer[j] = this->Square_2_Key_Up(buffer[j]);
        } else {
            for (int j = 0; j < buffer.size(); ++j) buffer[j] = this->Square_2_Key_Down(buffer[j]);
        }
    }
    return this->Write_Bytes_To_Offset(0x74EA, buffer);
}
bool Sound::Stomp_1() { return this->Write_Bytes_To_Offset(0x73C1, QByteArray::fromHex(QString("0101010101010101010101010101").toLatin1())); }
bool Sound::Stomp_Random() {
    QByteArray buffer = QByteArray::fromHex(QString("9F9B989695949290909A97959392").toLatin1());
    int addAmount = Random::Get_Num(0xFF);
    for (int i = 0; i < buffer.size(); ++i) buffer[i] = static_cast<char>((static_cast<int>(buffer.at(i))+addAmount)%0x100);
    return this->Write_Bytes_To_Offset(0x73C1, buffer);
}

char Sound::Square_2_Key_Up(char byte) {
    switch (byte) {
    default: return byte; //ignore tempo and other unknown bytes
    case 0x42: return static_cast<char>(0x52); //B-4 -> Bb5 (there is no B-5)
    case 0x3E: return static_cast<char>(0x52); //Bb4 -> Bb5
    case 0x40: return static_cast<char>(0x50); //A-4 -> Ab5 (there is no A-5)
    case 0x3C: return static_cast<char>(0x50); //A-4 -> Ab5
    case 0x3A: return static_cast<char>(0x4E); //G-4 -> G-5
    case 0x38: return static_cast<char>(0x4C); //F#4 -> F-5 (there is no F#5)
    case 0x36: return static_cast<char>(0x4C); //F-4 -> F-5
    case 0x34: return static_cast<char>(0x44); //E-4 -> E-5
    case 0x32: return static_cast<char>(0x4A); //Eb4 -> Eb5
    case 0x30: return static_cast<char>(0x48); //D-4 -> D-5
    case 0x2E: return static_cast<char>(0x46); //C#4 -> C#5
    case 0x2C: return static_cast<char>(0x64); //C-4 -> C-5
    case 0x2A: return static_cast<char>(0x42); //B-3 -> B-4
    case 0x28: return static_cast<char>(0x3E); //Bb3 -> Bb4
    case 0x26: return static_cast<char>(0x40); //A-3 -> A-4
    case 0x24: return static_cast<char>(0x3C); //Ab3 -> Ab4
    case 0x22: return static_cast<char>(0x3A); //G-3 -> G-4
    case 0x20: return static_cast<char>(0x38); //F#3 -> F#4
    case 0x1E: return static_cast<char>(0x36); //F-3 -> F-4
    case 0x1C: return static_cast<char>(0x34); //E-3 -> E-4
    case 0x1A: return static_cast<char>(0x32); //Eb3 -> Eb4
    case 0x18: return static_cast<char>(0x30); //D-3 -> D-4
    case 0x16: return static_cast<char>(0x2E); //C#3 -> C#4
    case 0x14: return static_cast<char>(0x2C); //C-3 -> C-4
    case 0x12: return static_cast<char>(0x2A); //B-2 -> B-3
    case 0x10: return static_cast<char>(0x28); //Bb2 -> Bb3
    case 0x62: return static_cast<char>(0x26); //A-2 -> A-3
    case 0x0E: return static_cast<char>(0x24); //Ab2 -> Ab3
    case 0x5A: case 0x0C: return static_cast<char>(0x22); //G-2 -> G-3
    case 0x0A: return static_cast<char>(0x20); //F#2 -> F#3
    case 0x08: return static_cast<char>(0x1E); //F-2 -> F-3
    case 0x06: return static_cast<char>(0x1C); //E-2 -> E-3
    case 0x60: return static_cast<char>(0x1A); //Eb2 -> Eb3
    case 0x5E: return static_cast<char>(0x18); //D-2 -> D-3
    case 0x5C: return static_cast<char>(0x14); //C-2 -> C-3
    }
}

char Sound::Square_2_Key_Down(char byte) {
    switch (byte) {
    default: return byte; //ignore tempo and other unknown bytes
    case 0x58: return static_cast<char>(0x4E); //G-6 -> G-5
    case 0x56: return static_cast<char>(0x44); //E-6 -> E-5
    case 0x02: return static_cast<char>(0x48); //D-6 -> D-5
    case 0x54: return static_cast<char>(0x64); //C-6 -> C-5
    case 0x52: return static_cast<char>(0x3E); //Bb5 -> Bb4
    case 0x50: return static_cast<char>(0x3C); //Ab5 -> Ab4
    case 0x4E: return static_cast<char>(0x3A); //G-5 -> G-4
    case 0x4C: return static_cast<char>(0x36); //F-5 -> F-4
    case 0x44: return static_cast<char>(0x34); //E-5 -> E-4
    case 0x4A: return static_cast<char>(0x32); //Eb5 -> Eb4
    case 0x48: return static_cast<char>(0x30); //D-5 -> D-4
    case 0x46: return static_cast<char>(0x2E); //C#5 -> C#4
    case 0x64: return static_cast<char>(0x2C); //C-5 -> C-4
    case 0x42: return static_cast<char>(0x2A); //B-4 -> B-3
    case 0x3E: return static_cast<char>(0x28); //Bb4 -> Bb3
    case 0x40: return static_cast<char>(0x26); //A-4 -> A-3
    case 0x3C: return static_cast<char>(0x24); //Ab4 -> Ab3
    case 0x3A: return static_cast<char>(0x22); //G-4 -> G-3
    case 0x38: return static_cast<char>(0x20); //F#4 -> F#3
    case 0x36: return static_cast<char>(0x1E); //F-4 -> F-3
    case 0x34: return static_cast<char>(0x1C); //E-4 -> E-3
    case 0x32: return static_cast<char>(0x1A); //Eb4 -> Eb3
    case 0x30: return static_cast<char>(0x18); //D-4 -> D-3
    case 0x2E: return static_cast<char>(0x16); //C#4 -> C#3
    case 0x2C: return static_cast<char>(0x14); //C-4 -> C-3
    case 0x2A: return static_cast<char>(0x12); //B-3 -> B-2
    case 0x28: return static_cast<char>(0x10); //Bb3 -> Bb2
    case 0x26: return static_cast<char>(0x62); //A-3 -> A-2
    case 0x24: return static_cast<char>(0x0E); //Ab3 -> Ab2
    case 0x22: return static_cast<char>(0x0C); //G-3 -> G-2
    case 0x20: return static_cast<char>(0x0A); //F#3 -> F#2
    case 0x1E: return static_cast<char>(0x08); //F-3 -> F-2
    case 0x1C: return static_cast<char>(0x06); //E-3 -> E-2
    case 0x1A: return static_cast<char>(0x60); //Eb3 -> Eb2
    case 0x18: return static_cast<char>(0x5E); //D-3 -> D-2
    case 0x16: return static_cast<char>(0x5C); //C#3 -> C-2 (there is no C#2)
    case 0x14: return static_cast<char>(0x5C); //C-3 -> C-2
    }
}
