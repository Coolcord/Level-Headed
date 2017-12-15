#ifndef MUSIC_H
#define MUSIC_H

#include "Byte_Writer.h"

class Music : public Byte_Writer
{
public:
    Music(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Music() {}
    bool Enigmario();
    bool U1_1();
    bool U1_2();
    bool U1_3();
    bool U1_6();
    bool U1_7();
    bool U1_9();
    bool U1_10();
};

#endif // MUSIC_H
