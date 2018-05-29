#include "Text.h"

QByteArray Text::Convert_String_To_SMB_Bytes(const QString &string) {
    QByteArray bytes(string.size(), 0x00);
    for (int i = 0; i < string.size(); ++i) {
        switch (string.at(i).toLower().toLatin1()) {
        default:        bytes[i] = 0x24; break; //unsupported character. Simply use a space instead
        case ' ':       bytes[i] = 0x24; break;
        case '!':       bytes[i] = 0x2B; break;
        case '.':       bytes[i] = 0xAF; break;
        case '-':       bytes[i] = 0x28; break;
        case '0':       bytes[i] = 0x00; break;
        case '1':       bytes[i] = 0x01; break;
        case '2':       bytes[i] = 0x02; break;
        case '3':       bytes[i] = 0x03; break;
        case '4':       bytes[i] = 0x04; break;
        case '5':       bytes[i] = 0x05; break;
        case '6':       bytes[i] = 0x06; break;
        case '7':       bytes[i] = 0x07; break;
        case '8':       bytes[i] = 0x08; break;
        case '9':       bytes[i] = 0x09; break;
        case 'a':       bytes[i] = 0x0A; break;
        case 'b':       bytes[i] = 0x0B; break;
        case 'c':       bytes[i] = 0x0C; break;
        case 'd':       bytes[i] = 0x0D; break;
        case 'e':       bytes[i] = 0x0E; break;
        case 'f':       bytes[i] = 0x0F; break;
        case 'g':       bytes[i] = 0x10; break;
        case 'h':       bytes[i] = 0x11; break;
        case 'i':       bytes[i] = 0x12; break;
        case 'j':       bytes[i] = 0x13; break;
        case 'k':       bytes[i] = 0x14; break;
        case 'l':       bytes[i] = 0x15; break;
        case 'm':       bytes[i] = 0x16; break;
        case 'n':       bytes[i] = 0x17; break;
        case 'o':       bytes[i] = 0x18; break;
        case 'p':       bytes[i] = 0x19; break;
        case 'q':       bytes[i] = 0x1A; break;
        case 'r':       bytes[i] = 0x1B; break;
        case 's':       bytes[i] = 0x1C; break;
        case 't':       bytes[i] = 0x1D; break;
        case 'u':       bytes[i] = 0x1E; break;
        case 'v':       bytes[i] = 0x1F; break;
        case 'w':       bytes[i] = 0x20; break;
        case 'x':       bytes[i] = 0x21; break;
        case 'y':       bytes[i] = 0x22; break;
        case 'z':       bytes[i] = 0x23; break;
        }
    }
    return bytes;
}

bool Text::Replace_But_Our_Princess_Is_In_Another_Castle_With_Random_Phrase() {
    //TODO: Write this!
}

bool Text::Replace_But_Our_Princess_Is_In_Another_Castle(const QString &string) {
    //TODO: Write this!
}
