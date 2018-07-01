#include "Graphics.h"
#include "../../Common_Files/Version.h"
#include "Sequential_Archive_Handler.h"
#include "Text.h"
#include <assert.h>

Graphics::Graphics(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler, Text *text) : Byte_Writer(file, levelOffset) {
    assert(sequentialArchiveHandler);
    assert(text);
    this->sequentialArchiveHandler = sequentialArchiveHandler;
    this->text = text;
}

bool Graphics::Write_Title_Screen_Core() {
    if (!this->Write_Bytes_To_Offset(0x9ED1, QByteArray::fromHex(QString("8702ABAD20894E452097"
            "02ABAD20A702ACAE20A94E4720B702ACAE20C7424720C94E2420D7424720"
            "E7424720E9C22420EA051C1E190E1B20EF482420F742472107424721091024160A1B1218240B1B18"
            "1CAF242447472127424721294E4521374247214752472167424721694E2421774247218742472189"
            "C224218A0F150E1F0E1528110E0A0D0E0D24474721A7424721A9422421AB0E1F242400AF2403AF24"
            "002424474721C702ABAD21C94E4521D702ABAD21E702ACAE21E94E4721F702ACAE224B0D01241915"
            "0A220E1B24100A160E228B0D022419150A220E1B24100A160E22EC041D18192822F6010023C95655"
            "23E20499AAAAAA23EA0499AAAAAA0000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000").toLatin1()))) return false;
    return this->Write_Bytes_To_Offset(0x9F68, this->Get_Version_Bytes());
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
    return this->Write_Bytes_To_Offset(0x9F94, QByteArray::fromHex(QString("0A160A1B121824100A160E228B0A151E12101224100A160E22EC041D18192822F6010023C9565523"
            "E20499AAAAAA23EA0499AAAAAA000000000000").toLatin1()));
}

bool Graphics::Write_Title_Screen_For_2_Player_Game() {
    return this->Write_Title_Screen_Core();
}
