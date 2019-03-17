#ifndef SOUND_H
#define SOUND_H

#include "Byte_Writer.h"

class Sound : public Byte_Writer {
public:
    Sound(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    bool Randomize_Sounds();

private:
    QByteArray Get_Random_Bytes(int size);

    bool Big_Jump_Is_Small_Jump();
    bool Small_Jump_Is_Big_Jump();

    bool Brick_Break_1();
    bool Brick_Break_2();
    bool Brick_Break_3();
    bool Coin_Random();
    bool Jump_Random();
    bool Powerup_1();
    bool Powerup_2();
    bool Small_Jump_1();
    bool Stomp_1();
    bool Stomp_Random();
};

#endif // SOUND_H
