#include "Graphics_Offsets.h"

Graphics_Offsets::Graphics_Offsets() {
    this->tileOrderMap = nullptr;
    this->Initialize_Tile_Order_Map();
}

Graphics_Offsets::~Graphics_Offsets() {
    this->Deallocate_Tile_Order_Map();
}

QByteArray *Graphics_Offsets::Get_Values_At_Offset(qint64 offset) {
    QMap<qint64, QByteArray*>::iterator iter = this->tileOrderMap->find(offset);
    if (iter == this->tileOrderMap->end()) return nullptr;
    return iter.value();
}

QByteArray *Graphics_Offsets::Get_Values_At_Offset_And_Never_Fail(qint64 offset) {
    QMap<qint64, QByteArray*>::iterator iter = this->tileOrderMap->find(offset);
    assert(iter != this->tileOrderMap->end());
    return iter.value();
}

QStack<qint64> Graphics_Offsets::Get_Air_Bubble_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Blooper_Offsets() {
    QStack<qint64> offsets; offsets.push(0x678A); offsets.push(0x6790);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Bowser_Offsets() {
    QStack<qint64> offsets; offsets.push(0x6820); offsets.push(0x6826); offsets.push(0x682C); offsets.push(0x6832);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Bowser_Fire_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Brick_Piece_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Bullet_Bill_Offsets() {
    QStack<qint64> offsets; offsets.push(0x6838);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Buzzy_Beetle_Offsets() {
    QStack<qint64> offsets; offsets.push(0x674E); offsets.push(0x6754); offsets.push(0x67C0); offsets.push(0x67C6); offsets.push(0x67CC); offsets.push(0x67D2);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Castle_Flag_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Cheep_Cheep_Offsets() {
    QStack<qint64> offsets; offsets.push(0x6796); offsets.push(0x679C);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Coin_Animation_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Explosion_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Fireball_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Fire_Flower_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Flagpole_Flag_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Goomba_Offsets() {
    QStack<qint64> offsets; offsets.push(0x67A2); offsets.push(0x67D8);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Hammer_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Hammer_Bro_Offsets() {
    QStack<qint64> offsets; offsets.push(0x67F6); offsets.push(0x67FC); offsets.push(0x6802); offsets.push(0x6808);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Jump_Spring_Base_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C5C); offsets.push(0x0C64);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Jump_Spring_Animation_Offsets() {
    QStack<qint64> offsets; offsets.push(0x683E); offsets.push(0x6844); offsets.push(0x684A);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Koopa_Offsets() {
    QStack<qint64> offsets; offsets.push(0x675A); offsets.push(0x6760); offsets.push(0x6766); offsets.push(0x676C); offsets.push(0x67A8); offsets.push(0x67AE); offsets.push(0x67B4); offsets.push(0x67BA);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Lakitu_Offsets() {
    QStack<qint64> offsets; offsets.push(0x67DE); offsets.push(0x67E4);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Lift_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Mario_Offsets() {
    QStack<qint64> offsets; offsets.push(0x6E27); offsets.push(0x6E2F); offsets.push(0x6E37); offsets.push(0x6E3F); offsets.push(0x6E47); offsets.push(0x6E4F); offsets.push(0x6E57); offsets.push(0x6E5F); offsets.push(0x6E67);
    offsets.push(0x6E6F); offsets.push(0x6E77); offsets.push(0x6E7F); offsets.push(0x6E87); offsets.push(0x6E8F); offsets.push(0x6E97); offsets.push(0x6E9F); offsets.push(0x6EA7); offsets.push(0x6EAF); offsets.push(0x6EB7);
    offsets.push(0x6EBF); offsets.push(0x6EC7); offsets.push(0x6ECF); offsets.push(0x6ED7); offsets.push(0x6EDF); offsets.push(0x6EE7); offsets.push(0x6EEF);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_One_Up_Font_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Mushroom_Powerup_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Peach_Offsets() {
    QStack<qint64> offsets; offsets.push(0x67EA);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Piranha_Plant_Offsets() {
    QStack<qint64> offsets; offsets.push(0x680E); offsets.push(0x6814);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Podoboo_Offsets() {
    QStack<qint64> offsets; offsets.push(0x681A);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Score_Font_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Sky_Lift_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Spiny_Offsets() {
    QStack<qint64> offsets; offsets.push(0x6772); offsets.push(0x6778);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Spiny_Egg_Offsets() {
    QStack<qint64> offsets; offsets.push(0x677E); offsets.push(0x6784);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Starman_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Toad_Offsets() {
    QStack<qint64> offsets; offsets.push(0x67F0);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Axe_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0CB0);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Brick_Block_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0BD0); offsets.push(0x0BD4); offsets.push(0x0BD8); offsets.push(0x0BDC); offsets.push(0x0BE0); offsets.push(0x0C00); offsets.push(0x0C04); offsets.push(0x0C08);
    offsets.push(0x0C10); offsets.push(0x0C14); offsets.push(0x0C18); offsets.push(0x0C1C); offsets.push(0x0C20); offsets.push(0x0C24); offsets.push(0x0C28); offsets.push(0x0C2C); offsets.push(0x0C30);
    offsets.push(0x0C34);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Brick_Block_Animation_Offsets() {
    QStack<qint64> offsets; offsets.push(0x6BDD);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Bowser_Bridge_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C98);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Bridge_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0B4C); offsets.push(0x0C48);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Bullet_Bill_Cannon_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C4C); offsets.push(0x0C50); offsets.push(0x0C54);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Castle_Block_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C44);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Chain_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0B50);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Cloud_Block_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C94);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Coin_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0CA4); offsets.push(0x0CA8);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Coin_Icon_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Coral_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0BA8);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Flagpole_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0BB0); offsets.push(0x0BB4);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Font_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Mushroom_Platform_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0B84); offsets.push(0x0B88); offsets.push(0x0B8C); offsets.push(0x0BF8); offsets.push(0x0BFC);
     offsets.push(0x0B54); offsets.push(0x0B5C); //tall trees share mushroom tiles
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Overworld_Block_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C0C);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Pipe_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C68); offsets.push(0x0C6C); offsets.push(0x0B60); offsets.push(0x0B64); offsets.push(0x0B68); offsets.push(0x0B6C); offsets.push(0x0B70); offsets.push(0x0B74);
    offsets.push(0x0B90); offsets.push(0x0B94); offsets.push(0x0B98); offsets.push(0x0B9C); offsets.push(0x0BA0); offsets.push(0x0BA4);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Question_Block_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C9C); offsets.push(0x0CA0); offsets.push(0x0CAC);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Rope_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0BBC); offsets.push(0x0BC0); offsets.push(0x0BC4); offsets.push(0x0BC8);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Selector_Icon_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Solid_Block_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C40);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Tree_Platform_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0B78); offsets.push(0x0B7C); offsets.push(0x0B80); offsets.push(0x0BEC);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Underwater_Block_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C60);
    return offsets;
}

