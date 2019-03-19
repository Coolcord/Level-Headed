#include "Graphics.h"
#include "../../Common_Files/Version.h"
#include "../../../Hexagon/Hexagon/Patch_Strings.h"
#include "Level_Offset.h"
#include "Sequential_Archive_Handler.h"
#include "Text.h"
#include <assert.h>
#include <QTextStream>

const static qint64 DEFAULT_VERSION_OFFSET = 0x9F68;
const static qint64 DEFAULT_PARTIAL_VERSION_OFFSET = 0x9F75;
const static QString STRING_VERSION_OFFSET = Patch_Strings::STRING_COMMENT+" Version Offset: ";
const static QString STRING_CUTTER_FLOWER = "Cutter Flower";
const static QString STRING_DOUBLE_JUMP_STAR = "Double Jump Star";
const static QString STRING_FIRE_STAR = "Fire Star";
const static QString STRING_HAMMER_SUIT = "Hammer Suit";
const static QString STRING_POISON_BUBBLES = "Poison Bubbles";
const static QString STRING_POWER_WAND = "Power Wand";
const static QString STRING_SLIME_FLOWER = "Slime Flower";
const static QString STRING_SPINBALL_FLOWER = "Spinball Flower";
const static QString STRING_TITLE_SCREEN_1P = "Title Screen 1P";
const static QString STRING_TITLE_SCREEN_2P = "Title Screen 2P";

Graphics::Graphics(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler, Text *text) : Byte_Writer(file, levelOffset) {
    assert(text); assert(sequentialArchiveHandler);
    this->versionOffset = DEFAULT_VERSION_OFFSET;
    this->sequentialArchiveHandler = sequentialArchiveHandler;
    this->text = text;
}

bool Graphics::Apply_Cutter_Flower_Fix() { return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_CUTTER_FLOWER); }
bool Graphics::Apply_Double_Jump_Star_Fix() { return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_DOUBLE_JUMP_STAR); }
bool Graphics::Apply_Fire_Star_Fix() { return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_FIRE_STAR); }
bool Graphics::Apply_Hammer_Suit_Fix() { return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_HAMMER_SUIT); }
bool Graphics::Apply_Poison_Bubbles_Fix() { return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_POISON_BUBBLES); }
bool Graphics::Apply_Power_Wand_Fix() { return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_POWER_WAND); }
bool Graphics::Apply_Slime_Flower_Fix() { return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_SLIME_FLOWER); }
bool Graphics::Apply_Spinball_Flower_Fix() { return this->sequentialArchiveHandler->Apply_Graphics_Fix(STRING_SPINBALL_FLOWER); }

bool Graphics::Apply_Title_Screen_1P_Fix(qint64 &versionOffset) {
    QByteArray patchBytes = this->sequentialArchiveHandler->Read_Graphics_Fix(STRING_TITLE_SCREEN_1P);
    if (patchBytes.isEmpty()) return true; //nothing to do
    this->Get_Version_Offset_From_Title_Screen_Fix(patchBytes, versionOffset);
    return this->sequentialArchiveHandler->Apply_Graphics_Fix(patchBytes);
}

bool Graphics::Apply_Title_Screen_2P_Fix(qint64 &versionOffset) {
    QByteArray patchBytes = this->sequentialArchiveHandler->Read_Graphics_Fix(STRING_TITLE_SCREEN_2P);
    if (patchBytes.isEmpty()) return true; //nothing to do
    this->Get_Version_Offset_From_Title_Screen_Fix(patchBytes, versionOffset);
    return this->sequentialArchiveHandler->Apply_Graphics_Fix(patchBytes);
}

bool Graphics::Change_1UP_Palette(int palette) {
    if (palette < 0 || palette > 3) return false;
    return this->Write_Bytes_To_Offset(0x66E1, QByteArray(1, static_cast<char>(palette)));
}

bool Graphics::Write_Title_Screen_Core() {
    this->versionOffset = DEFAULT_VERSION_OFFSET;
    return this->Write_Bytes_To_Offset(0x9ED1, QByteArray::fromHex(QString("8702ABAD20894E452097"
            "02ABAD20A702ACAE20A94E4720B702ACAE20C7424720C94E2420D7424720"
            "E7424720E9C22420EA051C1E190E1B20EF482420F742472107424721091024160A1B1218240B1B18"
            "1CAF242447472127424721294E4521374247214752472167424721694E2421774247218742472189"
            "C224218A0F150E1F0E1528110E0A0D0E0D24474721A7424721A9422421AB0E1F242400AF2403AF24"
            "002424474721C702ABAD21C94E4521D702ABAD21E702ACAE21E94E4721F702ACAE224B0D01241915"
            "0A220E1B24100A160E228B0D022419150A220E1B24100A160E22EC041D18192822F6010023C95655"
            "23E20499AAAAAA23EA0499AAAAAA0000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000").toLatin1()));
}

