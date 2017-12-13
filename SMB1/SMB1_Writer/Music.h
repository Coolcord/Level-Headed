#ifndef MUSIC_H
#define MUSIC_H

#include "Byte_Writer.h"

class Music : public Byte_Writer
{
public:
    Music(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Music() {}
    bool Enigmario();
    bool U1_7();
};

#endif // MUSIC_H