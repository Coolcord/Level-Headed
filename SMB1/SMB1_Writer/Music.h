#ifndef MUSIC_H
#define MUSIC_H

#include "Byte_Writer.h"

class Music : public Byte_Writer
{
public:
    Music(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Music() {}
    bool Disable_Music();
    bool Tone_Color_1();
    bool Tone_Color_2();
    bool Tone_Color_3();
    bool Tone_Color_4();
    bool Tone_Color_5();
    bool Tone_Color_6();
    bool Tone_Color_7();
    bool Tone_Color_8();
    bool Tone_Color_9();
    bool Tone_Color_10();
};

#endif // MUSIC_H
