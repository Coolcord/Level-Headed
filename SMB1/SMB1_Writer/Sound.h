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

    bool One_Up_1();
    bool One_Up_2();
    bool One_Up_Random();
    bool Brick_Break_1();
    bool Brick_Break_2();
    bool Brick_Break_3();
    bool Coin_Random();
    bool Jump_Random();
    bool Powerup_1();
    bool Powerup_2();
    bool Powerup_3();
    bool Small_Jump_1();
    bool Stomp_Random_1();
    bool Stomp_Random_2();
    bool Vine_1();

    QByteArray Randomize_Notes(QByteArray bytes, int maxKeyChanges = 4);
    QByteArray Reverse_Notes(const QByteArray &bytes);
    char Square_2_Get_Random_Note();
    char Square_2_Key_Random(char byte);
    char Square_2_Key_Up(char byte);
    char Square_2_Key_Down(char byte);
};

#endif // SOUND_H
