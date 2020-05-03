#include "Powerups.h"
#include "Level_Offset.h"
#include "Graphics.h"
#include "Hacks.h"
#include "Sequential_Archive_Handler.h"

Powerups::Powerups(QFile *f, Level_Offset *lo, Sequential_Archive_Handler *sequentialArchiveHandler, Graphics *graphics, Hacks *hacks) : Byte_Writer(f, lo) {
    assert(sequentialArchiveHandler); assert(graphics); assert(hacks);
    this->graphics = graphics;
    this->hacks = hacks;
    this->sequentialArchiveHandler = sequentialArchiveHandler;
    this->primaryPowerupIsFireBased = true;
}

bool Powerups::Is_Primary_Powerup_Fire_Based() {
    return this->primaryPowerupIsFireBased;
}

bool Powerups::Replace_1UP_With_Poison_Mushroom() {
    //by YY
    if (!this->Write_Bytes_To_Offset(0x5813, QByteArray::fromHex(QString("A539C903F033A9062011DAA92085FEA539C902900AA9238D9F07A94085FB60AD5607F019C901D021"
            "A608A9028D560720F185A608A90C4C47D84CF2D7").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x66E1, QByteArray(1, static_cast<char>(0x03)));
}

bool Powerups::Replace_1UP_With_Swimming_Mushroom() {
    if (!this->Write_Bytes_To_Offset(0xD7A, QByteArray::fromHex(QString("AD0407F006CE04074CED90EE04074C1BD8").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x584A, QByteArray::fromHex(QString("4C6A8D").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x66E1, QByteArray(1, static_cast<char>(0x03)));
}

bool Powerups::Replace_Fire_Flower_With_Bone_Caster() {
    if (!this->Write_Bytes_To_Offset(0x05F2, QByteArray(1, static_cast<char>(0x2D)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("30D0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x36C4, QByteArray(1, static_cast<char>(0xFC)))) return false;
    if (!this->Write_Bytes_To_Offset(0x36D8, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3CAE, QByteArray::fromHex(QString("01F009").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61F1, QByteArray(1, static_cast<char>(0x00)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6720, QByteArray::fromHex(QString("C901F0178500A5094A29030DCA03990202990602EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("0000020608102020000C0E1E38F0E0600000C02010080C0060E0F0381E0E0C000000000000000000"
            "0143333B1F1F7FFF0103133B1F1F7FFF0103133B1F1F7FFF0001030B1F0F3F7F0000000000000000").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("0000000000000000040E1F1F0F070303").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("00000000000000000303070F1F1F0E04").toLatin1()))) return false;
    if (!this->graphics->Apply_Bone_Caster_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Coin_Flinger() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("13270A").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3660, QByteArray::fromHex(QString("0185FE").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("18E8").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x36C4, QByteArray(1, static_cast<char>(0xFA)))) return false;
    if (!this->Write_Bytes_To_Offset(0x36D8, QByteArray(1, static_cast<char>(0x06)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3CAE, QByteArray::fromHex(QString("01F009").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5735, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61F1, QByteArray(1, static_cast<char>(0xFE)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6720, QByteArray::fromHex(QString("C901F0178500A5094A29030DCA03990202990602EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8631, QByteArray::fromHex(QString("000000000000000000000000000000").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("3E675F5F5F5F7F3E3C7E7A7A7A7A663C080808000008080800080808080808000001010101010100"
            "00010101010101010001010303030301000101020202020200010307060D0D0D00010307070F0F0F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("070F1F1F3F3F3F3F000007070E0D0D0D").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("3F3F3F3F1F1F0F070D0D0D0E07070000").toLatin1()))) return false;
    if (!this->graphics->Apply_Coin_Flinger_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Cutter_Flower() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("16270D").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3671, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("50B0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61E2, QByteArray(1, static_cast<char>(0x0D)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61F1, QByteArray(1, static_cast<char>(0xFF)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61FA, QByteArray::fromHex(QString("090F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("1C06038383C77E3C000C06064E7E3C003860C0C1C1E37E3C00306060727E3C00070F1F3C78F0E0E0"
            "0003070F1F3C78780001070F1C38307000000007071F1C1C0000000003070E0C").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x869D, QByteArray::fromHex(QString("010307").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("0502018383C57E3D1D0F07064E7F3B01").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("C1B1596D353B1F030040201008040000").toLatin1()))) return false;
    if (!this->graphics->Apply_Cutter_Flower_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Double_Jump_Star() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("142702").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x1151, QByteArray::fromHex(QString("2046B6EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x2F0F, QByteArray(1, static_cast<char>(0x6E)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3191, QByteArray::fromHex(QString("2024B6").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x349F, QByteArray::fromHex(QString("202AB6").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3634, QByteArray::fromHex(QString("8E2307862960A529D014E628AD5607C902A5289005C9024C40B6C9019003AD040760A900851D852860").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3CAD, QByteArray::fromHex(QString("EAEAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5C00, QByteArray::fromHex(QString("2046B6EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5D6A, QByteArray::fromHex(QString("2046B6EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x03)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6720, QByteArray::fromHex(QString("C901F0178500A5094A29030DCA03990202990602EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("01010202047842220000010103073D1D").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("12080810111628300D07070F0E081000").toLatin1()))) return false;
    if (!this->graphics->Apply_Double_Jump_Star_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Fire_Star() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray(1, static_cast<char>(0x05)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3643, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x365F, QByteArray::fromHex(QString("EAEAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("639D").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x36C4, QByteArray(1, static_cast<char>(0x06)))) return false;
    if (!this->Write_Bytes_To_Offset(0x36D8, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3CAD, QByteArray::fromHex(QString("EAEAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61F1, QByteArray(1, static_cast<char>(0x05)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6208, QByteArray::fromHex(QString("EAEAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x6410, QByteArray(1, static_cast<char>(0x13)))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6720, QByteArray::fromHex(QString("C901F0178500A5094A29030DCA03990202990602EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("01010303077F7D3D0101020204784222").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("1D0F0F1F1F1E38301208081011162830").toLatin1()))) return false;
    if (!this->graphics->Apply_Fire_Star_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(true)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Hammer_Suit() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("30270F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("27D8").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x36C4, QByteArray::fromHex(QString("FB95A6A9079DA004D6248A18"
        "6907AAA9508500A9468502A90020D7BF200FBFA608203BF12087F1202DE220F1E1ADD20329CCD00620D9D64CC8E1A900952460203BF14C61").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3CAD, QByteArray(4, static_cast<char>(0xEA)))) return false;
    if (!this->Write_Bytes_To_Offset(0x5735, QByteArray(2, static_cast<char>(0xEA)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61D8, QByteArray::fromHex(QString("BCF106ADBA03990002ADAF03"
        "990302A5094A4A4829010A4980990102684A4AA903900209C099020260A900953A60FFFFFFFFFFFFFF").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x03)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6720, QByteArray::fromHex(QString("C901F0178500A5094A29030DCA03990202990602EA").toLatin1()))) return false; //remove the flashing palette
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("071F1F3F3F7A72300000000F18252D0F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("1827273F1D1F0F0707181B0002000600").toLatin1()))) return false;
    if (!this->graphics->Apply_Hammer_Suit_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    this->hacks->Set_Hammer_Suit_Active(true);
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Luigis_Fire_Flower() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("302719").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("32CE").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61E2, QByteArray(1, static_cast<char>(0x0D)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61F1, QByteArray(1, static_cast<char>(0xFF)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61FA, QByteArray::fromHex(QString("090F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x6D0D, QByteArray(1, static_cast<char>(0x01)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6D5D, QByteArray::fromHex(QString("01990202A981990602A941990A02A9C1").toLatin1()))) return false;
    if (!this->graphics->Apply_Luigis_Fire_Flower_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Force_Green_Palettes_To_Be_Green()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(true)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Pink_Fury() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("242706").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3660, QByteArray(1, static_cast<char>(0x08)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3671, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("2DD3").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3CAE, QByteArray::fromHex(QString("01F009").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61EA, QByteArray::fromHex(QString("EAEAB53AEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6720, QByteArray::fromHex(QString("C901F0178500A5094A29030DCA03990202990602EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("080C0E0F4F3E1C08183C7EFF3C08000002466E7E7E7E7E007E3E1E0E160240000062502000044000"
            "006270200004400000003A2C3014200100003A3C3A14280100000000000306040000000000030707").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("000000000102050B0000000000010306").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("162D2D160B0502010D1B1B0D06030100").toLatin1()))) return false;
    if (!this->graphics->Apply_Pink_Fury_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Poison_Bubbles() {
    if (!this->Write_Bytes_To_Offset(0x05F2, QByteArray(1, static_cast<char>(0x12)))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("20E0").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x36C4, QByteArray(1, static_cast<char>(0x01)))) return false;
    if (!this->Write_Bytes_To_Offset(0x36D8, QByteArray(1, static_cast<char>(0x08)))) return false;
    if (!this->Write_Bytes_To_Offset(0x5735, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61EA, QByteArray::fromHex(QString("EAEAB53AEAEAA9F8").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x03)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6D0D, QByteArray(1, static_cast<char>(0x03)))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("00180A466E1E3C003C5ABBF5E3956A3C0000082C2C1C0000003C4A7E6A563C").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("0003070C0C0C07030000000000000000").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("01010101010101010000000000000000").toLatin1()))) return false;
    if (!this->graphics->Apply_Poison_Bubbles_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Power_Wand() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("0F2703").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3660, QByteArray::fromHex(QString("0885FE").toLatin1()))) return false; //Blast sound
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("7F80").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61E2, QByteArray(1, static_cast<char>(0x15)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61FB, QByteArray(1, static_cast<char>(0xFC)))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6D0D, QByteArray(1, static_cast<char>(0x03)))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("0000183C3C1800000000001818000000000018242418000000000018180000000000000000000000"
            "021004520428920000000000000000000000020812002A0000000000000000000000000001040208").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("000105060B0D06030002030D060B0502").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("01010101010101010101010101010101").toLatin1()))) return false;
    if (!this->graphics->Apply_Power_Wand_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Pyro_Spark_Wand() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("273716").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3660, QByteArray::fromHex(QString("0885FE").toLatin1()))) return false; //Blast sound
    if (!this->Write_Bytes_To_Offset(0x3671, QByteArray::fromHex(QString("EAEAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("37C9").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5735, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61F1, QByteArray(1, static_cast<char>(0x00)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61EA, QByteArray::fromHex(QString("EAEAB53AEAEAA900").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x03)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6D0D, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("0092386CC66C3892000010387C38100000004410381044000000000010000000").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("03070F0E0E0F070302030D07070D0302").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("33617F3F010101010000000000000000").toLatin1()))) return false;
    if (!this->graphics->Apply_Pyro_Spark_Wand_Fix()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(true)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Slime_Flower() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("19270F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3697, QByteArray::fromHex(QString("18E8").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x5735, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61EA, QByteArray::fromHex(QString("EAEAB53AEAEAA900").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x6D0D, QByteArray(1, static_cast<char>(0x01)))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("3C7EE3C1E1F37E3C00001C3E1E0C00003C7EF7E3C3C3663C0000081C3C3C1800").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("00000F1F1F0F00000F1F3F38383F1F0F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("71FDC9C1F17D1F070000000000000000").toLatin1()))) return false;
    if (!this->graphics->Apply_Slime_Flower_Fix()) return false;
    if (!this->Update_Fire_Mario_Skin_Color()) return false;
    if (!this->Force_Green_Palettes_To_Be_Green()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Snowballs() {
    //Snowballs originally by Darkdata-Systemlogoff
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("113101").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CD1, QByteArray(1, static_cast<char>(0x30)))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CF5, QByteArray(1, static_cast<char>(0x30)))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D19, QByteArray(1, static_cast<char>(0x30)))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D3D, QByteArray(1, static_cast<char>(0x30)))) return false;
    if (!this->Write_Bytes_To_Offset(0x36D8, QByteArray(1, static_cast<char>(0x08)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61E2, QByteArray(1, static_cast<char>(0x15)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61EB, QByteArray(1, static_cast<char>(0xFF)))) return false;
    if (!this->Write_Bytes_To_Offset(0x61F1, QByteArray(1, static_cast<char>(0x00)))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x02)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6720, QByteArray::fromHex(QString("C901F0178500A5094A29030DCA03990202990602EA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("00000000000000003C7EFFFFFFFF7E3C00000000000000003C7EFFFFFFFF7E3C").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8670, QByteArray::fromHex(QString("000000000000000000000000020B070F000000000000000000000000020B070F000000000000000000000000020B070F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("000000000000000003070F0F0F0F0703").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("00000000000000003C7EFFFFFFFF7E3C").toLatin1()))) return false;
    if (!this->graphics->Apply_Snowballs_Fix()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false;
    return true;
}

bool Powerups::Replace_Fire_Flower_With_Spinball_Flower() {
    if (!this->Write_Bytes_To_Offset(0x05F0, QByteArray::fromHex(QString("2D3D0F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x3671, QByteArray(3, static_cast<char>(0xEA)))) return false;
    if (!this->Write_Bytes_To_Offset(0x36C4, QByteArray(1, static_cast<char>(0x05)))) return false;
    if (!this->Write_Bytes_To_Offset(0x36D6, QByteArray(1, static_cast<char>(0x07)))) return false;
    if (!this->Write_Bytes_To_Offset(0x5735, QByteArray::fromHex(QString("EAEA").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x61EA, QByteArray::fromHex(QString("EAEAB53AEAEAA9F9").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x66DF, QByteArray(1, static_cast<char>(0x03)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6D0D, QByteArray(1, static_cast<char>(0x03)))) return false;
    if (!this->Write_Bytes_To_Offset(0x8650, QByteArray::fromHex(QString("00183C7E7E3C1800000018080000000000183C7E7E3C18000000003020000000").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8D70, QByteArray::fromHex(QString("0000000000000000000F1F38383F1F0F").toLatin1()))) return false;
    if (!this->Write_Bytes_To_Offset(0x8DA0, QByteArray::fromHex(QString("010131393D1F0F030000000000000000").toLatin1()))) return false;
    if (!this->graphics->Apply_Spinball_Flower_Fix()) return false;
    if (!this->Set_Primary_Powerup_Is_Fire_Based(false)) return false; //don't update the skin color for the spinball powerup
    return true;
}

bool Powerups::Update_Fire_Mario_Skin_Color() {
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x05E9, 1, bytes)) return false;
    return this->Write_Bytes_To_Offset(0x05F1, bytes);
}

bool Powerups::Force_Green_Palettes_To_Be_Green() {
    if (!this->Write_Bytes_To_Offset(0xCCC, QByteArray(1, static_cast<char>(0x1A)))) return false;
    if (!this->Write_Bytes_To_Offset(0xCF0, QByteArray(1, static_cast<char>(0x1A)))) return false;
    if (!this->Write_Bytes_To_Offset(0xD14, QByteArray(1, static_cast<char>(0x1A)))) return false;
    return this->Write_Bytes_To_Offset(0xD38, QByteArray(1, static_cast<char>(0x1A)));
}

bool Powerups::Set_Primary_Powerup_Is_Fire_Based(bool isFireBased) {
    this->primaryPowerupIsFireBased = isFireBased;
    if (this->primaryPowerupIsFireBased) return true; //nothing else to do

    //Make Fire Immune Enemies Killable
    if (!this->Write_Bytes_To_Offset(0x5764, QByteArray(4, static_cast<char>(0xEA)))) return false;
    return this->Write_Bytes_To_Offset(0x579A, QByteArray::fromHex(QString("33F00AEAEAEAEA").toLatin1()));
}
