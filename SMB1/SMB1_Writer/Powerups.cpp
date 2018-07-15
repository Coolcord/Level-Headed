#include "Powerups.h"
#include "Level_Offset.h"
#include "Graphics.h"
#include "Sequential_Archive_Handler.h"

Powerups::Powerups(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler, Graphics *graphics) : Byte_Writer(file, levelOffset) {
    assert(sequentialArchiveHandler); assert(graphics);
    this->graphics = graphics;
    this->sequentialArchiveHandler = sequentialArchiveHandler;
}

bool Powerups::Replace_1UP_With_Poison_Mushroom() {
    //by YY
    if (!this->Write_Bytes_To_Offset(0x5813, QByteArray::fromHex(QString("A539C903F033A9062011DAA92085FEA539C902900AA9238D9F07A94085FB60AD5607F019C901D021"
            "A608A9028D560720F185A608A90C4C47D84CF2D7").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x66E1, QByteArray(1, 0x03));
}

bool Powerups::Replace_1UP_With_Poison_Mushroom_If_Not_Fire_Mario() {
    //by MHS
    if (!this->Write_Bytes_To_Offset(0x2254, QByteArray::fromHex(QString("CF7B").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x2280, QByteArray::fromHex(QString("CF7B").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x239F, QByteArray::fromHex(QString("7F7B5E8E7F7B").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x23AD, QByteArray::fromHex(QString("BF7B").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x23CF, QByteArray::fromHex(QString("DF7B").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x23DF, QByteArray::fromHex(QString("DF7BCE86DF7B").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x241E, QByteArray::fromHex(QString("4F7B").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x2436, QByteArray::fromHex(QString("EF7B").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x245A, QByteArray::fromHex(QString("3FFB").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x407B, QByteArray::fromHex(QString("AD5607F004C901D0034CF2D7A9062011DAA92085FEA90B9D100160").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5813, QByteArray::fromHex(QString("A539C903F033A9062011DAA92085FEA539C902900AA9238D9F07A94085FB60AD5607F019C901D021"
            "A608A9028D560720F185A608A90C4C47D84C6BC0").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x66E1, QByteArray(1, 0x03));
}

bool Powerups::Replace_1UP_With_Swimming_Mushroom() {
    if (!this->Write_Bytes_To_Offset(0xD7A, QByteArray::fromHex(QString("AD0407F006CE04074CED90EE04074C1BD8").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x584A, QByteArray::fromHex(QString("4C6A8D").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x66E1, QByteArray(1, 0x00));
}

bool Powerups::Replace_Fire_Flower_With_Hammer_Suit() {
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
    return this->graphics->Apply_Hammer_Suit_Fix();
}
