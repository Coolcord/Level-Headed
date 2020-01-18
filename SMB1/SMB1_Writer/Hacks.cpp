#include "Hacks.h"
#include "../../Common_Files/Random.h"
#include "Bowser_Bridge_Destroyer.h"
#include "Graphics.h"
#include "Level_Offset.h"
#include "Midpoint_Writer.h"
#include "Powerups.h"
#include "Sequential_Archive_Handler.h"
#include "Text.h"
#include <QDebug>
#include <QVector>
#include <assert.h>
#include <cmath>

const static QString STRING_FIRE_BROS = "Fire Bros";
const static QString STRING_BLACK_PIRANHA_PLANTS = "Black Piranha Plants";
const static QString STRING_RED_PIRANHA_PLANTS = "Red Piranha Plants";
const static QString STRING_SPINY_EGGS_EXPLODE_INTO_FLAMES = "Spiny Eggs Explode Into Flames";

Hacks::Hacks(QFile *f, Level_Offset *lo, Midpoint_Writer *midpointWriter, Sequential_Archive_Handler *sequentialArchiveHandler, Text *text) : Byte_Writer(f, lo) {
    assert(midpointWriter); assert(sequentialArchiveHandler); assert(text);
    this->powerups = nullptr;
    this->levelOffset = levelOffset;
    this->text = text;
    this->midpointWriter = midpointWriter;
    this->sequentialArchiveHandler = sequentialArchiveHandler;
    this->difficultyWalkingHammerBros = 11;
    this->spinyEggSpeedCap = 0x18;
    this->skipLivesScreen = false;
    this->isHammerSuitActive = false;
    this->permadeath = false;
    this->wasCastleLoopReplacedWithAutoScrollObject = false;
    this->wasCastleLoopReplacedWithFireBros = false;
    this->wasCastleLoopReplacedWithFlagpole1UP = false;
    this->wasCastleLoopReplacedWithFireFlower = false;
    this->wasVerticalObjectLimitRemoved = false;
}

void Hacks::Set_Graphics(Graphics *graphics) {
    assert(graphics); this->graphics = graphics;
}

void Hacks::Set_Powerups(Powerups *powerups) {
    assert(powerups); this->powerups = powerups;
}

bool Hacks::Was_Castle_Loop_Replaced_With_Autoscroll_Object() {
    return this->wasCastleLoopReplacedWithAutoScrollObject;
}

bool Hacks::Was_Vertical_Object_Limit_Removed() {
    return this->wasVerticalObjectLimitRemoved;
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
    if (!this->Write_Bytes_To_Offset(0x5348, QByteArray::fromHex(QString("205F8FA9044C36BC").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x59BF, QByteArray(1, static_cast<char>(0xFA)))) return false;
    if (!this->Write_Bytes_To_Offset(0x5A02, QByteArray(1, static_cast<char>(0xFA)))) return false;
    return true;
}

bool Hacks::Always_Autoscroll() {
    //By L
    return this->Write_Bytes_To_Offset(0x2FA3, QByteArray::fromHex(QString("EAADC4032920D060A50EC90B"
                                                                           "F05AA41DAD5507C003F014C908B008AD8507F0032058D9AD2307D0404CC2AFC9509039A001").toLatin1()));
}

bool Hacks::Always_Force_Continue_Code() {
    //Note: With this hack, once the game has been beaten, it is impossible to start from World 1 unless the game is reset!
    if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
        return false; //not compatible
    } else {
        return this->Write_Bytes_To_Offset(0x02EE, QByteArray::fromHex(QString("EAEA").toLatin1()));
    }
}

bool Hacks::Black_Piranha_Plants() {
    if (!this->Write_Bytes_To_Offset(0x53E2, QByteArray(1, static_cast<char>(0x0D)))) return false;
    if (!this->Write_Bytes_To_Offset(0x53FF, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5416, QByteArray(1, static_cast<char>(0x00)))) return false; //reduce inactive timer to 0
    if (!this->Write_Bytes_To_Offset(0x6878, QByteArray(1, static_cast<char>(0x23)))) return false;
    return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_BLACK_PIRANHA_PLANTS);
}

bool Hacks::Destroy_Bowser_Bridge_Randomly() {
    Bowser_Bridge_Destroyer bowserBridgeDestroyer(this->file, this->levelOffset);
    if (Random::Get_Instance().Get_Num(1)) { //orderly
        switch (Random::Get_Instance().Get_Num(3)) {
        default: assert(false); return false;
        case 0: return bowserBridgeDestroyer.Forwards();
        case 1: return bowserBridgeDestroyer.Backwards();
        case 2: return bowserBridgeDestroyer.From_Center();
        case 3: return bowserBridgeDestroyer.To_Center();
        }
    } else { //chaotic
        if (Random::Get_Instance().Get_Num(1)) { //chaotic neutral
            if (Random::Get_Instance().Get_Num(1)) return bowserBridgeDestroyer.Alternating();
            else return bowserBridgeDestroyer.Grouped();
        } else { //complete chaos
            return bowserBridgeDestroyer.Chaotic();
        }
    }
}

