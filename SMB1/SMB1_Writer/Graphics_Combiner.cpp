#include "Graphics_Combiner.h"

Graphics_Combiner::Graphics_Combiner() {
    this->marioPatchName = QString();
    this->brickPatchName = QString();
    this->tileOrderMap = nullptr;
    this->Initialize_Tile_Order_Map();
}

Graphics_Combiner::~Graphics_Combiner() {
    this->Deallocate_Tile_Order_Map();
}

bool Graphics_Combiner::Combine_All_Except_Mario() {

}

bool Graphics_Combiner::Combine_Mario() {

}

QString Graphics_Combiner::Get_Brick_Patch_Name() {
    return this->brickPatchName;
}

QString Graphics_Combiner::Get_Mario_Patch_Name() {
    return this->marioPatchName;
}

void Graphics_Combiner::Initialize_Tile_Order_Map() {
    this->Deallocate_Tile_Order_Map();
    this->tileOrderMap = new QMap<qint64, QByteArray*>();
    this->tileOrderMap->insert(0xB4C, new QByteArray(QByteArray::fromHex(QString("24C024C0").toLatin1())));
    this->tileOrderMap->insert(0xB50, new QByteArray(QByteArray::fromHex(QString("247F7F24").toLatin1())));
    this->tileOrderMap->insert(0xB60, new QByteArray(QByteArray::fromHex(QString("60646165").toLatin1())));
    this->tileOrderMap->insert(0xB64, new QByteArray(QByteArray::fromHex(QString("62666367").toLatin1())));
    this->tileOrderMap->insert(0xB68, new QByteArray(QByteArray::fromHex(QString("60646165").toLatin1())));
    this->tileOrderMap->insert(0xB6C, new QByteArray(QByteArray::fromHex(QString("62666367").toLatin1())));
    this->tileOrderMap->insert(0xB70, new QByteArray(QByteArray::fromHex(QString("68686969").toLatin1())));
    this->tileOrderMap->insert(0xB74, new QByteArray(QByteArray::fromHex(QString("26266A6A").toLatin1())));
    this->tileOrderMap->insert(0xB78, new QByteArray(QByteArray::fromHex(QString("4B4C4D4E").toLatin1())));
    this->tileOrderMap->insert(0xB7C, new QByteArray(QByteArray::fromHex(QString("4D4F4D4F").toLatin1())));
    this->tileOrderMap->insert(0xB80, new QByteArray(QByteArray::fromHex(QString("4D4E5051").toLatin1())));
    this->tileOrderMap->insert(0xB84, new QByteArray(QByteArray::fromHex(QString("6B702C2D").toLatin1())));
    this->tileOrderMap->insert(0xB88, new QByteArray(QByteArray::fromHex(QString("6C716D72").toLatin1())));
    this->tileOrderMap->insert(0xB8C, new QByteArray(QByteArray::fromHex(QString("6E736F74").toLatin1())));
    this->tileOrderMap->insert(0xB90, new QByteArray(QByteArray::fromHex(QString("868A878B").toLatin1())));
    this->tileOrderMap->insert(0xB94, new QByteArray(QByteArray::fromHex(QString("888C888C").toLatin1())));
    this->tileOrderMap->insert(0xB98, new QByteArray(QByteArray::fromHex(QString("898D6969").toLatin1())));
    this->tileOrderMap->insert(0xB9C, new QByteArray(QByteArray::fromHex(QString("8E918F92").toLatin1())));
    this->tileOrderMap->insert(0xBA0, new QByteArray(QByteArray::fromHex(QString("26932693").toLatin1())));
    this->tileOrderMap->insert(0xBA4, new QByteArray(QByteArray::fromHex(QString("90946969").toLatin1())));
    this->tileOrderMap->insert(0xBA8, new QByteArray(QByteArray::fromHex(QString("A4E9EAEB").toLatin1())));
    this->tileOrderMap->insert(0xBB0, new QByteArray(QByteArray::fromHex(QString("242F243D").toLatin1())));
    this->tileOrderMap->insert(0xBB4, new QByteArray(QByteArray::fromHex(QString("A2A2A3A3").toLatin1())));
    this->tileOrderMap->insert(0xBBC, new QByteArray(QByteArray::fromHex(QString("A2A2A3A3").toLatin1())));
    this->tileOrderMap->insert(0xBC0, new QByteArray(QByteArray::fromHex(QString("99249924").toLatin1())));
    this->tileOrderMap->insert(0xBC4, new QByteArray(QByteArray::fromHex(QString("24A23E3F").toLatin1())));
    this->tileOrderMap->insert(0xBC8, new QByteArray(QByteArray::fromHex(QString("5B5C24A3").toLatin1())));
    this->tileOrderMap->insert(0xBD0, new QByteArray(QByteArray::fromHex(QString("9D479E47").toLatin1())));
    this->tileOrderMap->insert(0xBD4, new QByteArray(QByteArray::fromHex(QString("47472727").toLatin1())));
    this->tileOrderMap->insert(0xBD8, new QByteArray(QByteArray::fromHex(QString("47474747").toLatin1())));
    this->tileOrderMap->insert(0xBDC, new QByteArray(QByteArray::fromHex(QString("27274747").toLatin1())));
    this->tileOrderMap->insert(0xBE0, new QByteArray(QByteArray::fromHex(QString("A947AA47").toLatin1())));
    this->tileOrderMap->insert(0xBEC, new QByteArray(QByteArray::fromHex(QString("52525252").toLatin1())));
    this->tileOrderMap->insert(0xBF8, new QByteArray(QByteArray::fromHex(QString("75BA76BB").toLatin1())));
    this->tileOrderMap->insert(0xBFC, new QByteArray(QByteArray::fromHex(QString("BABABBBB").toLatin1())));
    this->tileOrderMap->insert(0xC00, new QByteArray(QByteArray::fromHex(QString("45474547").toLatin1())));
    this->tileOrderMap->insert(0xC04, new QByteArray(QByteArray::fromHex(QString("47474747").toLatin1())));
    this->tileOrderMap->insert(0xC08, new QByteArray(QByteArray::fromHex(QString("45474547").toLatin1())));
    this->tileOrderMap->insert(0xC0C, new QByteArray(QByteArray::fromHex(QString("B4B6B5B7").toLatin1())));
    this->tileOrderMap->insert(0xC10, new QByteArray(QByteArray::fromHex(QString("45474547").toLatin1())));
    this->tileOrderMap->insert(0xC14, new QByteArray(QByteArray::fromHex(QString("45474547").toLatin1())));
    this->tileOrderMap->insert(0xC18, new QByteArray(QByteArray::fromHex(QString("45474547").toLatin1())));
    this->tileOrderMap->insert(0xC1C, new QByteArray(QByteArray::fromHex(QString("45474547").toLatin1())));
    this->tileOrderMap->insert(0xC20, new QByteArray(QByteArray::fromHex(QString("45474547").toLatin1())));
    this->tileOrderMap->insert(0xC24, new QByteArray(QByteArray::fromHex(QString("47474747").toLatin1())));
    this->tileOrderMap->insert(0xC28, new QByteArray(QByteArray::fromHex(QString("47474747").toLatin1())));
    this->tileOrderMap->insert(0xC2C, new QByteArray(QByteArray::fromHex(QString("47474747").toLatin1())));
    this->tileOrderMap->insert(0xC30, new QByteArray(QByteArray::fromHex(QString("47474747").toLatin1())));
    this->tileOrderMap->insert(0xC34, new QByteArray(QByteArray::fromHex(QString("47474747").toLatin1())));
    this->tileOrderMap->insert(0xC40, new QByteArray(QByteArray::fromHex(QString("ABACADAE").toLatin1())));
    this->tileOrderMap->insert(0xC44, new QByteArray(QByteArray::fromHex(QString("5D5E5D5E").toLatin1())));
    this->tileOrderMap->insert(0xC48, new QByteArray(QByteArray::fromHex(QString("C124C124").toLatin1())));
    this->tileOrderMap->insert(0xC4C, new QByteArray(QByteArray::fromHex(QString("C6C8C7C9").toLatin1())));
    this->tileOrderMap->insert(0xC50, new QByteArray(QByteArray::fromHex(QString("CACCCBCD").toLatin1())));
    this->tileOrderMap->insert(0xC54, new QByteArray(QByteArray::fromHex(QString("2A2A4040").toLatin1())));
    this->tileOrderMap->insert(0xC5C, new QByteArray(QByteArray::fromHex(QString("24472447").toLatin1())));
    this->tileOrderMap->insert(0xC60, new QByteArray(QByteArray::fromHex(QString("82838485").toLatin1())));
    this->tileOrderMap->insert(0xC64, new QByteArray(QByteArray::fromHex(QString("24472447").toLatin1())));
    this->tileOrderMap->insert(0xC68, new QByteArray(QByteArray::fromHex(QString("868A878B").toLatin1())));
    this->tileOrderMap->insert(0xC6C, new QByteArray(QByteArray::fromHex(QString("8E918F92").toLatin1())));
    this->tileOrderMap->insert(0xC8C, new QByteArray(QByteArray::fromHex(QString("41264126").toLatin1())));
    this->tileOrderMap->insert(0xC94, new QByteArray(QByteArray::fromHex(QString("B0B1B2B3").toLatin1())));
    this->tileOrderMap->insert(0xC98, new QByteArray(QByteArray::fromHex(QString("77797779").toLatin1())));
    this->tileOrderMap->insert(0xC9C, new QByteArray(QByteArray::fromHex(QString("53555456").toLatin1())));
    this->tileOrderMap->insert(0xCA0, new QByteArray(QByteArray::fromHex(QString("53555456").toLatin1())));
    this->tileOrderMap->insert(0xCA4, new QByteArray(QByteArray::fromHex(QString("A5A7A6A8").toLatin1())));
    this->tileOrderMap->insert(0xCA8, new QByteArray(QByteArray::fromHex(QString("C2C4C3C5").toLatin1())));
    this->tileOrderMap->insert(0xCAC, new QByteArray(QByteArray::fromHex(QString("5759585A").toLatin1())));
    this->tileOrderMap->insert(0xCB0, new QByteArray(QByteArray::fromHex(QString("7B7D7C7E").toLatin1())));
    this->tileOrderMap->insert(0x674E, new QByteArray(QByteArray::fromHex(QString("FCFCAAABACAD").toLatin1())));
    this->tileOrderMap->insert(0x6754, new QByteArray(QByteArray::fromHex(QString("FCFCAEAFB0B1").toLatin1())));
    this->tileOrderMap->insert(0x675A, new QByteArray(QByteArray::fromHex(QString("FCA5A6A7A8A9").toLatin1())));
    this->tileOrderMap->insert(0x6760, new QByteArray(QByteArray::fromHex(QString("FCA0A1A2A3A4").toLatin1())));
    this->tileOrderMap->insert(0x6766, new QByteArray(QByteArray::fromHex(QString("69A56AA7A8A9").toLatin1())));
    this->tileOrderMap->insert(0x676C, new QByteArray(QByteArray::fromHex(QString("6BA06CA2A3A4").toLatin1())));
    this->tileOrderMap->insert(0x6772, new QByteArray(QByteArray::fromHex(QString("FCFC96979899").toLatin1())));
    this->tileOrderMap->insert(0x6778, new QByteArray(QByteArray::fromHex(QString("FCFC9A9B9C9D").toLatin1())));
    this->tileOrderMap->insert(0x677E, new QByteArray(QByteArray::fromHex(QString("FCFC8F8E8E8F").toLatin1())));
    this->tileOrderMap->insert(0x6784, new QByteArray(QByteArray::fromHex(QString("FCFC95949495").toLatin1())));
    this->tileOrderMap->insert(0x678A, new QByteArray(QByteArray::fromHex(QString("FCFCDCDCDFDF").toLatin1())));
    this->tileOrderMap->insert(0x6790, new QByteArray(QByteArray::fromHex(QString("DCDCDDDDDEDE").toLatin1())));
    this->tileOrderMap->insert(0x6796, new QByteArray(QByteArray::fromHex(QString("FCFCB2B3B4B5").toLatin1())));
    this->tileOrderMap->insert(0x679C, new QByteArray(QByteArray::fromHex(QString("FCFCB6B3B7B5").toLatin1())));
    this->tileOrderMap->insert(0x67A2, new QByteArray(QByteArray::fromHex(QString("FCFC70717273").toLatin1())));
    this->tileOrderMap->insert(0x67A8, new QByteArray(QByteArray::fromHex(QString("FCFC6E6E6F6F").toLatin1())));
    this->tileOrderMap->insert(0x67AE, new QByteArray(QByteArray::fromHex(QString("FCFC6D6D6F6F").toLatin1())));
    this->tileOrderMap->insert(0x67B4, new QByteArray(QByteArray::fromHex(QString("FCFC6F6F6E6E").toLatin1())));
    this->tileOrderMap->insert(0x67BA, new QByteArray(QByteArray::fromHex(QString("FCFC6F6F6D6D").toLatin1())));
    this->tileOrderMap->insert(0x67C0, new QByteArray(QByteArray::fromHex(QString("FCFCF4F4F5F5").toLatin1())));
    this->tileOrderMap->insert(0x67C6, new QByteArray(QByteArray::fromHex(QString("FCFCF4F4F5F5").toLatin1())));
    this->tileOrderMap->insert(0x67CC, new QByteArray(QByteArray::fromHex(QString("FCFCF5F5F4F4").toLatin1())));
    this->tileOrderMap->insert(0x67D2, new QByteArray(QByteArray::fromHex(QString("FCFCF5F5F4F4").toLatin1())));
    this->tileOrderMap->insert(0x67D8, new QByteArray(QByteArray::fromHex(QString("FCFCFCFCEFEF").toLatin1())));
    this->tileOrderMap->insert(0x67DE, new QByteArray(QByteArray::fromHex(QString("B9B8BBBABCBC").toLatin1())));
    this->tileOrderMap->insert(0x67E4, new QByteArray(QByteArray::fromHex(QString("FCFCBDBDBCBC").toLatin1())));
    this->tileOrderMap->insert(0x67EA, new QByteArray(QByteArray::fromHex(QString("7A7BDADBD8D8").toLatin1())));
    this->tileOrderMap->insert(0x67F0, new QByteArray(QByteArray::fromHex(QString("CDCDCECECFCF").toLatin1())));
    this->tileOrderMap->insert(0x67F6, new QByteArray(QByteArray::fromHex(QString("7D7CD18CD3D2").toLatin1())));
    this->tileOrderMap->insert(0x67FC, new QByteArray(QByteArray::fromHex(QString("7D7C89888B8A").toLatin1())));
    this->tileOrderMap->insert(0x6802, new QByteArray(QByteArray::fromHex(QString("D5D4E3E2D3D2").toLatin1())));
    this->tileOrderMap->insert(0x6808, new QByteArray(QByteArray::fromHex(QString("D5D4E3E28B8A").toLatin1())));
    this->tileOrderMap->insert(0x680E, new QByteArray(QByteArray::fromHex(QString("E5E5E6E6EBEB").toLatin1())));
    this->tileOrderMap->insert(0x6814, new QByteArray(QByteArray::fromHex(QString("ECECEDEDEEEE").toLatin1())));
    this->tileOrderMap->insert(0x681A, new QByteArray(QByteArray::fromHex(QString("FCFCD0D0D7D7").toLatin1())));
    this->tileOrderMap->insert(0x6820, new QByteArray(QByteArray::fromHex(QString("BFBEC1C0C2FC").toLatin1())));
    this->tileOrderMap->insert(0x6826, new QByteArray(QByteArray::fromHex(QString("C4C3C6C5C8C7").toLatin1())));
    this->tileOrderMap->insert(0x682C, new QByteArray(QByteArray::fromHex(QString("BFBECAC9C2FC").toLatin1())));
    this->tileOrderMap->insert(0x6832, new QByteArray(QByteArray::fromHex(QString("C4C3C6C5CCCB").toLatin1())));
    this->tileOrderMap->insert(0x6838, new QByteArray(QByteArray::fromHex(QString("FCFCE8E7EAE9").toLatin1())));
    this->tileOrderMap->insert(0x683E, new QByteArray(QByteArray::fromHex(QString("F2F2F3F3F2F2").toLatin1())));
    this->tileOrderMap->insert(0x6844, new QByteArray(QByteArray::fromHex(QString("F1F1F1F1FCFC").toLatin1())));
    this->tileOrderMap->insert(0x684A, new QByteArray(QByteArray::fromHex(QString("F0F0FCFCFCFC").toLatin1())));
    this->tileOrderMap->insert(0x6BDD, new QByteArray(QByteArray::fromHex(QString("85858686").toLatin1())));
    this->tileOrderMap->insert(0x6E28, new QByteArray(QByteArray::fromHex(QString("0102030405060708").toLatin1())));
    this->tileOrderMap->insert(0x6E30, new QByteArray(QByteArray::fromHex(QString("090A0B0C0D0E0F10").toLatin1())));
    this->tileOrderMap->insert(0x6E38, new QByteArray(QByteArray::fromHex(QString("1112131415161718").toLatin1())));
    this->tileOrderMap->insert(0x6E40, new QByteArray(QByteArray::fromHex(QString("191A1B1C1D1E1F20").toLatin1())));
    this->tileOrderMap->insert(0x6E48, new QByteArray(QByteArray::fromHex(QString("2122232425262708").toLatin1())));
    this->tileOrderMap->insert(0x6E50, new QByteArray(QByteArray::fromHex(QString("0928292A2B2C2D08").toLatin1())));
    this->tileOrderMap->insert(0x6E58, new QByteArray(QByteArray::fromHex(QString("090A0B0C302C2D08").toLatin1())));
    this->tileOrderMap->insert(0x6E60, new QByteArray(QByteArray::fromHex(QString("090A0B2E2F2C2D08").toLatin1())));
    this->tileOrderMap->insert(0x6E68, new QByteArray(QByteArray::fromHex(QString("0928292A2B5C5D08").toLatin1())));
    this->tileOrderMap->insert(0x6E70, new QByteArray(QByteArray::fromHex(QString("090A0B0C0D5E5FFC").toLatin1())));
    this->tileOrderMap->insert(0x6E78, new QByteArray(QByteArray::fromHex(QString("FC080958595A5A08").toLatin1())));
    this->tileOrderMap->insert(0x6E80, new QByteArray(QByteArray::fromHex(QString("0928292A2B0E0FFC").toLatin1())));
    this->tileOrderMap->insert(0x6E88, new QByteArray(QByteArray::fromHex(QString("FCFCFC32333435FC").toLatin1())));
    this->tileOrderMap->insert(0x6E90, new QByteArray(QByteArray::fromHex(QString("FCFCFC36373839FC").toLatin1())));
    this->tileOrderMap->insert(0x6E98, new QByteArray(QByteArray::fromHex(QString("FCFCFC3A373B3CFC").toLatin1())));
    this->tileOrderMap->insert(0x6EA0, new QByteArray(QByteArray::fromHex(QString("FCFCFC3D3E3F40FC").toLatin1())));
    this->tileOrderMap->insert(0x6EA8, new QByteArray(QByteArray::fromHex(QString("FCFCFC32414243FC").toLatin1())));
    this->tileOrderMap->insert(0x6EB0, new QByteArray(QByteArray::fromHex(QString("FCFCFC32334445FC").toLatin1())));
    this->tileOrderMap->insert(0x6EB8, new QByteArray(QByteArray::fromHex(QString("FCFCFC32334447FC").toLatin1())));
    this->tileOrderMap->insert(0x6EC0, new QByteArray(QByteArray::fromHex(QString("FCFCFC32334849FC").toLatin1())));
    this->tileOrderMap->insert(0x6EC8, new QByteArray(QByteArray::fromHex(QString("FCFCFC32339091FC").toLatin1())));
    this->tileOrderMap->insert(0x6ED0, new QByteArray(QByteArray::fromHex(QString("FCFCFC3A379293FC").toLatin1())));
    this->tileOrderMap->insert(0x6ED8, new QByteArray(QByteArray::fromHex(QString("FCFCFC9E9E9F9FFC").toLatin1())));
    this->tileOrderMap->insert(0x6EE0, new QByteArray(QByteArray::fromHex(QString("FCFCFC3A374F4FFC").toLatin1())));
    this->tileOrderMap->insert(0x6EE8, new QByteArray(QByteArray::fromHex(QString("FC00014C4D4E4E00").toLatin1())));
    this->tileOrderMap->insert(0x6EF0, new QByteArray(QByteArray::fromHex(QString("014C4D4A4A4B4B31").toLatin1())));
}

void Graphics_Combiner::Deallocate_Tile_Order_Map() {
    if (this->tileOrderMap) {
        for (QByteArray *bytes : this->tileOrderMap->values()) delete bytes;
    }
    delete this->tileOrderMap;
}
