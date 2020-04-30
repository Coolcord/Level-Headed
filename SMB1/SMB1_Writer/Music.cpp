#include "Music.h"
#include "Sequential_Archive_Handler.h"
#include <assert.h>

Music::Music(QFile *f, Level_Offset *lo, Sequential_Archive_Handler *sequentialArchiveHandler) : Byte_Writer(f, lo) {
    assert(sequentialArchiveHandler);
    this->sequentialArchiveHandler = sequentialArchiveHandler;
    this->lastToneColor = 0;
}

bool Music::Get_Last_Tone_Color() {
    return this->lastToneColor;
}

bool Music::Disable_Music() {
    return this->Write_Bytes_To_Offset(0x7364, QByteArray::fromHex(QString("EAEAEA").toLatin1()));
}

bool Music::Tone_Color_1() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(1)) return true;
    if (!this->Write_Bytes_To_Offset(0x77C2, QByteArray(1, static_cast<char>(0xFE)))) return false;
    if (!this->Write_Bytes_To_Offset(0x77FF, QByteArray::fromHex(QString("E7F88DB7072081F3A5F1D01FADB1072991D00EACB707F003CEB707B9A2FF").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x78F2, QByteArray::fromHex(QString("06A908D002A928A282A07F60EAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAADB1072908F004B996FF60B99A").toLatin1()))) return false;
    this->lastToneColor = 1;
    return true;
}

bool Music::Tone_Color_2() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(2)) return true;
    if (!this->Write_Bytes_To_Offset(0x77C2, QByteArray(1, static_cast<char>(0xFE)))) return false;
    if (!this->Write_Bytes_To_Offset(0x77FF, QByteArray::fromHex(QString("E7F88DB7072081F3A5F1D01FADB1072991D00EACB707F003CEB707B99AFF").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x78F2, QByteArray::fromHex(QString("06A928D002A908A282A07F60EAEAEAEAEAEAEAEAEAEAEAEAEAEAEAEAADB1072908F004B996").toLatin1()))) return false;
    this->lastToneColor = 2;
    return true;
}

bool Music::Tone_Color_3() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(3)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("F8F9FAFBF0F4F4F5F5F6F7F8F0F1F2F2F3F3F3F4F4F4F4F4F4F5F5F5F5F5F5F6F6F6F6F6F6F6F6F6"
            "F6F6F6F6F6F6F6F6F5F5F4F3").toLatin1()))) return false;
    this->lastToneColor = 3;
    return true;
}

bool Music::Tone_Color_4() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(4)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("08090A0B000404050506000800010202030303040404040404050505050505060606060606060606"
            "060606060606060605050403").toLatin1()))) return false;
    this->lastToneColor = 4;
    return true;
}

bool Music::Tone_Color_5() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(5)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("11945060606097971212").toLatin1()))) return false;
    this->lastToneColor = 5;
    return true;
}

bool Music::Tone_Color_6() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(6)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("18191A1B101414151516171810111212131313141414141414151515151515161616161616161616"
            "161616161616161615151413").toLatin1()))) return false;
    this->lastToneColor = 6;
    return true;
}

bool Music::Tone_Color_7() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(7)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("48494A4B404444454546474840414242434343444444444444454545454545464646464646464646"
            "464646464646464645454443").toLatin1()))) return false;
    this->lastToneColor = 7;
    return true;
}

bool Music::Tone_Color_8() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(8)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("88898A8B808484858586878880818282838383848484848484858585858585868686868686868686"
            "868686868686868685858483").toLatin1()))) return false;
    this->lastToneColor = 8;
    return true;
}

bool Music::Tone_Color_9() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(9)) return true;
    if (!this->Write_Bytes_To_Offset(0x77B7, QByteArray(1, static_cast<char>(0x10)))) return false;
    this->lastToneColor = 9;
    return true;
}

bool Music::Tone_Color_10() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(10)) return true;
    if (!this->Write_Bytes_To_Offset(0x7F17, QByteArray(1, static_cast<char>(0xCF)))) return false;
    if (!this->Write_Bytes_To_Offset(0x7F2D, QByteArray(1, static_cast<char>(0x67)))) return false;
    if (!this->Write_Bytes_To_Offset(0x7F45, QByteArray(1, static_cast<char>(0xB3)))) return false;
    if (!this->Write_Bytes_To_Offset(0x7F72, QByteArray::fromHex(QString("021A").toLatin1()))) return false;
    this->lastToneColor = 10;
    return true;
}

bool Music::Tone_Color_11() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(11)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("4050505454585C60404449494C4C4C50505050505054545454545458585858585858585858585858"
            "585858585454504C").toLatin1()))) return false;
    this->lastToneColor = 11;
    return true;
}

bool Music::Tone_Color_12() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(12)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("80A0A0A8A8B0B8C080889090989898A0A0A0A0A0A0A8A8A8A8A8A8B0B0B0B0B0B0B0B0B0B0B0B0B0"
            "B0B0B0B0A8A8A098").toLatin1()))) return false;
    this->lastToneColor = 12;
    return true;
}

bool Music::Tone_Color_13() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(13)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("018080A0A0C0E00101204040606060808080808080A0A0A0A0A0A0C0C0C0C0C0C0C0C0C0C0C0C0C0"
            "C0C0C0C0A0A08060").toLatin1()))) return false;
    this->lastToneColor = 13;
    return true;
}

bool Music::Tone_Color_14() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(14)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("01010101010101010101010101010101010101010101010101010101010101010101010101010101"
            "0101010101010101").toLatin1()))) return false;
    this->lastToneColor = 14;
    return true;
}

bool Music::Tone_Color_15() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(15)) return true;
    if (!this->Write_Bytes_To_Offset(0x7861, QByteArray(1, static_cast<char>(0xFF)))) return false;
    this->lastToneColor = 15;
    return true;
}

bool Music::Tone_Color_16() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(16)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("50545455555657585051535353535354545454545455555555555556565656565656565656565656"
            "5656565655555453").toLatin1()))) return false;
    this->lastToneColor = 16;
    return true;
}

bool Music::Tone_Color_17() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(17)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("10141415151617181011131313131314141414141415151515151516161616161616161616161616"
            "1616161615151413").toLatin1()))) return false;
    this->lastToneColor = 17;
    return true;
}

bool Music::Tone_Color_18() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(18)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FA6, QByteArray::fromHex(QString("909192939495969798999A9B9C9D9E9F909192939395969798999A9B9C9D9E9F9091929394959597"
            "98999A9B9C9D9E9F90919292939495").toLatin1()))) return false;
    this->lastToneColor = 18;
    return true;
}

bool Music::Tone_Color_19() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(19)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("8888888888888888").toLatin1()))) return false;
    this->lastToneColor = 19;
    return true;
}

bool Music::Tone_Color_20() {
    if (this->sequentialArchiveHandler->Is_Tone_Invalid(20)) return true;
    if (!this->Write_Bytes_To_Offset(0x7FAA, QByteArray::fromHex(QString("888888888888888888").toLatin1()))) return false;
    this->lastToneColor = 20;
    return true;
}
