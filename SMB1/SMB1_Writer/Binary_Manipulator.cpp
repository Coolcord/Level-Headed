#include "Binary_Manipulator.h"
#include <assert.h>

QString Binary_Manipulator::Hex_To_Binary_String(unsigned char hex) {
    int value = static_cast<int>(hex);
    QString binary = "";
    for (int i = 128; i >= 1; i /= 2) {
        value -= i;
        if (value >= 0) {
            binary += "1";
        } else {
            value += i;
            binary += "0";
        }
    }
    return binary;
}

QString Binary_Manipulator::Hex_Digit_To_Binary_String(unsigned char hexDigit) {
    int value = static_cast<int>(hexDigit);
    QString binary = "";
    for (int i = 8; i >= 1; i /= 2) {
        value -= i;
        if (value >= 0) {
            binary += "1";
        } else {
            value += i;
            binary += "0";
        }
    }
    return binary;
}

QBitArray Binary_Manipulator::Hex_To_BitArray(unsigned char hex) {
    int value = static_cast<int>(hex);
    QBitArray bits(8, false);
    for (int i = 128, j = 0; i >= 1; i /= 2, ++j) {
        value -= i;
        if (value >= 0) {
            bits.setBit(j, true);
        } else {
            value += i;
            bits.setBit(j, false);
        }
    }
    return bits;
}

QBitArray Binary_Manipulator::Hex_Digit_To_BitArray(unsigned char hexDigit) {
    int value = static_cast<int>(hexDigit);
    QBitArray bits(4, false);
    for (int i = 8, j = 0; i >= 1; i /= 2, ++j) {
        value -= i;
        if (value >= 0) {
            bits.setBit(j, true);
        } else {
            value += i;
            bits.setBit(j, false);
        }
    }
    return bits;
}

unsigned char Binary_Manipulator::Binary_String_To_Hex(const QString &binary) {
    unsigned char value = 0;
    for (int i = binary.length() - 1, j = 1; i >= 0; --i) {
        if (binary[i] != '0') value += j;
        j *= 2;
    }
    return value;
}

unsigned char Binary_Manipulator::BitArray_To_Hex(const QBitArray &bits, int start, int end) {
    assert(end < bits.size());
    unsigned char value = 0;
    for (int i = end, j = 1; i >= start; --i) {
        if (bits.testBit(i)) value += j;
        j *= 2;
    }
    return value;
}

unsigned char Binary_Manipulator::BitArray_To_Hex(const QBitArray &bits) {
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

void Binary_Manipulator::Write_Hex_Digit_To_BitArray(QBitArray &bits, int start, unsigned char hexDigit) {
    return Write_Hex_Digit_To_BitArray(bits, start, hexDigit, 0, 3);
}

void Binary_Manipulator::Write_Hex_Digit_To_BitArray(QBitArray &bits, int start, unsigned char hexDigit, unsigned int hexStart, unsigned int hexEnd) {
    //Make sure the byte passed in is valid
    assert(start <= bits.size() && start >= 0);
    assert(hexDigit >= 0x0 && hexDigit <= 0xF);
    assert(hexStart <= 3 && hexEnd <= 3);

    QBitArray hexDigitBits = Hex_Digit_To_BitArray(hexDigit);
    assert(hexDigitBits.size() <= bits.size());

    for (int i = start, j = hexStart; j <= hexEnd; ++i, ++j) {
        bits.setBit(i, hexDigitBits.testBit(j));
    }
}

void Binary_Manipulator::Write_Byte_To_BitArray(QBitArray &bits, int start, int hex) {
    //Make sure the byte passed in is valid
    assert(start <= bits.size() && start >= 0);
    assert(hex >= 0x00 && hex <= 0xFF);

    QBitArray hexBits = Hex_To_BitArray(hex);
    if (start + hexBits.size() > bits.size()) throw "Cannot write outside of the bit array";

    for (int i = start, j = 0; i < start + bits.size(); ++i, ++j) {
        bits.setBit(i, hexBits.testBit(j));
    }
}

bool Binary_Manipulator::Is_Valid_Hex_Digit(unsigned char hexDigit) {
    return (hexDigit <= 0xF);
}

unsigned char Binary_Manipulator::Get_First_Digit_From_Hex(unsigned char hex) {
    QBitArray bits = Hex_To_BitArray(hex);
    return BitArray_To_Hex(bits, 0, 3);
}

unsigned char Binary_Manipulator::Get_Second_Digit_From_Hex(unsigned char hex) {
    QBitArray bits = Hex_To_BitArray(hex);
    return BitArray_To_Hex(bits, 4, 7);
}