QStack<qint64> Graphics_Offsets::Get_Vine_Offsets() {
    return QStack<qint64>();
}

QStack<qint64> Graphics_Offsets::Get_Water_Offsets() {
    QStack<qint64> offsets; offsets.push(0x0C8C);
    return offsets;
}

void Graphics_Offsets::Initialize_Tile_Order_Map() {
    this->Deallocate_Tile_Order_Map();
    this->tileOrderMap = new QMap<qint64, QByteArray*>();

    //=========================================================================================================================
    //====================================================== DO NOT EDIT ======================================================
    //============================= THIS SECTION IS AUTOMATICALLY GENERATED BY THE GRAPHICS RIPPER ============================
    //=========================================================================================================================
    this->tileOrderMap->insert(0xB4C, new QByteArray(QByteArray::fromHex(QString("24C024C0").toLatin1())));
    this->tileOrderMap->insert(0xB50, new QByteArray(QByteArray::fromHex(QString("247F7F24").toLatin1())));
    this->tileOrderMap->insert(0xB54, new QByteArray(QByteArray::fromHex(QString("B8BAB9BB").toLatin1())));
    this->tileOrderMap->insert(0xB5C, new QByteArray(QByteArray::fromHex(QString("BABCBBBD").toLatin1())));
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
    this->tileOrderMap->insert(0xBEC, new QByteArray(QByteArray::fromHex(QString("52525252").toLatin1())));
    this->tileOrderMap->insert(0xBF8, new QByteArray(QByteArray::fromHex(QString("75BA76BB").toLatin1())));
    this->tileOrderMap->insert(0xBFC, new QByteArray(QByteArray::fromHex(QString("BABABBBB").toLatin1())));
    this->tileOrderMap->insert(0xC0C, new QByteArray(QByteArray::fromHex(QString("B4B6B5B7").toLatin1())));
    this->tileOrderMap->insert(0xC40, new QByteArray(QByteArray::fromHex(QString("ABACADAE").toLatin1())));
    this->tileOrderMap->insert(0xC44, new QByteArray(QByteArray::fromHex(QString("5D5E5D5E").toLatin1())));
    this->tileOrderMap->insert(0xC48, new QByteArray(QByteArray::fromHex(QString("C124C124").toLatin1())));
    this->tileOrderMap->insert(0xC4C, new QByteArray(QByteArray::fromHex(QString("C6C8C7C9").toLatin1())));
    this->tileOrderMap->insert(0xC50, new QByteArray(QByteArray::fromHex(QString("CACCCBCD").toLatin1())));
    this->tileOrderMap->insert(0xC54, new QByteArray(QByteArray::fromHex(QString("2A2A4040").toLatin1())));
    this->tileOrderMap->insert(0xC60, new QByteArray(QByteArray::fromHex(QString("82838485").toLatin1())));
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
    //=========================================================================================================================
}

void Graphics_Offsets::Deallocate_Tile_Order_Map() {
    if (this->tileOrderMap) {
        for (QByteArray *bytes : this->tileOrderMap->values()) delete bytes;
    }
    delete this->tileOrderMap;
}
