#include "Sound.h"
#include "../../Common_Files/Random.h"
#include <assert.h>

#include <QDebug>

bool Sound::Randomize_Sounds() {
    bool success = false;

    //Set Jump Sounds
    if (!this->Jump_Random()) return false;
    if (Random::Get_Instance().Get_Num(1)) {
        if (Random::Get_Instance().Get_Num(1)) {
            if (!this->Big_Jump_Is_Small_Jump()) return false;
        } else {
            if (!this->Small_Jump_Is_Big_Jump()) return false;
        }
    }

    //1-UP
    switch(Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); break;
    case 0:     success = this->One_Up_1(); break;
    case 1:     success = this->One_Up_2(); break;
    case 2:     success = this->One_Up_Random(); break;
    }
    if (!success) return false;

    //Coin
    if (!this->Coin_Random()) return false;

    //Bump
    if (!Bump_Random()) return false;

    //Brick Break
    switch(Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); break;
    case 0:     success = this->Brick_Break_1(); break;
    case 1:     success = this->Brick_Break_2(); break;
    case 2:     success = this->Brick_Break_Random(); break;
    }
    if (!success) return false;

    //Powerups
    switch(Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); break;
    case 0:     success = this->Powerup_1(); break;
    case 1:     success = this->Powerup_2(); break;
    case 2:     success = this->Powerup_3(); break;
    }
    if (!success) return false;

    //Stomp
    if (Random::Get_Instance().Get_Num(1)) {
        if (!this->Stomp_Random_1()) return false;
    } else {
        if (!this->Stomp_Random_2()) return false;
    }

    //Vine and Powerup Spawn
    if (!this->Vine_1()) return false;

    return true;
}

bool Sound::Big_Jump_Is_Small_Jump() { return this->Write_Bytes_To_Offset(0x3522, QByteArray(1, static_cast<char>(0x80))); }
bool Sound::Small_Jump_Is_Big_Jump() { return this->Write_Bytes_To_Offset(0x3529, QByteArray(1, static_cast<char>(0x01))); }

bool Sound::One_Up_1() { return this->Write_Bytes_To_Offset(0x74E4, this->Randomize_Notes(QByteArray::fromHex(QString("580254564E44").toLatin1()))); }
bool Sound::One_Up_2() { return this->Write_Bytes_To_Offset(0x74E4, this->Randomize_Notes(QByteArray::fromHex(QString("5240523A342C").toLatin1()))); }
bool Sound::One_Up_Random() {
    QByteArray bytes(6, ' ');
    for (int i = 0; i < bytes.size(); ++i) bytes[i] = this->Square_2_Get_Random_Note();
    return this->Write_Bytes_To_Offset(0x74E4, bytes);
}

bool Sound::Bump_Random() {
    //TODO: Check out offset 0x
    if (!this->Write_Bytes_To_Offset(0x7410, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(0x05, 0x1B))))) return false;
    return this->Write_Bytes_To_Offset(0x7412, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(0x91, 0x95))));
}
bool Sound::Brick_Break_1() { return this->Write_Bytes_To_Offset(0x763F, this->Possibly_Reverse_Notes(QByteArray::fromHex(QString("0B060C0F0A09030D080D060C").toLatin1()))); }
bool Sound::Brick_Break_2() { return this->Write_Bytes_To_Offset(0x763F, this->Possibly_Reverse_Notes(QByteArray::fromHex(QString("0C0B0A090807060504030201").toLatin1()))); }
bool Sound::Brick_Break_Random() { return this->Write_Bytes_To_Offset(0x763F, this->Get_Random_Bytes(12, 0x0F)); }
bool Sound::Coin_Random() {
    if (!this->Write_Bytes_To_Offset(0x7529, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(0x69)+0x04)))) return false;
    return this->Write_Bytes_To_Offset(0x7536, this->Get_Random_Bytes(1));
}
bool Sound::Jump_Random() {
    if (!this->Write_Bytes_To_Offset(0x73EB, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(0x0C)+0x1F)))) return false;
    if (!this->Write_Bytes_To_Offset(0x73F3, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(0x07)+0x24)))) return false;
    return this->Write_Bytes_To_Offset(0x73FD, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(0x0B)+0x18)));
}
bool Sound::Powerup_1() { return this->Write_Bytes_To_Offset(0x74EA, this->Randomize_Notes(QByteArray::fromHex(QString("4C524C483E363E3630284A504A643C323C322C243A643A342C222C221C1414").toLatin1()))); }
bool Sound::Powerup_2() { return this->Write_Bytes_To_Offset(0x74EA, this->Randomize_Notes(QByteArray::fromHex(QString("122A426226405A0C223A4E58081E364C061C3444565E18304802122A4200").toLatin1()), 2)); }
bool Sound::Powerup_3() { return this->Write_Bytes_To_Offset(0x74EA, this->Randomize_Notes(QByteArray::fromHex(QString("3052302C3E1A3E1A140C2E502E642032203210241E491E34102210061CF8").toLatin1()), 2)); }
bool Sound::Stomp_Random_1() {
    int highByte = Random::Get_Instance().Get_Num(0xF);
    int lowByte = Random::Get_Instance().Get_Num(0xB); //values that end with 0x0C or higher are loud
    if ((highByte&1) == 1 && lowByte < 5) lowByte = 5;; //some low bytes are quiet
    return this->Write_Bytes_To_Offset(0x73C1, QByteArray(14, static_cast<char>((highByte*0x10)+lowByte)));
}
bool Sound::Stomp_Random_2() { return this->Write_Bytes_To_Offset(0x73C1, this->Random_Increment(QByteArray::fromHex(QString("9F9B989695949290909A97959392").toLatin1()))); }
bool Sound::Vine_1() { return this->Write_Bytes_To_Offset(0x7508, this->Randomize_Notes(QByteArray::fromHex(QString("4C524C483E363E3630284A504A643C323C322C243A643A342C222C221C1414").toLatin1()), 3)); }

