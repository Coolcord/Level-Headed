#ifndef MUSIC_H
#define MUSIC_H

#include "Byte_Writer.h"

class Music : public Byte_Writer
{
public:
    Music(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Music() {}
    bool Disable_Music();
    bool Alternative_Tone_Color_For_SQ1_And_SQ2_1();
    bool Alternative_Tone_Color_For_SQ1_And_SQ2_2();
    bool Enigmario();
    bool Flower_Mario();
    bool Star_Remix();
    bool Super_Mario_263_Shiki();
    bool Super_Mario_Evolution_Mikamashi();
    bool Super_Mushroom();
    bool U1_1();
    bool U1_2();
    bool U1_3();
    bool U1_4();
    bool U1_5();
    bool U1_6();
    bool U1_7();
    bool U1_8();
    bool U1_9();
    bool U1_10();
    bool Water_Mario();
};

#endif // MUSIC_H
