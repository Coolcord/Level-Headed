#include "Sound.h"
#include "../../Common_Files/Random.h"
#include <assert.h>

bool Sound::Randomize_Sounds() {
    const int CHANCE = 3;
    bool success = false;

    //Set Jump Sounds
    if (Random::Get_Num(1)) {
        if (Random::Get_Num(1)) {
            if (!this->Big_Jump_Is_Small_Jump()) return false;
        } else {
            if (!this->Small_Jump_Is_Big_Jump()) return false;
        }
    }

    //Coin
    if (Random::Get_Num(CHANCE)==0) {
        switch(Random::Get_Num(1)+1) {
        default:    assert(false); break;
        case 1:     success = this->Coin_1(); break;
        case 2:     success = this->Coin_2(); break;
        }
        if (!success) return false;
    } else {
        if (!this->Coin_Random()) return false;
    }

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
    if (Random::Get_Num(CHANCE)) {
        switch(Random::Get_Num(1)+1) {
        default:    assert(false); break;
        case 1:     success = this->Powerup_1(); break;
        case 2:     success = this->Powerup_2(); break;
        }
        if (!success) return false;
    }

    //Small Jump
    if (Random::Get_Num(CHANCE)==0 && !this->Small_Jump_1()) return false;

    //Stomp
    if (Random::Get_Num(CHANCE)) {
        if (Random::Get_Num(CHANCE)==0) {
            if (!this->Stomp_1()) return false;
        } else {
            if (!this->Stomp_Random()) return false;
        }
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
bool Sound::Coin_1() {
    if (!this->Write_Bytes_To_Offset(0x7529, QByteArray(1, static_cast<char>(0x04)))) return false;
    return this->Write_Bytes_To_Offset(0x7540, QByteArray(1, static_cast<char>(0xFA)));
}
bool Sound::Coin_2() { return this->Write_Bytes_To_Offset(0x7540, QByteArray(1, static_cast<char>(0xFA))); }
bool Sound::Coin_Random() { return this->Write_Bytes_To_Offset(0x7536, this->Get_Random_Bytes(1)); }
bool Sound::Powerup_1() { return this->Write_Bytes_To_Offset(0x74EA, QByteArray::fromHex(QString("122A426226405A0C223A4E58081E364C061C3444565E18304802122A4200").toLatin1())); }
bool Sound::Powerup_2() { return this->Write_Bytes_To_Offset(0x74EA, QByteArray::fromHex(QString("3052302C3E1A3E1A140C2E502E642032203210241E491E34102210061CF8F8").toLatin1())); }
bool Sound::Small_Jump_1() { return this->Write_Bytes_To_Offset(0x7453, QByteArray::fromHex(QString("EAEA").toLatin1())); }
bool Sound::Stomp_1() { return this->Write_Bytes_To_Offset(0x73C1, QByteArray::fromHex(QString("0101010101010101010101010101").toLatin1())); }
bool Sound::Stomp_Random() { return this->Write_Bytes_To_Offset(0x73C1, this->Get_Random_Bytes(14)); }
