#ifndef MUSIC_H
#define MUSIC_H

class Sequential_Archive_Handler;

#include "Byte_Writer.h"

class Music : public Byte_Writer {
public:
    Music(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler);
    ~Music() {}
    bool Get_Last_Tone_Color();
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
    bool Tone_Color_11();
    bool Tone_Color_12();
    bool Tone_Color_13();
    bool Tone_Color_14();
    bool Tone_Color_15();
    bool Tone_Color_16();
    bool Tone_Color_17();
    bool Tone_Color_18();
    bool Tone_Color_19();
    bool Tone_Color_20();
    bool Tone_Color_21();

private:
    Sequential_Archive_Handler *sequentialArchiveHandler;
    int lastToneColor;
};

#endif // MUSIC_H
