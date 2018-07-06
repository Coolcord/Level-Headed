#include "Hacks.h"
#include "Level_Offset.h"
#include "Sequential_Archive_Handler.h"
#include <assert.h>

Hacks::Hacks(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler) : Byte_Writer(file, levelOffset) {
    assert(sequentialArchiveHandler);
    this->sequentialArchiveHandler = sequentialArchiveHandler;
}

bool Hacks::Add_Luigi_Game() {
    //Based on the patch by Yy
    if (!this->Write_Bytes_To_Offset(0x607, QByteArray(1, 0x7A))) return false;
    if (!this->Write_Bytes_To_Offset(0x7A8, QByteArray::fromHex(QString("23CD05160A1B1218220C071D12160E241E19FF23").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x873, QByteArray::fromHex(QString("0BA004B9ED879904038810F76018181822220D0442423E5D5D0A09B46850EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1222, QByteArray::fromHex(QString("38EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x125C, QByteArray::fromHex(QString("38EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1292, QByteArray::fromHex(QString("AD7A07D004B924B460B9708860AD7A07D004B92BB460B9778860AD7A07D004B947B460B97E8860EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x34F4, QByteArray::fromHex(QString("2082928D0907208F92").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3584, QByteArray::fromHex(QString("209C92").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5348, QByteArray::fromHex(QString("205F8FA9044C36BCEAEAEAEAEAEAEAEAEAEAEAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x59BF, QByteArray(1, 0xFA))) return false;
    if (!this->Write_Bytes_To_Offset(0x5A02, QByteArray(1, 0xFA))) return false;
    //return this->Write_Bytes_To_Offset(0x9FC6, QByteArray::fromHex(QString("160A1B121824100A160E242424228B0D151E12101224100A160E242424").toLatin1()));
    return true; //the line above is no longer necessary with the new title screen
}

bool Hacks::Always_Autoscroll() {
    //By L
    return this->Write_Bytes_To_Offset(0x2FA3, QByteArray::fromHex(QString("EAADC4032920D060A50EC90B"
        "F05AA41DAD5507C003F014C908B008AD8507F0032058D9AD2307D0404CC2AFC9509039A001").toLatin1()));
}

bool Hacks::Disable_Intro_Demo() {
    if (!this->Write_Bytes_To_Offset(0x0350, QByteArray(21, 0x00))) return false;
    return this->Write_Bytes_To_Offset(0x0365, QByteArray(20, 0xFF));
}

bool Hacks::Enable_Hitting_Underwater_Blocks() {
    return this->Write_Bytes_To_Offset(0x5CEA, QByteArray(5, 0xEA));
}

bool Hacks::Enable_Piranha_Plants_On_First_Level() {
    return this->Write_Bytes_To_Offset(0x1905, QByteArray(5, 0xEA));
}

bool Hacks::Fast_Enemies(int speed) {
    if (speed < 1 || speed > 8) return false;
    QByteArray speedBytes;
    switch (speed) {
    default:    assert(false);
    case 1:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x01)));
    case 2:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x02)));
    case 3:
        speedBytes.append(0xE8);
        speedBytes.append(0xE4);
        return this->Write_Bytes_To_Offset(0x431C, speedBytes);
    case 4:
        speedBytes.append(0xD8);
        speedBytes.append(0xD4);
        return this->Write_Bytes_To_Offset(0x431C, speedBytes);
    case 5:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x08)));
    case 6:
        speedBytes.append(0xC8);
        speedBytes.append(0xC4);
        return this->Write_Bytes_To_Offset(0x431C, speedBytes);
    case 7:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x0D)));
    case 8:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x0B)));
    }
}

bool Hacks::Fireballs_Kill_Everything_Onscreen() {
    return this->Write_Bytes_To_Offset(0x572D, QByteArray(1, 0x8F));
}

bool Hacks::Fix_Lakitu_Throw_Arc() {
    return this->Write_Bytes_To_Offset(0x4444, QByteArray::fromHex(QString("A0029558C9003001882046C3").toLatin1()));
}

