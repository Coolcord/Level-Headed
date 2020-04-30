#include "Text.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"

Text::Text(QFile *f, Level_Offset *lo) : Byte_Writer(f, lo) {
    this->specialP1Name = QString();
    this->specialP2Name = QString();
    this->p1Name = "MARIO";
    this->p2Name = "LUIGI";
    this->p1ThankYouText = "THANK YOU";
    this->p2ThankYouText = "THANK YOU";
}

bool Text::Replace_Castle_With_Text(const QString &text) {
    const QString ORIGINAL_PREFIX = "ANOTHER ";
    const QString ORIGINAL_WORD = "CASTLE";
    const QString ORIGINAL_SUFFIX = "!";
    const QString ORIGINAL_TEXT = ORIGINAL_PREFIX+ORIGINAL_WORD+ORIGINAL_SUFFIX;
    if (text.size() > ORIGINAL_WORD.size()) return false;
    QString newText = ORIGINAL_PREFIX+text+ORIGINAL_SUFFIX;
    QString padding;
    int paddingSize = ORIGINAL_TEXT.size()-newText.size();
    for (int i = 0; i < paddingSize; ++i) padding += " ";
    newText += padding;
    return this->Write_Bytes_To_Offset(0x0DA8, this->Convert_String_To_SMB_Bytes(newText));
}

bool Text::Replace_Princess_With_Text(const QString &text) {
    const QString ORIGINAL_PREFIX = "BUT OUR ";
    const QString ORIGINAL_WORD = "PRINCESS";
    const QString ORIGINAL_SUFFIX = " IS IN";
    const QString ORIGINAL_TEXT = ORIGINAL_PREFIX+ORIGINAL_WORD+ORIGINAL_SUFFIX;
    if (text.size() > ORIGINAL_WORD.size()) return false;
    QString newText = ORIGINAL_PREFIX+text+ORIGINAL_SUFFIX;
    QString padding;
    int paddingSize = ORIGINAL_TEXT.size()-newText.size();
    for (int i = 0; i < paddingSize; ++i) padding += " ";
    newText += padding;
    return this->Write_Bytes_To_Offset(0x0D8F, this->Convert_String_To_SMB_Bytes(newText));
}

bool Text::Replace_Thank_You_Player_One_With_Text(const QString &text) {
    const QString STRING_THANK_YOU = "THANK YOU";
    const QString STRING_THANK_YOU_MARIO = "THANK YOU MARIO!";
    if (text.size() > STRING_THANK_YOU.size()) return false;
    if (!this->Write_Bytes_To_Offset(0x0D67, this->Convert_String_To_SMB_Bytes(this->Get_Centered_Text(STRING_THANK_YOU_MARIO, text+" "+this->p1Name+"!")))) return false;
    this->p1ThankYouText = text;
    return true;
}

bool Text::Replace_Thank_You_Player_Two_With_Text(const QString &text) {
    const QString STRING_THANK_YOU = "THANK YOU";
    const QString STRING_THANK_YOU_LUIGI = "THANK YOU LUIGI!";
    if (text.size() > STRING_THANK_YOU.size()) return false;
    if (!this->Write_Bytes_To_Offset(0x0D7B, this->Convert_String_To_SMB_Bytes(this->Get_Centered_Text(STRING_THANK_YOU_LUIGI, text+" "+this->p2Name+"!")))) return false;
    this->p2ThankYouText = text;
    return true;
}

bool Text::Set_Mario_Name(const QString &name) {
    const QString MARIO_NAME = "MARIO";
    QString upperName = name.toUpper();
    if (upperName == MARIO_NAME) {
        if (!this->specialP1Name.isEmpty() && this->specialP1Name != MARIO_NAME) return this->Set_Mario_Name(this->specialP1Name);
        else return true; //assume there is nothing to do
    }
    QString trimmedName = name;
    if (trimmedName.size() > 5) trimmedName.resize(5);
    QByteArray nameBytes = this->Convert_String_To_SMB_Bytes(trimmedName);
    QByteArray spaceBytes = this->Convert_String_To_SMB_Bytes(QString(QByteArray(5-trimmedName.size(), ' ')));
    QByteArray leftJustified = nameBytes+spaceBytes;

    if (!this->Write_Bytes_To_Offset(0x0765, leftJustified)) return false; //HUD
    if (!this->Write_Bytes_To_Offset(0x07AB, leftJustified)) return false; //Time Up
    if (!this->Write_Bytes_To_Offset(0x07BE, leftJustified)) return false; //Game Over
    if (trimmedName.size() > 0) { //Thank you Mario!
        if (!this->Write_Bytes_To_Offset(0x0D67, this->Convert_String_To_SMB_Bytes(this->Get_Centered_Text("THANK YOU MARIO!", this->p1ThankYouText+" "+trimmedName+"!")))) return false;
    } else {
        if (!this->Write_Bytes_To_Offset(0x0D67, this->Convert_String_To_SMB_Bytes(this->Get_Centered_Text("THANK YOU MARIO!", this->p1ThankYouText+"!")))) return false;
    }
    if (trimmedName.size() > 0) this->Find_And_Replace_Next_Instance(MARIO_NAME+" GAME", trimmedName+" GAME", 0x9ED1, 0xA010); //Title Screen
    this->p1Name = upperName;
    return true;
}