QByteArray Graphics::Get_Version_Bytes() {
    QStringList versionNumbers = Version::VERSION_NUMBER.split('-').first().split('.');
    assert(versionNumbers.size() == 3);

    //Parse the Version Numbers
    bool valid = false;
    int significant = versionNumbers.at(0).toInt(&valid); assert(valid);
    int major = versionNumbers.at(1).toInt(&valid); assert(valid);
    int minor = versionNumbers.at(2).toInt(&valid); assert(valid);
    QString significantString = "", majorString = "", minorString = "";
    if (significant < 10) significantString = "  "+QString::number(significant);
    else if (significant < 100) significantString = " "+QString::number(significant);
    else if (significant < 1000) significantString = QString::number(significant);
    else assert(false);
    if (major < 10) majorString = " "+QString::number(major);
    else if (major < 100) majorString = QString::number(major);
    else assert(false);
    if (minor < 10) minorString = " "+QString::number(minor);
    else if (minor < 100) minorString = QString::number(minor);
    else assert(false);

    //Convert the string to SMB Bytes
    QString version = "v"+significantString+"."+majorString+"."+minorString;
    QByteArray bytes = this->text->Convert_String_To_SMB_Bytes(version);
    assert(bytes.size() == 10);
    return bytes;
}

bool Graphics::Write_Title_Screen_For_1_Player_Game() {
    if (!this->Write_Title_Screen_Core()) return false;
    if (!this->Write_Bytes_To_Offset(0x9F94, QByteArray::fromHex(QString("0A160A1B121824100A160E228B0A151E12101224100A160E22EC041D18192822F6010023C9565523"
            "E20499AAAAAA23EA0499AAAAAA000000000000").toLatin1()))) return false;
    if (!this->Apply_Title_Screen_1P_Fix(this->versionOffset)) return false;
    return this->Write_Bytes_To_Offset(this->versionOffset, this->Get_Version_Bytes());
}

bool Graphics::Write_Title_Screen_For_2_Player_Game() {
    if (!this->Write_Title_Screen_Core()) return false;
    if (!this->Apply_Title_Screen_2P_Fix(this->versionOffset)) return false;
    return this->Write_Bytes_To_Offset(this->versionOffset, this->Get_Version_Bytes());
}

bool Graphics::Write_Title_Screen_For_Partial_Game() {
    if (this->levelOffset->Get_ROM_Type() == ROM_Type::COOP_CGTI_1) {
        if (!this->Write_Bytes_To_Offset(0x9ED1, QByteArray::fromHex(QString("8702ABAD20894E45209702ABAD20A702ACAE20A94E4720B702ACAE20C7"
            "424720C94E2420D7424720E7424720E9C22420EA051C1E190E1B20EF482420F742472107424721091024160A1B1218240B1B181CAF242447472127424721294E452137424721475247216742472169C324216A050C18281819216F4824217742472187424721891024150E1F0E1528110E0A0D0E0D24474721A7424721A9422421AB0E1F242400AF2403AF240124244747"
            "21C702ABAD21C94E4521D702ABAD21E702ACAE21E94E4721F702ACAE224B0D012419150A220E1B24100A160E228B0C161E151D122819150A220E1B22EC041D18192822F6010023C9565523E20499AAAAAA23EA0499AAAAAA000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000FFFFFFFFFFFF").toLatin1()))) return false;
        return this->Write_Bytes_To_Offset(0x9F71, this->Get_Version_Bytes());
    } else {
        if (!this->Write_Bytes_To_Offset(0x9ED1, QByteArray::fromHex(QString("8702ABAD20894E45209702ABAD20A702ACAE20A94E4720B702ACAE20C7"
            "424720C94E2420D7424720E7424720E9C22420EA051C1E190E1B20EF482420F742472107424721091024160A1B1218240B1B181CAF242447472127424721294E4521374247214752472167424721694224216B0E190A1B1D120A1515222424244747218742472189C224218A0F150E1F0E152811"
            "0E0A0D0E0D24474721A7424721A9422421AB0E1F242400AF2403AF24012424474721C702ABAD21C94E4521D702ABAD21E702ACAE21E94E4721F702ACAE224B0D012419150A220E1B24100A160E228B0D022419150A220E1B24100A160E22EC041D18192822F6010023C9565523E20499AAAAAA23EA0499AAAAAA00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000FFFFFFFFFFFF").toLatin1()))) return false;
        return this->Write_Bytes_To_Offset(DEFAULT_PARTIAL_VERSION_OFFSET, this->Get_Version_Bytes());
    }
}

void Graphics::Get_Version_Offset_From_Title_Screen_Fix(const QByteArray &patchBytes, qint64 &versionOffset) {
    QTextStream stream(patchBytes);
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.startsWith(Patch_Strings::STRING_CHECKSUM) || line.startsWith(Patch_Strings::STRING_OFFSET)) return;
        if (line.startsWith(STRING_VERSION_OFFSET)) { //attempt to parse the offset
            line = line.remove(0, STRING_VERSION_OFFSET.size());
            bool isHex = false, valid = false;
            if (line.startsWith(Patch_Strings::STRING_HEX_IDENTIFIER)) {
                line = line.remove(0, Patch_Strings::STRING_HEX_IDENTIFIER.size());
                isHex = true;
            }
            qint64 num = 0;
            if (isHex) num = line.toLongLong(&valid, 0x10);
            else num = line.toLongLong(&valid, 10);
            if (valid) {
                versionOffset = num;
                return;
            }
        }
    }
}