bool Hacks::Hard_Mode_Does_Not_Affect_Lift_Size() {
    //By w7n
    if (!this->Write_Bytes_To_Offset(0x47F6, QByteArray(2, 0xEA))) return false;
    if (!this->Write_Bytes_To_Offset(0x4840, QByteArray(9, 0xEA))) return false;
    if (!this->Write_Bytes_To_Offset(0x555B, QByteArray(2, 0xEA))) return false;
    return this->Write_Bytes_To_Offset(0x65ED, QByteArray(14, 0xEA));
}

bool Hacks::Infinite_Lives() {
    return this->Write_Bytes_To_Offset(0x11E9, QByteArray(1, 0xAD));
}

bool Hacks::Invincibility() {
    if (!this->Write_Bytes_To_Offset(0x399D, QByteArray(1, 0xFF))) return false;
    if (!this->Write_Bytes_To_Offset(0x589B, QByteArray(1, 0xFF))) return false;
    if (!this->Write_Bytes_To_Offset(0x119E, QByteArray(1, 0xC8))) return false; //gives Mario Star Power, but it wears off after a while
    return this->Write_Bytes_To_Offset(0x5945, QByteArray(1, 0x03)); //if Mario gets damaged, revert to Super Mario
}

bool Hacks::Moon_Jump() {
    //if (!this->Write_Bytes_To_Offset(0x2F78, QByteArray(1, 0x6A))) return false; //turbo button presses
    return this->Write_Bytes_To_Offset(0x3497, QByteArray(1, 0x13)); //jump while in midair
}