bool Text::Set_Luigi_Name(const QString &name) {
    const QString LUIGI_NAME = "LUIGI";
    QString upperName = name.toUpper();
    if (upperName == LUIGI_NAME) {
        if (!this->specialP2Name.isEmpty() && this->specialP2Name != LUIGI_NAME) return this->Set_Luigi_Name(this->specialP2Name);
        else return true; //assume there is nothing to do
    }
    QString trimmedName = name;
    if (trimmedName.size() > 5) trimmedName.resize(5);
    QByteArray nameBytes = this->Convert_String_To_SMB_Bytes(trimmedName);
    QByteArray spaceBytes = this->Convert_String_To_SMB_Bytes(QString(QByteArray(5-trimmedName.size(), ' ')));
    QByteArray leftJustified = nameBytes+spaceBytes;

    if (!this->Write_Bytes_To_Offset(0x07FD, leftJustified)) return false; //HUD, Time Up, Game Over
    if (trimmedName.size() > 0) { //Thank you Luigi!
        if (!this->Write_Bytes_To_Offset(0x0D7B, this->Convert_String_To_SMB_Bytes(this->Get_Centered_Text("THANK YOU LUIGI!", this->p2ThankYouText+" "+trimmedName+"!")))) return false;
    } else {
        if (!this->Write_Bytes_To_Offset(0x0D7B, this->Convert_String_To_SMB_Bytes(this->Get_Centered_Text("THANK YOU LUIGI!", this->p2ThankYouText+"!")))) return false;
    }
    if (trimmedName.size() > 0) this->Find_And_Replace_Next_Instance(LUIGI_NAME+" GAME", trimmedName+" GAME", 0x9ED1, 0xA010); //Title Screen
    this->p2Name = upperName;
    return true;
}

void Text::Set_Special_P1_Name(const QString &name) {
    if (name.size() > 5) return;
    this->specialP1Name = name.toUpper();
}

void Text::Set_Special_P2_Name(const QString &name) {
    if (name.size() > 5) return;
    this->specialP2Name = name.toUpper();
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

QString Text::Convert_SMB_Bytes_To_String(const QByteArray &bytes) {
    QString string = QString();
    for (int i = 0; i < bytes.size(); ++i) {
        switch (static_cast<int>(bytes.at(i))) {
        default:        string += " "; break; //unsupported character. Simply use a space instead
        case 0x00:      string += "0"; break;
        case 0x01:      string += "1"; break;
        case 0x02:      string += "2"; break;
        case 0x03:      string += "3"; break;
        case 0x04:      string += "4"; break;
        case 0x05:      string += "5"; break;
        case 0x06:      string += "6"; break;
        case 0x07:      string += "7"; break;
        case 0x08:      string += "8"; break;
        case 0x09:      string += "9"; break;
        case 0x0A:      string += "A"; break;
        case 0x0B:      string += "B"; break;
        case 0x0C:      string += "C"; break;
        case 0x0D:      string += "D"; break;
        case 0x0E:      string += "E"; break;
        case 0x0F:      string += "F"; break;
        case 0x10:      string += "G"; break;
        case 0x11:      string += "H"; break;
        case 0x12:      string += "I"; break;
        case 0x13:      string += "J"; break;
        case 0x14:      string += "K"; break;
        case 0x15:      string += "L"; break;
        case 0x16:      string += "M"; break;
        case 0x17:      string += "N"; break;
        case 0x18:      string += "O"; break;
        case 0x19:      string += "P"; break;
        case 0x1A:      string += "Q"; break;
        case 0x1B:      string += "R"; break;
        case 0x1C:      string += "S"; break;
        case 0x1D:      string += "T"; break;
        case 0x1E:      string += "U"; break;
        case 0x1F:      string += "V"; break;
        case 0x20:      string += "W"; break;
        case 0x21:      string += "X"; break;
        case 0x22:      string += "Y"; break;
        case 0x23:      string += "Z"; break;
        case 0x24:      string += " "; break;
        case 0x28:      string += "-"; break;
        case 0x2B:      string += "!"; break;
        case 0xAF:      string += "."; break;
        }
    }
    return string;
}

bool Text::Find_And_Replace_Next_Instance(const QString &oldText, QString newText, qint64 startingOffset, qint64 endingOffset) {
    if (newText.size() > oldText.size()) return false; //larger insertions are not supported
    if (newText.size() < oldText.size()) { //add spaces to pad the new text if it is too small
        int numberOfSpaces = oldText.size() - newText.size();
        QString spaces(numberOfSpaces, QChar(' '));
        newText += spaces;
    }
    assert(oldText.size() == newText.size());

    //Read the search area
    int amount = static_cast<int>(endingOffset - startingOffset);
    if (amount <= 0) return false;
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(startingOffset, amount, bytes)) return false;

    //Determine what to search for
    QByteArray oldTextBytes = this->Convert_String_To_SMB_Bytes(oldText);
    QByteArray newTextBytes = this->Convert_String_To_SMB_Bytes(newText);

    //Start the search
    int currentByte = 0;
    qint64 replaceOffset = 0;
    bool instanceFound = false;
    for (int i = 0; i < bytes.size() && !instanceFound; ++i) {
        if (bytes.at(i) == oldTextBytes.at(currentByte)) {
            ++currentByte;
            if (oldTextBytes.size() == currentByte) {
                instanceFound = true;
                replaceOffset = (startingOffset+i)-(currentByte-1);
            }
        } else {
            currentByte = 0;
        }
    }

    //Perform the replace
    if (!instanceFound) return false;
    return this->Write_Bytes_To_Offset(replaceOffset, newTextBytes);
}

QString Text::Get_Centered_Text(const QString &oldText, const QString &newText) {
    if (newText.size() >= oldText.size()) return newText;
    int offsetAmount = oldText.size() - newText.size();
    QString centeredText = newText+QString(QByteArray(offsetAmount/2, ' '));
    int spacesRemaining = oldText.size()-centeredText.size();
    centeredText = QString(QByteArray(spacesRemaining, ' '))+centeredText;
    return centeredText;
}