QByteArray Sound::Get_Random_Bytes(int size, int maxValue) {
    if (size <= 0) return QByteArray();
    QByteArray bytes(size, static_cast<char>(0x00));
    for (int i = 0; i < size; ++i) bytes[i] = static_cast<char>(Random::Get_Instance().Get_Num(maxValue));
    return bytes;
}

QByteArray Sound::Random_Increment(QByteArray bytes, int maxAmount) {
    bytes = this->Possibly_Reverse_Notes(bytes);
    int addAmount = Random::Get_Instance().Get_Num(maxAmount);
    for (int i = 0; i < bytes.size(); ++i) bytes[i] = static_cast<char>((static_cast<int>(bytes.at(i))+addAmount)%0x100);
    return bytes;
}

QByteArray Sound::Randomize_Notes(QByteArray bytes, int maxKeyChanges) {
    int keyChanges = Random::Get_Instance().Get_Num(maxKeyChanges);
    bool keyup = Random::Get_Instance().Get_Num(1);
    bytes = this->Possibly_Reverse_Notes(bytes);
    for (int i = 0; i < keyChanges; ++i) {
        if (keyup) {
            for (int j = 0; j < bytes.size(); ++j) bytes[j] = this->Square_2_Key_Up(bytes[j]);
        } else {
            for (int j = 0; j < bytes.size(); ++j) bytes[j] = this->Square_2_Key_Down(bytes[j]);
        }
    }
    return bytes;
}

QByteArray Sound::Reverse_Notes(const QByteArray &bytes) {
    QByteArray reversed(bytes.size(), ' ');
    for (int i = 0, j = bytes.size()-1; i < bytes.size(); ++i, --j) reversed[j] = bytes.at(i);
    return reversed;
}

QByteArray Sound::Possibly_Reverse_Notes(const QByteArray &bytes) {
    if (Random::Get_Instance().Get_Num(1)) return this->Reverse_Notes(bytes);
    else return bytes;
}

char Sound::Square_2_Get_Random_Note() {
    switch (Random::Get_Instance().Get_Num(39)) {
    default: assert(false); return static_cast<char>(0x00);
    case 0:  return static_cast<char>(0x60);//Eb2
    case 1:  return static_cast<char>(0x5E);//D-2
    case 2:  return static_cast<char>(0x5C);//C-2
    case 3:  return static_cast<char>(0x58);//G-6
    case 4:  return static_cast<char>(0x56);//E-6
    case 5:  return static_cast<char>(0x02);//D-6
    case 6:  return static_cast<char>(0x54);//C-6
    case 7:  return static_cast<char>(0x52);//Bb5
    case 8:  return static_cast<char>(0x50);//Ab5
    case 9:  return static_cast<char>(0x4E);//G-5
    case 10: return static_cast<char>(0x4C);//F-5
    case 11: return static_cast<char>(0x44);//E-5
    case 12: return static_cast<char>(0x4A);//Eb5
    case 13: return static_cast<char>(0x48);//D-5
    case 14: return static_cast<char>(0x46);//C#5
    case 15: return static_cast<char>(0x64);//C-5
    case 16: return static_cast<char>(0x42);//B-4
    case 17: return static_cast<char>(0x3E);//Bb4
    case 18: return static_cast<char>(0x40);//A-4
    case 19: return static_cast<char>(0x3C);//Ab4
    case 20: return static_cast<char>(0x3A);//G-4
    case 21: return static_cast<char>(0x38);//F#4
    case 22: return static_cast<char>(0x36);//F-4
    case 23: return static_cast<char>(0x34);//E-4
    case 24: return static_cast<char>(0x32);//Eb4
    case 25: return static_cast<char>(0x30);//D-4
    case 26: return static_cast<char>(0x2E);//C#4
    case 27: return static_cast<char>(0x2C);//C-4
    case 28: return static_cast<char>(0x2A);//B-3
    case 29: return static_cast<char>(0x28);//Bb3
    case 30: return static_cast<char>(0x26);//A-3
    case 31: return static_cast<char>(0x24);//Ab3
    case 32: return static_cast<char>(0x22);//G-3
    case 33: return static_cast<char>(0x20);//F#3
    case 34: return static_cast<char>(0x1E);//F-3
    case 35: return static_cast<char>(0x1C);//E-3
    case 36: return static_cast<char>(0x1A);//Eb3
    case 37: return static_cast<char>(0x18);//D-3
    case 38: return static_cast<char>(0x16);//C#3
    case 39: return static_cast<char>(0x14);//C-3
    }
}

char Sound::Square_2_Key_Random(char byte) {
    int keyChanges = Random::Get_Instance().Get_Num(4);
    bool keyup = Random::Get_Instance().Get_Num(1);
    for (int i = 0; i < keyChanges; ++i) {
        if (keyup) byte = this->Square_2_Key_Up(byte);
        else byte = this->Square_2_Key_Down(byte);
    }
    return byte;
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