bool Hacks::Disable_Intro_Demo() {
    if (!this->Write_Bytes_To_Offset(0x277, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x289, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x2D0, QByteArray::fromHex(QString("EAEAEA").toLatin1()));
}

bool Hacks::Disable_One_Up_Coin_Limits() {
    return this->Write_Bytes_To_Offset(0x32D2, QByteArray(8, static_cast<char>(0x00)));
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

bool Hacks::European_Blooper_Swim_Height() {
    return this->Write_Bytes_To_Offset(0x4C3C, QByteArray(1, static_cast<char>(0x0C)));
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
    if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
        if (!this->Write_Bytes_To_Offset(0x11E0, QByteArray::fromHex(QString("EAEAEAEAEAEAEAEAEAEAEA1890103005EAEA1890").toLatin1()))) return false; //prevent decrementing
    } else {
        if (!this->Write_Bytes_To_Offset(0x11E9, QByteArray::fromHex(QString("EAEA1890").toLatin1()))) return false; //prevent decrementing
    }
    if (!this->Write_Bytes_To_Offset(0x04F6, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing
    if (!this->Write_Bytes_To_Offset(0x3C2B, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing
    return this->Skip_Lives_Screen();
}

bool Hacks::Invincibility() {
    if (!this->Write_Bytes_To_Offset(0x2F51, QByteArray::fromHex(QString("A923EA").toLatin1()))) return false; //make Mario flash
    if (!this->Write_Bytes_To_Offset(0x2F67, QByteArray::fromHex(QString("38EA").toLatin1()))) return false; //make Mario flash quickly
    if (!this->Write_Bytes_To_Offset(0x4E20, QByteArray::fromHex(QString("18EAEA90").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5802, QByteArray(1, static_cast<char>(0x60)))) return false;
    if (!this->Write_Bytes_To_Offset(0x582B, QByteArray(1, static_cast<char>(0x60)))) return false;
    return this->Write_Bytes_To_Offset(0x589D, QByteArray::fromHex(QString("EAEA").toLatin1()));
}

bool Hacks::Mario_Stands_Still_At_Intro_Demo() {
    QByteArray buttons(21, static_cast<char>(0x00));
    QByteArray timings(22, static_cast<char>(0xFF));
    timings.data()[timings.size()-1] = static_cast<char>(0x00); //terminator byte
    if (!this->Write_Bytes_To_Offset(0x0350, buttons)) return false;
    return this->Write_Bytes_To_Offset(0x365, timings);
}

bool Hacks::Moon_Jump() {
    //if (!this->Write_Bytes_To_Offset(0x2F78, QByteArray(1, static_cast<char>(0x6A)))) return false; //turbo button presses
    return this->Write_Bytes_To_Offset(0x3497, QByteArray(1, static_cast<char>(0x13))); //jump while in midair
}

bool Hacks::Only_Jump_At_Intro_Demo() {
    QByteArray buttons(21, static_cast<char>(0x00));
    QByteArray timings(22, static_cast<char>(0x20));

    //Handle Button Presses
    for (int i = 0; i < buttons.size(); ++i) {
        if (i%2 == 0) { //jump
            buttons.data()[i] = buttons.data()[i]|static_cast<char>(0x80);
            switch (Random::Get_Instance().Get_Num(5)) {
            case 0:     timings.data()[i] = static_cast<char>(0x01); break; //lowest jump
            case 1:     timings.data()[i] = static_cast<char>(0x05); break;
            case 2:     timings.data()[i] = static_cast<char>(0x07); break;
            case 3:     timings.data()[i] = static_cast<char>(0x10); break;
            case 4:     timings.data()[i] = static_cast<char>(0x15); break;
            case 5:     timings.data()[i] = static_cast<char>(0x2F); break; //highest jump
            }
        } else { //do nothing
            buttons.data()[i] = static_cast<char>(0x00); //do nothing
            if (Random::Get_Instance().Get_Num(7) == 0) timings.data()[i] = static_cast<char>(Random::Get_Instance().Get_Num(0x40, 0x80)); //long wait
            else timings.data()[i] = static_cast<char>(Random::Get_Instance().Get_Num(0x08, 0x2F)); //short wait (the low end is intentionally too short to possibly skip a jump)
        }
    }
    buttons.data()[buttons.size()-1] = static_cast<char>(0x00); //do nothing for last action
    timings.data()[timings.size()-2] = static_cast<char>(0xFF); //max wait time for last action
    timings.data()[timings.size()-1] = static_cast<char>(0x00); //terminator byte

    if (!this->Write_Bytes_To_Offset(0x0350, buttons)) return false;
    return this->Write_Bytes_To_Offset(0x365, timings);
}

bool Hacks::Permadeath() {
    if (!this->Set_Starting_Lives(1)) return false; //set the starting lives and fix the life counter bugs first

    //Disable the A+Start continue cheat code
    if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
        if (!this->Write_Bytes_To_Offset(0x1261,  QByteArray::fromHex(QString("A900").toLatin1()))) return false;
    } else {
        if (!this->Write_Bytes_To_Offset(0x1261,  QByteArray::fromHex(QString("EAA900").toLatin1()))) return false;
    }

    if (!this->Write_Bytes_To_Offset(0x04F6, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing
    if (!this->Write_Bytes_To_Offset(0x3C2B, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing
    if (this->wasCastleLoopReplacedWithFlagpole1UP && !this->Write_Bytes_To_Offset(0x4080, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing
    if (!this->Skip_Lives_Screen()) return false;
    this->permadeath = true;
    return true;
}

bool Hacks::Random_Group_Enemy_Goomba(bool allowHammerBros) {
    return this->Random_Enemy_Group(0x4737, allowHammerBros);
}

bool Hacks::Random_Group_Enemy_Koopa(bool allowHammerBros) {
    return this->Random_Enemy_Group(0x472C, allowHammerBros);
}

bool Hacks::Random_Intro_Demo() {
    if (this->wasVerticalObjectLimitRemoved) return this->Only_Jump_At_Intro_Demo(); //Intro Demo is not compatible with the vertical object limit patch
    QByteArray buttons(21, static_cast<char>(0x01)); //default is holding right
    bool run = Random::Get_Instance().Get_Num(1);
    bool stopAfterJump = false;
    bool left = false;
    int consecutiveLefts = 0;

    //Handle Button Presses
    for (int i = 1; i < buttons.size(); ++i) {
        if (Random::Get_Instance().Get_Num(8) == 0) run = !run; //possibly run

        //Possibly Stop After Jumping
        if (stopAfterJump) {
            if (Random::Get_Instance().Get_Num(4) == 0) stopAfterJump = false;
        } else {
            if (Random::Get_Instance().Get_Num(10) == 0) stopAfterJump = true;
        }

        //Possibly Move Left For a Short Distance
        if (consecutiveLefts > 5 || (left && Random::Get_Instance().Get_Num(2) == 0)) {
            left = false;
            consecutiveLefts = 0;
        } else if (i > 4 && Random::Get_Instance().Get_Num(15) == 0) {
            left = true;
        }

        bool jump = i%2 == 0;
        if (left) {
            ++consecutiveLefts;
            buttons.data()[i] = (buttons.data()[i]&static_cast<char>(0xFE))|static_cast<char>(0x02);
        }
        else if (jump) buttons.data()[i] = buttons.data()[i]|static_cast<char>(0x80); //jump every other action, but not if moving left
        if (run) buttons.data()[i] = buttons.data()[i]|static_cast<char>(0x40); //add the run button if run mode is enabled
        if (!jump && stopAfterJump) buttons.data()[i] = static_cast<char>(0x00);
    }
    buttons.data()[buttons.size()-1] = static_cast<char>(0x00); //do nothing for last action

    //Handle Timings
    QByteArray timings(22, static_cast<char>(0x20));
    timings.data()[0] = static_cast<char>(0x70); //walk past the title screen
    for (int i = 1; i < timings.size(); ++i) {
        if (i < buttons.size() && (buttons.at(i)&static_cast<char>(0x40)) == static_cast<char>(0x40)) timings.data()[i] = static_cast<char>(Random::Get_Instance().Get_Num(0x10, 0x1F)); //jump more often when running
        else timings.data()[i] = static_cast<char>(Random::Get_Instance().Get_Num(0x14, 0x2F)); //jump less often when walking
    }
    timings.data()[timings.size()-2] = static_cast<char>(0xFF); //max wait time for last action
    timings.data()[timings.size()-1] = static_cast<char>(0x00); //terminator byte

    if (!this->Write_Bytes_To_Offset(0x0350, buttons)) return false;
    return this->Write_Bytes_To_Offset(0x365, timings);
}

bool Hacks::Random_True_Bowser_Characters() {
    //Populate the Possible Enemies
    QByteArray bytes(8, static_cast<char>(0x2D));
    const int TOTAL_POSSIBLE_ENEMIES = 9;
    QVector<char> possibleEnemies(TOTAL_POSSIBLE_ENEMIES, static_cast<char>(0x00));
    for (int i = 0; i < TOTAL_POSSIBLE_ENEMIES; ++i) {
        switch (i) {
        default:     assert(false); return false;
        case 0:      possibleEnemies[i] = static_cast<char>(0x00); break; //Green Koopa (+1 for Red)
        case 1:      possibleEnemies[i] = static_cast<char>(0x02); break; //Buzzy Beetle
        case 2:      possibleEnemies[i] = static_cast<char>(0x05); break; //Hammer Bro
        case 3:      possibleEnemies[i] = static_cast<char>(0x06); break; //Goomba
        case 4:      possibleEnemies[i] = static_cast<char>(0x07); break; //Blooper
        case 5:      possibleEnemies[i] = static_cast<char>(0x08); break; //Bullet Bill
        case 6:      possibleEnemies[i] = static_cast<char>(0x0A); break; //Green Cheep-Cheep (+1 for Red)
        case 7:      possibleEnemies[i] = static_cast<char>(0x11); break; //Lakitu
        case 8:      possibleEnemies[i] = static_cast<char>(0x12); break; //Spiny
        }
    }

    //Populate the Enemies to be Written
    for (int i = 0; i < 8; ++i) {
        //Get a Unique Random Enemy
        int index = Random::Get_Instance().Get_Num(possibleEnemies.size()-1);
        char value = possibleEnemies.at(index);

        //Get Random Enemy Colors
        switch (value) {
        default:                    break;
        case 0x00: case 0x0A:       value += Random::Get_Instance().Get_Num(1); break;
        }

        //Save the Enemy
        bytes.data()[i] = value;
        possibleEnemies.removeAt(index);
    }
    bytes.data()[7] = static_cast<char>(0x2D); //make sure the final Bowser is the true Bowser
    if (!this->Write_Bytes_To_Offset(0x5787, QByteArray::fromHex(QString("C00F").toLatin1()))) return false; //force all enemies to be flipped (up to world 0xF)
    return this->Write_Bytes_To_Offset(0x5746, bytes);
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
    qDebug() << "Vertical Object Limit will be removed!";
    if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
        qDebug() << "Error: Vertical Object Limit cannot be removed in the Co-op ROM!";
        return false; //not compatible with Co-op
    }

    //by Chacky
    if (!this->Write_Bytes_To_Offset(0x1031, QByteArray::fromHex(QString("A20ADE8504CA10FAEA").toLatin1()))) return false;
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
    if (this->levelOffset->Get_ROM_Type() != ROM_Type::COOP_CGTI_1) { //this offset does not exist in Co-op
        if (!this->Write_Bytes_To_Offset(0x1883, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    }
    if (!this->Write_Bytes_To_Offset(0x18C8, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x190B, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1957, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1BC0, QByteArray::fromHex(QString("8504").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1BC7, QByteArray::fromHex(QString("85043860BC5704").toLatin1()))) return false;
    if (!this->Only_Jump_At_Intro_Demo()) return false; //this patch breaks the intro demo, so just disable it
    this->wasVerticalObjectLimitRemoved = true;
    return true;
}

bool Hacks::Replace_Castle_Loop_With_Autoscroll_Object() {
    //By ATA
    if (this->wasCastleLoopReplacedWithFireBros || this->wasCastleLoopReplacedWithFlagpole1UP || this->wasCastleLoopReplacedWithFireFlower) return false;
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
    if (this->wasCastleLoopReplacedWithAutoScrollObject || this->wasCastleLoopReplacedWithFlagpole1UP || this->wasCastleLoopReplacedWithFireFlower) return false;
    if (!this->Write_Bytes_To_Offset(0x3AA2, QByteArray::fromHex(QString("1CE4").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3AEB, QByteArray(1, static_cast<char>(0x38)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3B01, QByteArray::fromHex(QString("6BC0A90495AC2095C0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3B36, QByteArray(1, static_cast<char>(0x60)))) return false;
    if (!this->Write_Bytes_To_Offset(0x407B, QByteArray::fromHex(QString("8A18690DAAA01BA90020F0E3A608C900F00EB5AC300DA9FD95ACB5DB29F895DB4C28BBA900952A4C"
            "28BBA92085FFB91E0060").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5131, QByteArray(1, static_cast<char>(0x1F)))) return false; //Bowser Fireball throwing speed
    if (!this->Write_Bytes_To_Offset(0x64E8, QByteArray::fromHex(QString("0202C2C2").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x6870, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6881, QByteArray(2, static_cast<char>(0x02)))) return false; //red palette for Bowser
    if (!this->isHammerSuitActive) {
        if (!this->Write_Bytes_To_Offset(0x8810, QByteArray::fromHex(QString("000000003C7E77FB0000000000183C0E9F5F8E20000000000E040000000000000502080307070703"
                "0000000001030100C0E0F0F0B070E0C000004060E0C08000").toLatin1()))) return false;
    }
    if (!this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_FIRE_BROS)) return false;
    this->wasCastleLoopReplacedWithFireBros = true;
    return true;
}

bool Hacks::Replace_Castle_Loop_With_Start_With_Fire_Flower() {
    if (this->wasCastleLoopReplacedWithAutoScrollObject || this->wasCastleLoopReplacedWithFireBros) return false;
    if (!this->Write_Bytes_To_Offset(0x06D2, QByteArray::fromHex(QString("4C79C0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x4089, QByteArray::fromHex(QString("A9028D5607A9008D540720A4EF4CC586").toLatin1()))) return false;
    if (this->skipLivesScreen && !this->Write_Bytes_To_Offset(0x4093, QByteArray::fromHex(QString("4CE086").toLatin1()))) return false;
    this->wasCastleLoopReplacedWithFireFlower = true;
    return true;
}

bool Hacks::Replace_Castle_Loop_With_Top_Of_Flagpole_Gives_1UP() {
    if (this->wasCastleLoopReplacedWithAutoScrollObject || this->wasCastleLoopReplacedWithFireBros) return false;
    if (!this->Write_Bytes_To_Offset(0x407B, QByteArray::fromHex(QString("AC0F01D007EE5A07A94085FE60").toLatin1()))) return false;
    if (this->permadeath && !this->Write_Bytes_To_Offset(0x4080, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false; //prevent incrementing if permadeath is active
    if (!this->Write_Bytes_To_Offset(0x38A9, QByteArray::fromHex(QString("206BC0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x6551, QByteArray::fromHex(QString("FDFE").toLatin1()))) return false;
    this->wasCastleLoopReplacedWithFlagpole1UP = true;
    return true;
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
    if (speed < 1 || speed > 127) return false; //negative values are technically possible, but the enemies will get stuck in walls
    int easySpeed = 0x100-speed;
    int hardSpeed = easySpeed-4;
    int invertedHardSpeed = speed+4;
    if (!this->Write_Bytes_To_Offset(0x431C, QByteArray(1, static_cast<char>(easySpeed)))) return false; //basic enemy speed
    if (!this->Write_Bytes_To_Offset(0x431D, QByteArray(1, static_cast<char>(hardSpeed)))) return false; //basic enemy speed on hard mode
    if (!this->Write_Bytes_To_Offset(0x49E4, QByteArray(1, static_cast<char>(speed)))) return false; //right speed after revive
    if (!this->Write_Bytes_To_Offset(0x49E5, QByteArray(1, static_cast<char>(easySpeed)))) return false; //left speed after revive
    if (!this->Write_Bytes_To_Offset(0x49E6, QByteArray(1, static_cast<char>(invertedHardSpeed)))) return false; //right speed after revive
    if (!this->Write_Bytes_To_Offset(0x49E7, QByteArray(1, static_cast<char>(hardSpeed)))) return false; //left speed after revive
    if (!this->Write_Bytes_To_Offset(0x5861, QByteArray(1, static_cast<char>(speed)))) return false; //right speed after stomping paratroopa
    return this->Write_Bytes_To_Offset(0x5862, QByteArray(1, static_cast<char>(easySpeed))); //left speed after stomping paratroopa
}

bool Hacks::Set_Bowser_Bridge_Destruction_Speed(int speed) {
    if (speed < 1 || speed > 20) return false;
    return this->Write_Bytes_To_Offset(0x502B, QByteArray(1, static_cast<char>(speed)));
}

bool Hacks::Set_Bowser_Flame_Frequency(int frequency) {
    if (frequency < 1 || frequency > 3) return false;
    if (frequency == 2) return true; //frequency is set to Normal
    if (frequency == 1) { //less often
        if (!this->Write_Bytes_To_Offset(0x516A, QByteArray(1, static_cast<char>(0xBF)))) return false;
        if (!this->Write_Bytes_To_Offset(0x5182, QByteArray(1, static_cast<char>(0x80)))) return false;
        if (!this->Write_Bytes_To_Offset(0x51E1, QByteArray::fromHex(QString("BFBFBFBFBFBFBFBF").toLatin1()))) return false;
        return this->Write_Bytes_To_Offset(0x51F3, QByteArray(1, static_cast<char>(0x00)));
    } else {
        assert(frequency == 3); //more often
        if (!this->Write_Bytes_To_Offset(0x516A, QByteArray(1, static_cast<char>(0x10)))) return false;
        if (!this->Write_Bytes_To_Offset(0x5182, QByteArray(1, static_cast<char>(0x08)))) return false;
        return this->Write_Bytes_To_Offset(0x51E1, QByteArray::fromHex(QString("6020606060202060").toLatin1()));
    }
}

bool Hacks::Set_Brick_Break_Animation_Bounce_Height(int lowerHeight, int upperHeight) {
    if (lowerHeight < -127 || lowerHeight > 127) return false;
    if (upperHeight < -127 || upperHeight > 127) return false;
    int lowerValue = lowerHeight, upperValue = upperHeight;
    if (lowerHeight > 0) lowerValue = 0x100 - lowerHeight;
    if (upperHeight > 0) upperValue = 0x100 - upperHeight;
    if (lowerHeight < 0) lowerValue = std::abs(lowerHeight);
    if (upperHeight < 0) upperValue = std::abs(upperHeight);
    if (!this->Write_Bytes_To_Offset(0x3E61, QByteArray(1, static_cast<char>(lowerValue)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3E7C, QByteArray(1, static_cast<char>(upperValue)))) return false;
    return true;
}

bool Hacks::Set_Bullet_Bill_Firing_Rate(int rate) {
    if (rate < 1 || rate > 7) return false;
    if (rate == 7) {
        if (!this->Write_Bytes_To_Offset(0x39FF, QByteArray(1, static_cast<char>(0x01)))) return false;
        rate = 6; //the patch below glitches out when set to 7 or higher
    }
    return this->Write_Bytes_To_Offset(0x39D2, QByteArray(1, static_cast<char>(rate-1)));
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

bool Hacks::Set_Coin_Animation_Bounce_Height(int height) {
    if (height < 1 || height > 127) return false;
    return this->Write_Bytes_To_Offset(0x3B7D, QByteArray(1, static_cast<char>(0x100-height)));
}

bool Hacks::Set_Death_Animation_Jump_Height(int height) {
    if (height < 0 || height > 127) return false;
    int value = 0;
    if (height > 0) value = 0x100-height;
    return this->Write_Bytes_To_Offset(0x596E, QByteArray(1, static_cast<char>(value)));
}

bool Hacks::Set_Enemy_Group_Size(int value) {
    if (value < 1 || value > 5) return false; //default value is 2 for 2, 3 enemies
    return this->Write_Bytes_To_Offset(0x4757, QByteArray(1, static_cast<char>(value)));
}

bool Hacks::Set_Enemy_Revival_Speed(int speed) {
    if (speed < 2 || speed > 0xFF) return false;
    int hardTime = speed-5;
    if (hardTime < 2) hardTime = 2;
    if (!this->Write_Bytes_To_Offset(0x59E2, QByteArray(1, static_cast<char>(speed)))) return false;
    return this->Write_Bytes_To_Offset(0x59E3, QByteArray(1, static_cast<char>(hardTime)));
}

bool Hacks::Set_Firebar_Length(int length) {
    if (length < 1 || length > 6) return false;
    return this->Write_Bytes_To_Offset(0x4D9D, QByteArray(1, static_cast<char>(length-1)));
}

bool Hacks::Set_Flying_Cheep_Cheep_Jump_Height(int height) {
    if (height < 1 || height > 9) return false;
    return this->Write_Bytes_To_Offset(0x44E4, QByteArray(1, static_cast<char>(0x100-height)));
}

bool Hacks::Set_Long_Firebar_Length(int length) {
    if (length < 1 || length > 12) return false;
    return this->Write_Bytes_To_Offset(0x4DA5, QByteArray(1, static_cast<char>(length-1)));
}

bool Hacks::Set_Maximum_Number_Of_Pirahna_Plants(int value) {
    if (value < 1 || value > 6) return false;
    return this->Write_Bytes_To_Offset(0x1963, QByteArray(1, static_cast<char>(value)));
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
    if (!this->Write_Bytes_To_Offset(0x515D, QByteArray(1, static_cast<char>(value)))) return false; //world Bowser does not breathe fire (last world)
    if (!this->Write_Bytes_To_Offset(0x5161, QByteArray(1, static_cast<char>(startHardModeOnWorld)))) return false; //world Bowser starts breathing fire

    //Correct Bowser's "True" Form
    if (!this->Write_Bytes_To_Offset(0x5746+value, QByteArray(8-value, static_cast<char>(0x2D)))) return false;

    //Apply the Walking Hammer Bros patch
    assert(this->difficultyWalkingHammerBros >= 1 && this->difficultyWalkingHammerBros <= 11);
    if (this->difficultyWalkingHammerBros == 11) return true;
    int walkingHammerBrosWorld = 0;
    assert(this->Convert_Difficulty_To_World(this->difficultyWalkingHammerBros, numWorlds, walkingHammerBrosWorld));
    assert(walkingHammerBrosWorld >= 1 && walkingHammerBrosWorld <= 8);
    return this->Enable_Walking_Hammer_Bros_In_World(walkingHammerBrosWorld);
}

bool Hacks::Set_Number_Of_Levels_Per_World(int value) {
    bool moreThan4LevelsPerWorld = (value > 4);
    this->midpointWriter->Set_More_Than_4_Levels_Per_World(moreThan4LevelsPerWorld);
    if (moreThan4LevelsPerWorld) {
        if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
            if (!this->Write_Bytes_To_Offset(0x11F9, QByteArray::fromHex(QString("AD5C070AAAA5E4").toLatin1()))) return false;
            if (!this->Write_Bytes_To_Offset(0x1208, QByteArray::fromHex(QString("A5E4EA").toLatin1()))) return false;
        } else {
            if (!this->Write_Bytes_To_Offset(0x11FA, QByteArray(1, static_cast<char>(0x5C)))) return false;
            if (!this->Write_Bytes_To_Offset(0x11FF, QByteArray(1, static_cast<char>(0x5F)))) return false;
            if (!this->Write_Bytes_To_Offset(0x120A, QByteArray(1, static_cast<char>(0x5F)))) return false;
        }
    }
    return true;
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

bool Hacks::Set_Surfing_Lift_Speed(int speed) {
    if (speed < 0 || speed > 127) return false;
    return this->Write_Bytes_To_Offset(0x5658, QByteArray(1, static_cast<char>(speed)));
}

bool Hacks::Set_Hammer_Bros_Throw_Rate(int easyRate, int hardRate) {
    if (easyRate < 0 || easyRate > 0xFF || hardRate < 0 || hardRate > 0xFF) return false;
    if (!this->Write_Bytes_To_Offset(0x49DE, QByteArray(1, static_cast<char>(easyRate)))) return false;
    return this->Write_Bytes_To_Offset(0x49DF, QByteArray(1, static_cast<char>(hardRate)));
}

void Hacks::Set_Hammer_Suit_Active(bool isHammerSuitActive) {
    this->isHammerSuitActive = isHammerSuitActive;
}

bool Hacks::Speedy_Objects_And_Enemies() {
    //by L
    if (!this->Write_Bytes_To_Offset(0x36D4, QByteArray(1, static_cast<char>(0x60)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3AA2, QByteArray::fromHex(QString("20E0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3AEB, QByteArray(1, static_cast<char>(0x20)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3CD4, QByteArray(1, static_cast<char>(0x20)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3F89, QByteArray(1, static_cast<char>(0x06)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3F8D, QByteArray(1, static_cast<char>(0x0C)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3F91, QByteArray(1, static_cast<char>(0x06)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3F9D, QByteArray::fromHex(QString("14A906").toLatin1()))) return false;
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
    if (!this->Write_Bytes_To_Offset(0x585F, QByteArray::fromHex(QString("40C018E8").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5FCF, QByteArray::fromHex(QString("20E0").toLatin1()))) return false;
    return this->Increase_Spiny_Egg_Speed(0x09);
}

bool Hacks::Spiny_Eggs_Do_Not_Break() {
    if (!this->Increase_Spiny_Egg_Speed(0x07)) return false;

    //Read the egg frames
    QByteArray eggFrame1, eggFrame2;
    if (!this->Read_Bytes_From_Offset(0x677E, 6, eggFrame1)) return false;
    if (!this->Read_Bytes_From_Offset(0x6784, 6, eggFrame2)) return false;
    if (static_cast<unsigned char>(eggFrame1.at(0)) != static_cast<unsigned char>(0xFC) ||
        static_cast<unsigned char>(eggFrame1.at(1)) != static_cast<unsigned char>(0xFC) ||
        static_cast<unsigned char>(eggFrame2.at(0)) != static_cast<unsigned char>(0xFC) ||
        static_cast<unsigned char>(eggFrame2.at(1)) != static_cast<unsigned char>(0xFC)) {
        return false; //the top two tiles are assumed to be the blank tiles. Fail if they are not.
    }

    //Read the spiny frames
    QByteArray spinyFrame1, spinyFrame2;
    if (!this->Read_Bytes_From_Offset(0x6772, 6, spinyFrame1)) return false;
    if (!this->Read_Bytes_From_Offset(0x6778, 6, spinyFrame2)) return false;
    if (static_cast<unsigned char>(spinyFrame1.at(0)) != static_cast<unsigned char>(0xFC) ||
        static_cast<unsigned char>(spinyFrame1.at(1)) != static_cast<unsigned char>(0xFC) ||
        static_cast<unsigned char>(spinyFrame2.at(0)) != static_cast<unsigned char>(0xFC) ||
        static_cast<unsigned char>(spinyFrame2.at(1)) != static_cast<unsigned char>(0xFC)) {
        return false; //the top two tiles are assumed to be the blank tiles. Fail if they are not.
    }

    //Read the Left Side from the Spiny Egg Graphics Data
    QByteArray eggFrame1TopLeft, eggFrame1BottomLeft, eggFrame2TopLeft, eggFrame2BottomLeft;
    if (!this->graphics->Read_Graphics_Bytes_From_Sprite_Tile_ID(eggFrame1.at(4), eggFrame1TopLeft)) return false;
    if (!this->graphics->Read_Graphics_Bytes_From_Sprite_Tile_ID(eggFrame1.at(5), eggFrame1BottomLeft)) return false;
    if (!this->graphics->Read_Graphics_Bytes_From_Sprite_Tile_ID(eggFrame2.at(4), eggFrame2TopLeft)) return false;
    if (!this->graphics->Read_Graphics_Bytes_From_Sprite_Tile_ID(eggFrame2.at(5), eggFrame2BottomLeft)) return false;

    //Get the Right Side
    QByteArray eggFrame1TopRight = eggFrame1BottomLeft, eggFrame1BottomRight = eggFrame1TopLeft, eggFrame2TopRight = eggFrame2BottomLeft, eggFrame2BottomRight = eggFrame2TopLeft;
    if (!this->graphics->Perform_Vertical_Flip(eggFrame1TopRight)) return false;
    if (!this->graphics->Perform_Horizontal_Flip(eggFrame1TopRight)) return false;
    if (!this->graphics->Perform_Vertical_Flip(eggFrame1BottomRight)) return false;
    if (!this->graphics->Perform_Horizontal_Flip(eggFrame1BottomRight)) return false;
    if (!this->graphics->Perform_Vertical_Flip(eggFrame2TopRight)) return false;
    if (!this->graphics->Perform_Horizontal_Flip(eggFrame2TopRight)) return false;
    if (!this->graphics->Perform_Vertical_Flip(eggFrame2BottomRight)) return false;
    if (!this->graphics->Perform_Horizontal_Flip(eggFrame2BottomRight)) return false;

    //Write the Egg Graphics Data over the Spiny Graphics Data
    if (!this->graphics->Write_Graphics_Bytes_To_Sprite_Tile_ID(spinyFrame1.at(2), eggFrame1TopLeft)) return false;
    if (!this->graphics->Write_Graphics_Bytes_To_Sprite_Tile_ID(spinyFrame1.at(3), eggFrame1TopRight)) return false;
    if (!this->graphics->Write_Graphics_Bytes_To_Sprite_Tile_ID(spinyFrame1.at(4), eggFrame1BottomLeft)) return false;
    if (!this->graphics->Write_Graphics_Bytes_To_Sprite_Tile_ID(spinyFrame1.at(5), eggFrame1BottomRight)) return false;
    if (!this->graphics->Write_Graphics_Bytes_To_Sprite_Tile_ID(spinyFrame2.at(2), eggFrame2TopLeft)) return false;
    if (!this->graphics->Write_Graphics_Bytes_To_Sprite_Tile_ID(spinyFrame2.at(3), eggFrame2TopRight)) return false;
    if (!this->graphics->Write_Graphics_Bytes_To_Sprite_Tile_ID(spinyFrame2.at(4), eggFrame2BottomLeft)) return false;
    if (!this->graphics->Write_Graphics_Bytes_To_Sprite_Tile_ID(spinyFrame2.at(5), eggFrame2BottomRight)) return false;
    return true;
}

bool Hacks::Spiny_Eggs_Bouncy() {
    if (!this->Write_Bytes_To_Offset(0x4101, QByteArray::fromHex(QString("D0034CE2E0B416C012D0034CD9DF20AEE1F0EF4CFFDF").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x6005, QByteArray::fromHex(QString("4CF1C0").toLatin1()));
}

bool Hacks::Spiny_Eggs_Chase_Mario() {
    if (!this->Write_Bytes_To_Offset(0x4108, QByteArray::fromHex(QString("B516C912D005A905951E60A900951E60").toLatin1()))) return false; //only affect Spinies
    if (!this->Write_Bytes_To_Offset(0x60E6, QByteArray::fromHex(QString("4CF8C060").toLatin1()))) return false; //chase Mario

    //Make Spiny Eggs transparent
    QByteArray bytes;
    assert(this->graphics);
    if (!this->Read_Bytes_From_Offset(0x677E, 12, bytes)) return false;
    if (!this->graphics->Make_Sprite_Tiles_Transparent(bytes)) return false;
    this->spinyEggSpeedCap = 0x13;

    return this->Spiny_Eggs_Do_Not_Break();
}

bool Hacks::Spiny_Eggs_Explode_Into_Flames() {
    //Hitting the block under flames destroys them
    if (!this->Write_Bytes_To_Offset(0x4109, QByteArray::fromHex(QString("C912D003208EE1A9012011DA4C1BE0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x6026, QByteArray::fromHex(QString("4CF9C0EAEA").toLatin1()))) return false;

    //Fireballs can't hurt fire
    assert(this->powerups);
    if (this->powerups->Is_Primary_Powerup_Fire_Based()) {
        if (!this->Write_Bytes_To_Offset(0x40FD, QByteArray::fromHex(QString("C902F007C912F0034C58D760").toLatin1()))) return false;
        if (!this->Write_Bytes_To_Offset(0x5764, QByteArray::fromHex(QString("4CEDC0EA").toLatin1()))) return false;
    }

    //Flames can't move left or right
    this->spinyEggSpeedCap = 0;
    if (!this->Write_Bytes_To_Offset(0x60C4, QByteArray(1, static_cast<char>(0x00)))) return false;
    if (!this->Write_Bytes_To_Offset(0x687D, QByteArray(1, static_cast<char>(0x02)))) return false; //force color group to be red

    //Flame Sprites
    if (!this->Write_Bytes_To_Offset(0x88F0, QByteArray::fromHex(QString("00000000020B070E00000000000001030E070B02000000000301000000000000").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8950, QByteArray::fromHex(QString("000000000503170E00000000000001030E17030500000000030100000000000014292B0F6F7F7F7F"
            "100000004204050584A0E4E8DAFEFDFD0000040000200000BFF6FA7C7C3F07071F1F1F0F0F030000"
            "BFFFBE1E3CFCF0E0E0F0F8F8F0E00000210527175B7FBFBF00002000000400002894D4F0F6FEFEFE"
            "080000004220A0A0FDFF7D783C3F0F07070F1F1F0F070000FD6F5F3E3EFCE0E0F8F8F8F0F0C00000").toLatin1()))) return false;
    return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_SPINY_EGGS_EXPLODE_INTO_FLAMES);
}

bool Hacks::Spiny_Eggs_No_Eggs() {
    //Copy the Spiny tile layout over the Spiny Egg tile layout
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x6772, 12, bytes)) return false;
    if (!this->Write_Bytes_To_Offset(0x677E, bytes)) return false;

    //Disable tile mirroring for spiny eggs
    if (!this->Write_Bytes_To_Offset(0x6AD7, QByteArray(1, static_cast<char>(0xFF)))) return false;
    return this->Write_Bytes_To_Offset(0x6B00, QByteArray::fromHex(QString("EAEA").toLatin1()));
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
    if (!this->Write_Bytes_To_Offset(0x433A, QByteArray::fromHex(QString("9558AD5F07C90"+QString::number(world)+"B005A980").toLatin1()))) return false;
    if (this->wasCastleLoopReplacedWithFireBros) {
        //Make walking hammer bros wait a little bit before moving to avoid the pipe exit bug
        if (!this->Write_Bytes_To_Offset(0x40AD, QByteArray::fromHex(QString("B002A9809002A9044C35C3").toLatin1()))) return false; //change the 04 in "A9044C" to the desired time
        if (!this->Write_Bytes_To_Offset(0x4341, QByteArray::fromHex(QString("4C9DC0").toLatin1()))) return false;
    }
    return true;
}

bool Hacks::Increase_Spiny_Egg_Speed(int amount) {
    if (amount < 0 || amount > 127) return false;
    assert(this->spinyEggSpeedCap >= 0x00 && this->spinyEggSpeedCap <= 0xFF);

    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x60C4, 1, bytes)) return false;
    bytes.data()[0] += static_cast<char>(amount);
    if (bytes.data()[0] > this->spinyEggSpeedCap) bytes.data()[0] = static_cast<char>(this->spinyEggSpeedCap);
    return this->Write_Bytes_To_Offset(0x60C4, bytes);
}

bool Hacks::Skip_Lives_Screen() {
    if (this->wasCastleLoopReplacedWithFireFlower) {
        if (!this->Write_Bytes_To_Offset(0x4093, QByteArray::fromHex(QString("4CE086").toLatin1()))) return false;
    } else {
        //if (!this->Write_Bytes_To_Offset(0x06D2, QByteArray::fromHex(QString("F01CEA").toLatin1()))) return false;
        if (!this->Write_Bytes_To_Offset(0x06D2, QByteArray::fromHex(QString("F01CEA").toLatin1()))) return false;
    }
    this->skipLivesScreen = true;
    return true;
}

bool Hacks::Random_Enemy_Group(qint64 offset, bool allowHammerBros) {
    int byte = 0;
    int maxValue = 8;
    if (allowHammerBros) ++maxValue;
    switch (Random::Get_Instance().Get_Num(maxValue)) {
    case 0:     byte = 0x00; break; //Green Koopa
    case 1:     byte = 0x01; break; //Red Koopa
    case 2:     byte = 0x02; break; //Buzzy Beetle
    case 3:     byte = 0x06; break; //Goomba
    case 4:     byte = 0x07; break; //Blooper
    case 5:     byte = 0x08; break; //Bullet Bill
    case 6:     byte = 0x0C; break; //Podoboo
    case 7:     byte = 0x0E; break; //Green Paratroopa (Leaping)
    case 8:     byte = 0x10; break; //Green Paratroopa (L/R)
    case 9:     byte = 0x05; break; //Hammer Bros
    }
    return this->Write_Bytes_To_Offset(offset, QByteArray(1, static_cast<char>(byte)));
}
