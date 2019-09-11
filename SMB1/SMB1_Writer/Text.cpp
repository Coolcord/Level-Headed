#include "Text.h"

Text::Text(QFile *f, Level_Offset *lo) : Byte_Writer(f, lo) {}

bool Text::Set_Mario_Name(const QString &name) {
    if (name == "MARIO") return true; //assume there is nothing to do
    QString trimmedName = name;
    if (trimmedName.size() > 5) trimmedName.resize(5);
    QByteArray nameBytes = this->Convert_String_To_SMB_Bytes(trimmedName);
    QByteArray spaceBytes = this->Convert_String_To_SMB_Bytes(QString(QByteArray(5-trimmedName.size(), ' ')));
    QByteArray exclamationBytes = this->Convert_String_To_SMB_Bytes(QString(QByteArray(5-trimmedName.size(), '!')));
    QByteArray leftJustified = nameBytes+spaceBytes;

    if (!this->Write_Bytes_To_Offset(0x0765, leftJustified)) return false; //HUD
    if (!this->Write_Bytes_To_Offset(0x07AB, leftJustified)) return false; //Time Up
    if (!this->Write_Bytes_To_Offset(0x07BE, leftJustified)) return false; //Game Over
    if (!this->Write_Bytes_To_Offset(0x0D71, nameBytes+exclamationBytes)) return false; //Thank you Mario!
    return true;
}

bool Text::Set_Luigi_Name(const QString &name) {
    if (name == "LUIGI") return true; //assume there is nothing to do
    QString trimmedName = name;
    if (trimmedName.size() > 5) trimmedName.resize(5);
    QByteArray nameBytes = this->Convert_String_To_SMB_Bytes(trimmedName);
    QByteArray spaceBytes = this->Convert_String_To_SMB_Bytes(QString(QByteArray(5-trimmedName.size(), ' ')));
    QByteArray exclamationBytes = this->Convert_String_To_SMB_Bytes(QString(QByteArray(5-trimmedName.size(), '!')));
    QByteArray leftJustified = nameBytes+spaceBytes;

    if (!this->Write_Bytes_To_Offset(0x07FD, leftJustified)) return false; //HUD, Time Up, Game Over
    if (!this->Write_Bytes_To_Offset(0x0D85, nameBytes+exclamationBytes)) return false; //Thank you Luigi!
    return true;
}

QByteArray Text::Convert_String_To_SMB_Bytes(const QString &string) {
    QByteArray bytes(string.size(), 0x00);
    for (int i = 0; i < string.size(); ++i) {
        switch (string.at(i).toLower().toLatin1()) {
        default:        bytes[i] = static_cast<char>(0x24); break; //unsupported character. Simply use a space instead
        case ' ':       bytes[i] = static_cast<char>(0x24); break;
        case '!':       bytes[i] = static_cast<char>(0x2B); break;
        case '.':       bytes[i] = static_cast<char>(0xAF); break;
        case '-':       bytes[i] = static_cast<char>(0x28); break;
        case '0':       bytes[i] = static_cast<char>(0x00); break;
        case '1':       bytes[i] = static_cast<char>(0x01); break;
        case '2':       bytes[i] = static_cast<char>(0x02); break;
        case '3':       bytes[i] = static_cast<char>(0x03); break;
        case '4':       bytes[i] = static_cast<char>(0x04); break;
        case '5':       bytes[i] = static_cast<char>(0x05); break;
        case '6':       bytes[i] = static_cast<char>(0x06); break;
        case '7':       bytes[i] = static_cast<char>(0x07); break;
        case '8':       bytes[i] = static_cast<char>(0x08); break;
        case '9':       bytes[i] = static_cast<char>(0x09); break;
        case 'a':       bytes[i] = static_cast<char>(0x0A); break;
        case 'b':       bytes[i] = static_cast<char>(0x0B); break;
        case 'c':       bytes[i] = static_cast<char>(0x0C); break;
        case 'd':       bytes[i] = static_cast<char>(0x0D); break;
        case 'e':       bytes[i] = static_cast<char>(0x0E); break;
        case 'f':       bytes[i] = static_cast<char>(0x0F); break;
        case 'g':       bytes[i] = static_cast<char>(0x10); break;
        case 'h':       bytes[i] = static_cast<char>(0x11); break;
        case 'i':       bytes[i] = static_cast<char>(0x12); break;
        case 'j':       bytes[i] = static_cast<char>(0x13); break;
        case 'k':       bytes[i] = static_cast<char>(0x14); break;
        case 'l':       bytes[i] = static_cast<char>(0x15); break;
        case 'm':       bytes[i] = static_cast<char>(0x16); break;
        case 'n':       bytes[i] = static_cast<char>(0x17); break;
        case 'o':       bytes[i] = static_cast<char>(0x18); break;
        case 'p':       bytes[i] = static_cast<char>(0x19); break;
        case 'q':       bytes[i] = static_cast<char>(0x1A); break;
        case 'r':       bytes[i] = static_cast<char>(0x1B); break;
        case 's':       bytes[i] = static_cast<char>(0x1C); break;
        case 't':       bytes[i] = static_cast<char>(0x1D); break;
        case 'u':       bytes[i] = static_cast<char>(0x1E); break;
        case 'v':       bytes[i] = static_cast<char>(0x1F); break;
        case 'w':       bytes[i] = static_cast<char>(0x20); break;
        case 'x':       bytes[i] = static_cast<char>(0x21); break;
        case 'y':       bytes[i] = static_cast<char>(0x22); break;
        case 'z':       bytes[i] = static_cast<char>(0x23); break;
        }
    }
    return bytes;
}
