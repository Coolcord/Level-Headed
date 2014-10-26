#ifndef BINARY_MANIPULATOR_H
#define BINARY_MANIPULATOR_H

#include <QString>
#include <QByteArray>
#include <QBitArray>

namespace Binary_Manipulator {
    QString Hex_To_Binary_String(unsigned char hex);
    QString Hex_Digit_To_Binary_String(unsigned char hexDigit);
    QBitArray Hex_To_BitArray(unsigned char hex);
    QBitArray Hex_Digit_To_BitArray(unsigned char hexDigit);
    unsigned char Binary_String_To_Hex(const QString &binary);
    unsigned char BitArray_To_Hex(const QBitArray &bits, int start, int end);
    unsigned char BitArray_To_Hex(const QBitArray &bits);
    QString BitArray_To_String(const QBitArray &bits);
    QBitArray Combine_BitArrays(const QBitArray &a, const QBitArray &b);
    void Write_Hex_Digit_To_BitArray(QBitArray &bits, int start, unsigned char hexDigit);
    void Write_Hex_Digit_To_BitArray(QBitArray &bits, int start, unsigned char hexDigit, unsigned int hexStart, unsigned int hexEnd);
    void Write_Byte_To_BitArray(QBitArray &bits, int start, int hex);
    bool Is_Valid_Hex_Digit(unsigned char hexDigit);
    unsigned char Get_First_Digit_From_Hex(unsigned char hex);
    unsigned char Get_Second_Digit_From_Hex(unsigned char hex);
}

#endif // BINARY_MANIPULATOR_H