bool Hacks::Replace_Castle_Loop_With_Autoscroll_Object(int overworldSpeed, int undergroundSpeed, int underwaterSpeed, int castleSpeed) {
    //Make sure the speed values are valid
    const int MAX_SPEED = 3;
    if (overworldSpeed < 0 || overworldSpeed > MAX_SPEED) return false;
    if (undergroundSpeed < 0 || undergroundSpeed > MAX_SPEED) return false;
    if (underwaterSpeed < 0 || underwaterSpeed > MAX_SPEED) return false;
    if (castleSpeed < 0 || castleSpeed > MAX_SPEED) return false;

    //By ATA
    if (!this->Write_Bytes_To_Offset(0x16C9, QByteArray::fromHex(QString("E6C0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x2F46, QByteArray::fromHex(QString("6BC0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x2FAD, QByteArray::fromHex(QString("20B7C0F0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x406D, QByteArray::fromHex(QString("2FC168290FA8B90F00D00295"
        "0F6020AAC0D01DA50EC908D017AC4E07BEA6C0D007A5094AB00AA2018AA820C4AF206FAF4C4FB704856DAD250738E9048D2507AD1A0738E9048D1A07"
        "AD01010101AD2307D005A541C90160A9FD6020AAC0F002C9FD6020A1DFB02120BDDEF01B4820AAC0D014A586290FC90BB00CAD5507C902B005A2002058D9681860A5414901854160").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x41B9, QByteArray::fromHex(QString("2FC1").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5DBA, QByteArray::fromHex(QString("BFC0").toLatin1()))) return false;

    //Patch the speeds
    if (!this->Write_Bytes_To_Offset(0x40B6, QByteArray(1, static_cast<char>(underwaterSpeed)))) return false;
    if (!this->Write_Bytes_To_Offset(0x40B7, QByteArray(1, static_cast<char>(overworldSpeed)))) return false;
    if (!this->Write_Bytes_To_Offset(0x40B8, QByteArray(1, static_cast<char>(undergroundSpeed)))) return false;
    if (!this->Write_Bytes_To_Offset(0x40B9, QByteArray(1, static_cast<char>(castleSpeed)))) return false;
    return true;
}

bool Hacks::Replace_Fire_Flower_With_Hammer_Suit() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("30270F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("27D8").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x36C4, QByteArray::fromHex(QString("FB95A6A9079DA004D6248A18"
        "6907AAA9508500A9468502A90020D7BF200FBFA608203BF12087F1202DE220F1E1ADD20329CCD00620D9D64CC8E1A900952460203BF14C61").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3CAD, QByteArray(4, 0xEA))) return false;
    if (!this->Write_Bytes_To_Offset(0x5735, QByteArray(2, 0xEA))) return false;
    if (!this->Write_Bytes_To_Offset(0x5764, QByteArray(4, 0xEA))) return false;
    if (!this->Write_Bytes_To_Offset(0x579A, QByteArray::fromHex(QString("33F00AEAEAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61D8, QByteArray::fromHex(QString("BCF106ADBA03990002ADAF03"
        "990302A5094A4A4829010A4980990102684A4AA903900209C099020260A900953A60FFFFFFFFFFFFFF").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, 0x03))) return false;
    if (!this->Write_Bytes_To_Offset(0x6720, QByteArray::fromHex(QString("C901F0178500A5094A29030DCA03990202990602EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("071F1F3F3F7A72300000000F18252D").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("1827273F1D1F0F0707181B00020006").toLatin1()))) return false;
    return this->sequentialArchiveHandler->Apply_Hammer_Suit_Fix();
}

bool Hacks::Replace_Mario_With_Luigi() {
    if (!this->Write_Bytes_To_Offset(0x609, QByteArray(1, 0xA9))) return false;
    if (!this->Write_Bytes_To_Offset(0x870, QByteArray(1, 0x72))) return false;
    if (!this->Write_Bytes_To_Offset(0x42C, QByteArray(1, 0x5A))) return false;
    if (!this->Write_Bytes_To_Offset(0x42E, QByteArray(1, 0x70))) return false;

    QByteArray luigiText;
    luigiText.append(static_cast<char>(0x15));
    luigiText.append(static_cast<char>(0x1E));
    luigiText.append(static_cast<char>(0x12));
    luigiText.append(static_cast<char>(0x10));
    luigiText.append(static_cast<char>(0x12));
    return this->Write_Bytes_To_Offset(0x765, luigiText); //change name above score
}

bool Hacks::Set_Starting_Lives(int lives) {
    if (lives <= 0 && lives > 0x80) return false;
    return this->Write_Bytes_To_Offset(0x107A, QByteArray(1, static_cast<char>(lives-1)));
}

bool Hacks::Star_Color_Mario() {
    return this->Write_Bytes_To_Offset(0x2F53, QByteArray(1, 0xE7));
}

bool Hacks::Start_Underwater_Castle_Brick_On_World(int world) {
    if (world < 1 || world > 0xFF) return false;
    return this->Write_Bytes_To_Offset(0x1486, QByteArray(1, world-1));
}

bool Hacks::Taking_Damage_As_Fire_Reverts_To_Super() {
    //JMP $C114
    if (!this->Write_Bytes_To_Offset(0x3270, QByteArray::fromHex(QString("4C14C1").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5946, QByteArray(1, 0xCE))) return false;
    //$C114
    return this->Write_Bytes_To_Offset(0x4124, QByteArray::fromHex(QString("AD5607F005AD5407F0034C63B28D0B07A901851DA9C98D470760").toLatin1()));
}

bool Hacks::Unlimited_Time() {
    if (!this->Write_Bytes_To_Offset(0x0774, QByteArray::fromHex(QString("24242424").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3776, QByteArray(1, 0xAD))) return false;
    return this->Write_Bytes_To_Offset(0x52FC, QByteArray(1, 0x4E));
}

bool Hacks::Write_Watermark() {
    //Change the end game text
    if (!this->Write_Bytes_To_Offset(0x0DBB, QByteArray::fromHex(QString("242424150E1F0E1528110E0A0D0E0D242424240025E31B241F121C121D240C1818150C181B0D2418172410121D111E0B242400264A0D0F181B241E190D0A1D0E1C2B24002688112424242424242424242424242424242424").toLatin1()))) return false;
    return this->Start_Underwater_Castle_Brick_On_World(9); //disables underwater castle bricks
}
