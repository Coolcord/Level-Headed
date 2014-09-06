#include "Binary_Manipulator.h"

QString Binary_Manipulator::Hex_To_Binary_String(int hex) {
    //Make sure the byte passed in is valid
    if (hex > 0xFF) throw "Value cannot be greater than 0xFF";
    else if (hex < 0x00) throw "Value cannot be less than 0x00";

    QString binary = "";
    for (int i = 128; i >= 1; i /= 2) {
        hex -= i;
        if (hex >= 0) {
            binary += "1";
        } else {
            hex += i;
            binary += "0";
        }
    }
    return binary;
}

QString Binary_Manipulator::Hex_Digit_To_Binary_String(int hexDigit) {
    //Make sure the byte passed in is valid
    if (hexDigit < 0x0 || hexDigit > 0xF) {
        throw "A value between 0x0 and 0xF is expected";
    }

    QString binary = "";
    for (int i = 8; i >= 1; i /= 2) {
        hexDigit -= i;
        if (hexDigit >= 0) {
            binary += "1";
        } else {
            hexDigit += i;
            binary += "0";
        }
    }
    return binary;
}

QBitArray Binary_Manipulator::Hex_To_BitArray(int hex) {
    //Make sure the byte passed in is valid
    if (hex < 0x0 || hex > 0xFF) {
        throw "A value between 0x00 and 0xFF is expected";
    }
    QBitArray bits(8, false);

    for (int i = 128, j = 0; i >= 1; i /= 2, ++j) {
        hex -= i;
        if (hex >= 0) {
            bits.setBit(j, true);
        } else {
            hex += i;
            bits.setBit(j, false);
        }
    }
    return bits;
}

QBitArray Binary_Manipulator::Hex_Digit_To_BitArray(int hexDigit) {
    //Make sure the byte passed in is valid
    if (hexDigit < 0x0 || hexDigit > 0xF) {
        throw "A value between 0x0 and 0xF is expected";
    }
    QBitArray bits(4, false);

    for (int i = 8, j = 0; i >= 1; i /= 2, ++j) {
        hexDigit -= i;
        if (hexDigit >= 0) {
            bits.setBit(j, true);
        } else {
            hexDigit += i;
            bits.setBit(j, false);
        }
    }
    return bits;
}

int Binary_Manipulator::Binary_String_To_Hex(const QString &binary) {
    int value = 0;
    for (int i = binary.length() - 1, j = 1; i >= 0; --i) {
        if (binary[i] != '0') value += j;
        j *= 2;
    }
    return value;
}

int Binary_Manipulator::BitArray_To_Hex(const QBitArray &bits, int start, int end) {
    if (end >= bits.size()) {
        throw "End cannot be greater than the bit vector size!";
    }
    int value = 0;
    for (int i = end, j = 1; i >= start; --i) {
        if (bits.testBit(i)) value += j;
        j *= 2;
    }
    return value;
}

int Binary_Manipulator::BitArray_To_Hex(const QBitArray &bits) {
    return BitArray_To_Hex(bits, 0, (bits.size() - 1));
}

QString Binary_Manipulator::BitArray_To_String(const QBitArray &bits) {
    QString binary = "";

    for (int i = 0; i < bits.size(); ++i) {
        if (bits.testBit(i)) binary += "1";
        else binary += "0";
    }
    return binary;
}

QBitArray Binary_Manipulator::Combine_BitArrays(const QBitArray &a, const QBitArray &b) {
    QBitArray bits(a.size()+b.size());
    for (int i = 0; i < a.size(); ++i) {
        bits.setBit(i, a.testBit(i));
    }
    for (int i = a.size(), j = 0; i < (a.size()+b.size()); ++i, ++j) {
        bits.setBit(i, b.testBit(j));
    }
    return bits;
}

void Binary_Manipulator::Write_Hex_Digit_To_BitArray(QBitArray &bits, int start, int hexDigit) {
    return Write_Hex_Digit_To_BitArray(bits, start, hexDigit, 0, 3);
}

void Binary_Manipulator::Write_Hex_Digit_To_BitArray(QBitArray &bits, int start, int hexDigit, unsigned int hexStart, unsigned int hexEnd) {
    //Make sure the byte passed in is valid
    if (start > bits.size() || start < 0) throw "Start must be a proper index";
    else if (hexDigit < 0x0 || hexDigit > 0xF) throw "A hex digit is expected";
    else if (hexStart > 3 || hexEnd > 3) throw "Start and end must be between 0 and 3";

    QBitArray hexDigitBits = Hex_Digit_To_BitArray(hexDigit);
    if (start + hexDigitBits.size() > bits.size()) throw "Cannot write outside of the bit array";

    for (unsigned int i = start, j = hexStart; j <= hexEnd; ++i, ++j) {
        bits.setBit(i, hexDigitBits.testBit(j));
    }
}

void Binary_Manipulator::Write_Byte_To_BitArray(QBitArray &bits, int start, int hex) {
    //Make sure the byte passed in is valid
    if (start > bits.size() || start < 0) throw "Start must be a proper index";
    else if (hex < 0x00 || hex > 0xFF) throw "A hex digit is expected";

    QBitArray hexBits = Hex_To_BitArray(hex);
    if (start + hexBits.size() > bits.size()) throw "Cannot write outside of the bit array";

    for (int i = start, j = 0; i < start + bits.size(); ++i, ++j) {
        bits.setBit(i, hexBits.testBit(j));
    }
}

bool Binary_Manipulator::Is_Valid_Hex_Digit(int hexDigit) {
    return (hexDigit >= 0x0 && hexDigit <= 0xF);
}
