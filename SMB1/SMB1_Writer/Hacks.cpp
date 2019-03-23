#include "Hacks.h"
#include "Level_Offset.h"
#include "Sequential_Archive_Handler.h"
#include "Text.h"
#include <assert.h>
#include <cmath>

const static QString STRING_FIRE_BROS = "Fire Bros";
const static QString STRING_BLACK_PIRANHA_PLANTS = "Black Piranha Plants";
const static QString STRING_RED_PIRANHA_PLANTS = "Red Piranha Plants";

Hacks::Hacks(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler, Text *text) : Byte_Writer(file, levelOffset) {
    assert(sequentialArchiveHandler); assert(text);
    this->levelOffset = levelOffset;
    this->text = text;
    this->sequentialArchiveHandler = sequentialArchiveHandler;
    this->difficultyWalkingHammerBros = 11;
    this->isHammerSuitActive = false;
    this->wasCastleLoopReplacedWithAutoScrollObject = false;
}

bool Hacks::Was_Castle_Loop_Replaced_With_Autoscroll_Object() {
    return this->wasCastleLoopReplacedWithAutoScrollObject;
}

bool Hacks::Add_Luigi_Game() {
    //Based on the patch by Yy
    if (!this->Write_Bytes_To_Offset(0x607, QByteArray(1, static_cast<char>(0x7A)))) return false;
    if (!this->Write_Bytes_To_Offset(0x7A8, QByteArray::fromHex(QString("23CD05160A1B1218220C071D12160E241E19FF23").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x873, QByteArray::fromHex(QString("0BA004B9ED879904038810F76018181822220D0442423E5D5D0A09B46850EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1222, QByteArray::fromHex(QString("38EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x125C, QByteArray::fromHex(QString("38EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1292, QByteArray::fromHex(QString("AD7A07D004B924B460B9708860AD7A07D004B92BB460B9778860AD7A07D004B947B460B97E8860EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x34F4, QByteArray::fromHex(QString("2082928D0907208F92").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3584, QByteArray::fromHex(QString("209C92").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5348, QByteArray::fromHex(QString("205F8FA9044C36BCEAEAEAEAEAEAEAEAEAEAEAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x59BF, QByteArray(1, static_cast<char>(0xFA)))) return false;
    if (!this->Write_Bytes_To_Offset(0x5A02, QByteArray(1, static_cast<char>(0xFA)))) return false;
    return true;
}

bool Hacks::Always_Autoscroll() {
    //By L
    return this->Write_Bytes_To_Offset(0x2FA3, QByteArray::fromHex(QString("EAADC4032920D060A50EC90B"
                                                                           "F05AA41DAD5507C003F014C908B008AD8507F0032058D9AD2307D0404CC2AFC9509039A001").toLatin1()));
}

bool Hacks::Black_Piranha_Plants() {
    if (!this->Write_Bytes_To_Offset(0x53E2, QByteArray(1, static_cast<char>(0x13)))) return false;
    if (!this->Write_Bytes_To_Offset(0x53FF, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5416, QByteArray(1, static_cast<char>(0x00)))) return false; //reduce inactive timer to 0
    if (!this->Write_Bytes_To_Offset(0x6878, QByteArray(1, static_cast<char>(0x23)))) return false;
    return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_BLACK_PIRANHA_PLANTS);
}

bool Hacks::Bouncy_Spiny_Eggs() {
    if (!this->Write_Bytes_To_Offset(0x4101, QByteArray::fromHex(QString("D0034CE2E0B416C012D0034CD9DF20AEE1F0EF4CFFDF").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x6005, QByteArray::fromHex(QString("4CF1C0").toLatin1()));
}

bool Hacks::Disable_Intro_Demo() {
    if (!this->Write_Bytes_To_Offset(0x277, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x289, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x2D0, QByteArray::fromHex(QString("EAEAEA").toLatin1()));
}

bool Hacks::Enable_Hitting_Underwater_Blocks() {
    return this->Write_Bytes_To_Offset(0x5CEA, QByteArray(5, static_cast<char>(0xEA)));
}

bool Hacks::Enable_Piranha_Plants_On_First_Level() {
    return this->Write_Bytes_To_Offset(0x1905, QByteArray(5, static_cast<char>(0xEA)));
}

bool Hacks::Enable_Walking_Hammer_Bros(int difficulty) {
    if (difficulty < 1 || difficulty > 11) return false;
    this->difficultyWalkingHammerBros = difficulty;
    return true; //this patch will be applied when the number of worlds is set
}

bool Hacks::Fireballs_Kill_Everything_Onscreen() {
    return this->Write_Bytes_To_Offset(0x572D, QByteArray(1, static_cast<char>(0x8F)));
}

bool Hacks::Fix_Lakitu_Throw_Arc() {
    return this->Write_Bytes_To_Offset(0x4444, QByteArray::fromHex(QString("A0029558C9003001882046C3").toLatin1()));
}

bool Hacks::Fix_Life_Counter_Bugs() {
    //Allow for 2 digits on screen
    if (!this->Write_Bytes_To_Offset(0x7A4, QByteArray::fromHex(QString("240124").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x852, QByteArray::fromHex(QString("A000C90A90054C92E3").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x63A2, QByteArray::fromHex(QString("E90A18C88C08034C4488").toLatin1()))) return false;

    //Limit to 99 lives
    if (!this->Write_Bytes_To_Offset(0x4F6, QByteArray::fromHex(QString("2008C1").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3C2B, QByteArray::fromHex(QString("2008C1").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x4118, QByteArray::fromHex(QString("AE5A07E062B001E88E5A0760").toLatin1()));
}

bool Hacks::Hard_Mode_Does_Not_Affect_Lift_Size() {
    //By w7n
    if (!this->Write_Bytes_To_Offset(0x47F6, QByteArray(2, static_cast<char>(0xEA)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4840, QByteArray(9, static_cast<char>(0xEA)))) return false;
    if (!this->Write_Bytes_To_Offset(0x555B, QByteArray(2, static_cast<char>(0xEA)))) return false;
    return this->Write_Bytes_To_Offset(0x65ED, QByteArray(14, static_cast<char>(0xEA)));
}

bool Hacks::Infinite_Lives() {
    if (!this->Set_Starting_Lives(1)) return false; //set the starting lives and fix the life counter bugs first
    if (!this->Write_Bytes_To_Offset(0x04F6, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing
    if (!this->Write_Bytes_To_Offset(0x11E9, QByteArray::fromHex(QString("EAEA1890").toLatin1()))) return false; //prevent decrementing
    if (!this->Write_Bytes_To_Offset(0x3C2B, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing
    return this->Write_Bytes_To_Offset(0x06D2, QByteArray::fromHex(QString("F01CEA").toLatin1())); //skip the lives screen
}

bool Hacks::Invincibility() {
    if (!this->Write_Bytes_To_Offset(0x2F54, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false; //there's a bug here that causes Mario to walk slowly during the Pipe Intro
    if (!this->Write_Bytes_To_Offset(0x2F67, QByteArray::fromHex(QString("38EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x4E20, QByteArray::fromHex(QString("18EAEA90").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5802, QByteArray(1, static_cast<char>(0x60)))) return false;
    if (!this->Write_Bytes_To_Offset(0x582B, QByteArray(1, static_cast<char>(0x60)))) return false;
    return this->Write_Bytes_To_Offset(0x589D, QByteArray::fromHex(QString("EAEA").toLatin1()));
}

bool Hacks::Moon_Jump() {
    //if (!this->Write_Bytes_To_Offset(0x2F78, QByteArray(1, static_cast<char>(0x6A)))) return false; //turbo button presses
    return this->Write_Bytes_To_Offset(0x3497, QByteArray(1, static_cast<char>(0x13))); //jump while in midair
}

bool Hacks::Permadeath() {
    if (!this->Set_Starting_Lives(1)) return false; //set the starting lives and fix the life counter bugs first
    if (!this->Write_Bytes_To_Offset(0x02ED,  QByteArray(1, static_cast<char>(0x18)))) return false; //disable the A+Start continue cheat code
    if (!this->Write_Bytes_To_Offset(0x04F6, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing
    if (!this->Write_Bytes_To_Offset(0x3C2B, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing
    return this->Write_Bytes_To_Offset(0x06D2, QByteArray::fromHex(QString("F01CEA").toLatin1())); //skip the lives screen
}

bool Hacks::Real_Time() {
    if (!this->Write_Bytes_To_Offset(0x113E, QByteArray::fromHex(QString("030201").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3784, QByteArray::fromHex(QString("ADF807D00DADF907C904D006ADFA074C0BC1A940").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x411B, QByteArray::fromHex(QString("D004A94085FC4C86B7").toLatin1()));
}

bool Hacks::Red_Piranha_Plants() {
    if (!this->Write_Bytes_To_Offset(0x53E2, QByteArray(1, static_cast<char>(0x13)))) return false;
    if (!this->Write_Bytes_To_Offset(0x53FF, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x6878, QByteArray(1, static_cast<char>(0x22)))) return false;
    return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_RED_PIRANHA_PLANTS);
}

bool Hacks::Remove_Vertical_Object_Limit() {
    //by Chacky
    if (!this->Write_Bytes_To_Offset(0x1031, QByteArray::fromHex(QString("A20ADE8504CA10FAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x111F, QByteArray(1, static_cast<char>(0x00)))) return false;
    if (!this->Write_Bytes_To_Offset(0x1140, QByteArray(1, static_cast<char>(0x01)))) return false;
    if (!this->Write_Bytes_To_Offset(0x1519, QByteArray(1, static_cast<char>(0x0A)))) return false;
    if (!this->Write_Bytes_To_Offset(0x152B, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1584, QByteArray::fromHex(QString("85043003DE8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x15A6, QByteArray::fromHex(QString("85043003BC5704").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1629, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x166A, QByteArray::fromHex(QString("5704").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x16D6, QByteArray::fromHex(QString("5704").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1760, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1768, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1790, QByteArray::fromHex(QString("85044A9DF404").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x179E, QByteArray::fromHex(QString("8504F01EBDF404").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x17D3, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1823, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1849, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1883, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x18C8, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x190B, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1957, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1BC0, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x1BC7, QByteArray::fromHex(QString("85043860BC5704").toLatin1()));
}

bool Hacks::Replace_Castle_Loop_With_Autoscroll_Object() {
    //By ATA
    if (!this->Write_Bytes_To_Offset(0x16C9, QByteArray::fromHex(QString("E6C0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x2F46, QByteArray::fromHex(QString("6BC0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x2FAD, QByteArray::fromHex(QString("20B7C0F0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x406D, QByteArray::fromHex(QString("2FC1").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x407B, QByteArray::fromHex(QString("20AAC0D01DA50EC908D017AC4E07BEA6C0D007A5094AB00AA2018AA820C4AF206FAF4C4FB7").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x40B6, QByteArray::fromHex(QString("01010101AD2307D005A541C90160A9FD6020AAC0F002C9FD6020A1DFB02120BDDEF01B4820AAC0D0"
            "14A586290FC90BB00CAD5507C902B005A2002058D9681860A5414901854160").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x41B9, QByteArray::fromHex(QString("2FC1").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5DBA, QByteArray::fromHex(QString("BFC0").toLatin1()))) return false;
    this->wasCastleLoopReplacedWithAutoScrollObject = true;
    return true;
}

bool Hacks::Replace_Castle_Loop_With_Fire_Bros() {
    if (!this->Write_Bytes_To_Offset(0x3AA2, QByteArray::fromHex(QString("1CE4").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3AEB, QByteArray(1, static_cast<char>(0x38)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3B01, QByteArray::fromHex(QString("6BC0A90495AC2095C0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3B36, QByteArray(1, static_cast<char>(0x60)))) return false;
    if (!this->Write_Bytes_To_Offset(0x407B, QByteArray::fromHex(QString("8A18690DAAA01BA90020F0E3A608C900F00EB5AC300DA9FD95ACB5DB29F895DB4C28BBA900952A4C"
            "28BBA92085FFB91E0060").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x64E8, QByteArray::fromHex(QString("0202C2C2").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x6870, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6881, QByteArray(2, static_cast<char>(0x02)))) return false; //red palette for Bowser
    if (!this->isHammerSuitActive) {
        if (!this->Write_Bytes_To_Offset(0x8810, QByteArray::fromHex(QString("000000003C7E77FB0000000000183C0E9F5F8E20000000000E040000000000000502080307070703"
                "0000000001030100C0E0F0F0B070E0C000004060E0C08000").toLatin1()))) return false;
    }
    return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_FIRE_BROS);
}

bool Hacks::Replace_Castle_Loop_With_Top_Of_Flagpole_Gives_1UP() {
    if (!this->Write_Bytes_To_Offset(0x407B, QByteArray::fromHex(QString("AC0F01D007EE5A07A94085FE60").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x38A9, QByteArray::fromHex(QString("206BC0").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x6551, QByteArray::fromHex(QString("FDFE").toLatin1()));
}

bool Hacks::Replace_Mario_With_Luigi() {
    if (!this->Write_Bytes_To_Offset(0x609, QByteArray(1, static_cast<char>(0xA9)))) return false;
    if (!this->Write_Bytes_To_Offset(0x870, QByteArray(1, static_cast<char>(0x72)))) return false;
    if (!this->Write_Bytes_To_Offset(0x42C, QByteArray(1, static_cast<char>(0x5A)))) return false;
    if (!this->Write_Bytes_To_Offset(0x42E, QByteArray(1, static_cast<char>(0x70)))) return false;

    QByteArray luigiText;
    luigiText.append(static_cast<char>(0x15));
    luigiText.append(static_cast<char>(0x1E));
    luigiText.append(static_cast<char>(0x12));
    luigiText.append(static_cast<char>(0x10));
    luigiText.append(static_cast<char>(0x12));
    return this->Write_Bytes_To_Offset(0x765, luigiText); //change name above score
}

bool Hacks::Set_Basic_Enemy_Speed(int speed) {
    if (speed < 1 || speed > 8) return false;
    QByteArray speedBytes;
    switch (speed) {
    default:    assert(false); return false;
    case 1:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x01)));
    case 2:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x02)));
    case 3:
        speedBytes.append(static_cast<char>(0xE8));
        speedBytes.append(static_cast<char>(0xE4));
        return this->Write_Bytes_To_Offset(0x431C, speedBytes);
    case 4:
        speedBytes.append(static_cast<char>(0xD8));
        speedBytes.append(static_cast<char>(0xD4));
        return this->Write_Bytes_To_Offset(0x431C, speedBytes);
    case 5:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x08)));
    case 6:
        speedBytes.append(static_cast<char>(0xC8));
        speedBytes.append(static_cast<char>(0xC4));
        return this->Write_Bytes_To_Offset(0x431C, speedBytes);
    case 7:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x0D)));
    case 8:
        return this->Write_Bytes_To_Offset(0x431F, QByteArray(1, static_cast<char>(0x0B)));
    }
}

bool Hacks::Set_Bullet_Bill_Speed(int speed) {
    int speedValue = 0;
    switch (speed) {
    default: return false;
    case 1: speedValue = 0x10; break; //Slow
    case 2: speedValue = 0x18; break; //Normal (Original)
    case 3: speedValue = 0x20; break; //Fast
    case 4: speedValue = 0x28; break; //Speedy
    case 5: speedValue = 0x50; break; //Ludicrous
    }
    int invertedSpeedValue = 0x100-speedValue;
    if (!this->Write_Bytes_To_Offset(0x3A41, QByteArray(1, static_cast<char>(speedValue)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3A42, QByteArray(1, static_cast<char>(invertedSpeedValue)))) return false;
    return this->Write_Bytes_To_Offset(0x4C50, QByteArray(1, static_cast<char>(invertedSpeedValue)));
}

bool Hacks::Set_Number_Of_Worlds(int value) {
    if (value < 0 || value > 8) return false;
    int numWorlds = value;
    int startHardModeOnWorld = value/2;
    --value; //value should be 0 based
    QByteArray worldByte;
    worldByte.append(static_cast<char>(value));

    //These offsets must be patched for Number of Worlds to be set properly
    if (!this->Write_Bytes_To_Offset(0x02B2, QByteArray(1, static_cast<char>(0x00)))) return false; //disable to B button to select world
    if (!this->Write_Bytes_To_Offset(0x0438, worldByte)) return false;
    if (!this->Write_Bytes_To_Offset(0x047A, worldByte)) return false;
    if (!this->Write_Bytes_To_Offset(0x6A27, worldByte)) return false;

    //Correct hard mode activator
    if (!this->Write_Bytes_To_Offset(0x1054, QByteArray(1, static_cast<char>(0x00)))) return false;
    if (!this->Write_Bytes_To_Offset(0x104B, QByteArray(1, static_cast<char>(startHardModeOnWorld)))) return false;
    if (!this->Write_Bytes_To_Offset(0x512B, QByteArray(1, static_cast<char>(startHardModeOnWorld)))) return false;

    //Apply the Walking Hammer Bros patch
    assert(this->difficultyWalkingHammerBros >= 1 && this->difficultyWalkingHammerBros <= 11);
    if (this->difficultyWalkingHammerBros == 11) return true;
    int walkingHammerBrosWorld = 0;
    assert(this->Convert_Difficulty_To_World(this->difficultyWalkingHammerBros, numWorlds, walkingHammerBrosWorld));
    assert(walkingHammerBrosWorld >= 1 && walkingHammerBrosWorld <= 8);
    return this->Enable_Walking_Hammer_Bros_In_World(walkingHammerBrosWorld);
}

bool Hacks::Set_Lakitu_Respawn_Speed(int value) {
    //A value of 0x10 will disable respawn completely
    if (value < 0 || value > 0x10) return false;
    if (value == 0x10) return this->Write_Bytes_To_Offset(0x43D4, QByteArray::fromHex(QString("38EAB0").toLatin1()));
    else return this->Write_Bytes_To_Offset(0x43D5, QByteArray(1, static_cast<char>(value)));
}

bool Hacks::Set_Starting_Lives(int lives) {
    if (lives <= 0 || lives > 0x80) return false;
    if (!this->Write_Bytes_To_Offset(0x107A, QByteArray(1, static_cast<char>(lives-1)))) return false;
    if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
        if (!this->Write_Bytes_To_Offset(0x0256, QByteArray(1, static_cast<char>(lives-1)))) return false;
    }
    return true;
}

void Hacks::Set_Hammer_Suit_Active(bool isHammerSuitActive) {
    this->isHammerSuitActive = isHammerSuitActive;
}

bool Hacks::Speedy_Objects_And_Enemies() {
    //by L
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("50B0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x36D4, QByteArray(1, static_cast<char>(0x60)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3AA2, QByteArray::fromHex(QString("20E0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3AEB, QByteArray(1, static_cast<char>(0x20)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3CD4, QByteArray(1, static_cast<char>(0x20)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3F89, QByteArray(1, static_cast<char>(0x06)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3F8D, QByteArray(1, static_cast<char>(0x0C)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3F91, QByteArray(1, static_cast<char>(0x06)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3F9D, QByteArray::fromHex(QString("14A906").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3FA3, QByteArray::fromHex(QString("2CA908").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3FCE, QByteArray(1, static_cast<char>(0x0A)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3FD8, QByteArray(1, static_cast<char>(0x14)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3FDC, QByteArray(1, static_cast<char>(0x06)))) return false;
    if (!this->Write_Bytes_To_Offset(0x445F, QByteArray::fromHex(QString("3848384838").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x44A8, QByteArray::fromHex(QString("1E15161E2C30201C2E322824").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x44E4, QByteArray(1, static_cast<char>(0xF9)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4574, QByteArray(1, static_cast<char>(0x10)))) return false;
    if (!this->Write_Bytes_To_Offset(0x47E6, QByteArray(1, static_cast<char>(0xE8)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4861, QByteArray(1, static_cast<char>(0xFE)))) return false;
    if (!this->Write_Bytes_To_Offset(0x486D, QByteArray(1, static_cast<char>(0x01)))) return false;
    if (!this->Write_Bytes_To_Offset(0x49D3, QByteArray(1, static_cast<char>(0x05)))) return false;
    if (!this->Write_Bytes_To_Offset(0x49D8, QByteArray(1, static_cast<char>(0xF7)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4A82, QByteArray(1, static_cast<char>(0xE8)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4B56, QByteArray(1, static_cast<char>(0x23)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4B5C, QByteArray(1, static_cast<char>(0x01)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4C07, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4C27, QByteArray(1, static_cast<char>(0x01)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4C7C, QByteArray(1, static_cast<char>(0x01)))) return false;
    if (!this->Write_Bytes_To_Offset(0x4F01, QByteArray(1, static_cast<char>(0x1E)))) return false;
    if (!this->Write_Bytes_To_Offset(0x50D4, QByteArray(1, static_cast<char>(0x10)))) return false;
    if (!this->Write_Bytes_To_Offset(0x51E1, QByteArray::fromHex(QString("6F206F6F6F20206F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5217, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x55F7, QByteArray(1, static_cast<char>(0x03)))) return false;
    if (!this->Write_Bytes_To_Offset(0x5618, QByteArray(1, static_cast<char>(0x18)))) return false;
    if (!this->Write_Bytes_To_Offset(0x5658, QByteArray(1, static_cast<char>(0x20)))) return false;
    if (!this->Write_Bytes_To_Offset(0x585F, QByteArray::fromHex(QString("40C018E8").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5FCF, QByteArray::fromHex(QString("20E0").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x60C4, QByteArray(1, static_cast<char>(0x18)));
}

bool Hacks::Start_Underwater_Castle_Brick_On_World(int world) {
    if (world < 1 || world > 0xFF) return false;
    return this->Write_Bytes_To_Offset(0x1486, QByteArray(1, static_cast<char>(world-1)));
}

bool Hacks::Start_With_Fire_Flower_On_Room_Change() {
    if (!this->Write_Bytes_To_Offset(0x5B4, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x5C1, QByteArray(1, static_cast<char>(0xAD)))) return false;
    if (!this->Write_Bytes_To_Offset(0x1072, QByteArray::fromHex(QString("008D5707AD").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x1278, QByteArray(1, static_cast<char>(0x00)));
}

bool Hacks::Taking_Damage_As_Fire_Reverts_To_Super() {
    //JMP $C114
    if (!this->Write_Bytes_To_Offset(0x3270, QByteArray::fromHex(QString("4C14C1").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5946, QByteArray(1, static_cast<char>(0xCE)))) return false;
    //$C114
    return this->Write_Bytes_To_Offset(0x4124, QByteArray::fromHex(QString("AD5607F005AD5407F0034C63B28D0B07A901851DA9C98D470760").toLatin1()));
}

bool Hacks::Unlimited_Time() {
    if (!this->Write_Bytes_To_Offset(0x0774, QByteArray::fromHex(QString("24242424").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3776, QByteArray(1, static_cast<char>(0xAD)))) return false;
    return this->Write_Bytes_To_Offset(0x52FC, QByteArray(1, static_cast<char>(0x4E)));
}

bool Hacks::Write_Watermark() {
    //Change the end game text
    if (!this->Write_Bytes_To_Offset(0x0DBB, this->text->Convert_String_To_SMB_Bytes("   Level-Headed    "))) return false;
    if (!this->Write_Bytes_To_Offset(0x0DD2, this->text->Convert_String_To_SMB_Bytes(" Visit Coolcord on Github  "))) return false;
    if (!this->Write_Bytes_To_Offset(0x0DF1, this->text->Convert_String_To_SMB_Bytes("For Updates! "))) return false;
    if (!this->Write_Bytes_To_Offset(0x0E02, this->text->Convert_String_To_SMB_Bytes("                 "))) return false;
    if (!this->Start_Underwater_Castle_Brick_On_World(9)) return false; //disables underwater castle bricks
    if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
        if (!this->Disable_Intro_Demo()) return false; //if Luigi dies in the intro demo, a game will start
    }
    return true;
}

bool Hacks::Convert_Difficulty_To_World(int difficulty, int numWorlds, int &world) {
    if (difficulty < 1 || difficulty > 10) return false;
    if (difficulty == 1) {
        world = 1;
        return true;
    }
    double difficultyDouble = static_cast<double>(difficulty)/10.0, numWorldsDouble = static_cast<double>(numWorlds);
    world = static_cast<int>(std::ceil(difficultyDouble*numWorldsDouble));
    assert(world <= numWorlds);
    return true;
}

bool Hacks::Enable_Walking_Hammer_Bros_In_World(int world) {
    //by YY
    --world;
    if (world < 0 || world > 0xF) return false;
    return this->Write_Bytes_To_Offset(0x433A, QByteArray::fromHex(QString("9558AD5F07C90"+QString::number(world)+"B005A980").toLatin1()));
}
