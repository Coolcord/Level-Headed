#include "Music.h"
#include "Sequential_Archive_Handler.h"
#include <assert.h>

Music::Music(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler) : Byte_Writer(file, levelOffset) {
    assert(sequentialArchiveHandler);
    this->sequentialArchiveHandler = sequentialArchiveHandler;
}

bool Music::Disable_Music() {
    if (!this->Write_Bytes_To_Offset(0x1124, QByteArray(1, 0x49))) return false; //disables most music
    if (!this->Write_Bytes_To_Offset(0x582E, QByteArray(1, 0x82))) return false; //disables Starman music
    if (!this->Write_Bytes_To_Offset(0x7920, QByteArray(1, 0x4B))) return false; //disables castle complete theme
    if (!this->Write_Bytes_To_Offset(0x7922, QByteArray(1, 0x4B))) return false; //disables level complete theme
    return true;
}

bool Music::Tone_Color_1() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(1)) return true;
    if (!this->Write_Bytes_To_Offset(0x77C2, QByteArray(1, 0xFE))) return false;
    if (!this->Write_Bytes_To_Offset(0x77FF, QByteArray::fromHex(QString("E7F88DB7072081F3A5F1D01FADB1072991D00EACB707F003CEB707B9A2FF").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x78F2, QByteArray::fromHex(QString("06A908D002A928A282A07F60EAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAADB1072908F004B996FF60B99A").toLatin1()));
}

bool Music::Tone_Color_2() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(2)) return true;
    if (!this->Write_Bytes_To_Offset(0x77C2, QByteArray(1, 0xFE))) return false;
    if (!this->Write_Bytes_To_Offset(0x77FF, QByteArray::fromHex(QString("E7F88DB7072081F3A5F1D01FADB1072991D00EACB707F003CEB707B99AFF").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x78F2, QByteArray::fromHex(QString("06A928D002A908A282A07F60EAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAADB1072908F004B996").toLatin1()));
}

bool Music::Tone_Color_3() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(3)) return true;
    return this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("F8F9FAFBF0F4F4F5F5F6F7F8F0F1F2F2F3F3F3F4F4F4F4F4F4F5F5F5F5F5F5F6F6F6F6F6F6F6F6F6"
            "F6F6F6F6F6F6F6F6F5F5F4F3").toLatin1()));
}

bool Music::Tone_Color_4() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(4)) return true;
    return this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("08090A0B000404050506000800010202030303040404040404050505050505060606060606060606"
            "060606060606060605050403").toLatin1()));
}

bool Music::Tone_Color_5() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(5)) return true;
    return this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("11945060606097971212").toLatin1()));
}

bool Music::Tone_Color_6() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(6)) return true;
    return this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("18191A1B101414151516171810111212131313141414141414151515151515161616161616161616"
            "161616161616161615151413").toLatin1()));
}

bool Music::Tone_Color_7() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(7)) return true;
    return this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("48494A4B404444454546474840414242434343444444444444454545454545464646464646464646"
            "464646464646464645454443").toLatin1()));
}

bool Music::Tone_Color_8() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(8)) return true;
    return this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("88898A8B808484858586878880818282838383848484848484858585858585868686868686868686"
            "868686868686868685858483").toLatin1()));
}

bool Music::Tone_Color_9() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(9)) return true;
    return this->Write_Bytes_To_Offset(0x77B7, QByteArray(1, 0xD8));
}

bool Music::Tone_Color_10() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(10)) return true;
    return this->Write_Bytes_To_Offset(0x77B7, QByteArray(1, 0x10));
}

bool Music::Tone_Color_11() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(11)) return true;
    if (!this->Write_Bytes_To_Offset(0x7F17, QByteArray(1, 0xCF))) return false;
    if (!this->Write_Bytes_To_Offset(0x7F2D, QByteArray(1, 0x67))) return false;
    if (!this->Write_Bytes_To_Offset(0x7F45, QByteArray(1, 0xB3))) return false;
    return this->Write_Bytes_To_Offset(0x7F72, QByteArray::fromHex(QString("021A").toLatin1()));
}
